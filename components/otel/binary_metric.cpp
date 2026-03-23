// SPDX-License-Identifier: GPL-3.0-or-later

#include "binary_metric.h"

#ifdef USE_BINARY_SENSOR

#include <esphome/core/log.h>
#include <opentelemetry/proto/metrics/v1/metrics.pb.h>

#include "metrics_recorder.h"
#include "utils.h"

namespace esphome {
namespace otel {

static const char* const TAG = "OTLP.binarymetric";

static const std::string STR_TAG_SENSOR_NAME = "sensor_name";

bool nanopb_encode_binarymetric(pb_ostream_t* stream, const pb_field_t* field, void* const* arg) {
  BinaryMetric* esphome_metric = (BinaryMetric*)(*arg);

  opentelemetry_proto_metrics_v1_Metric metric = opentelemetry_proto_metrics_v1_Metric_init_zero;
  metric.name.arg = esphome_metric->get_name();
  metric.name.funcs.encode = nanopb_encode_std_string;
  metric.description.arg = (void*)(esphome_metric->get_sensor()->get_name().c_str());
  metric.description.funcs.encode = nanopb_encode_c_string;
  metric.which_data = opentelemetry_proto_metrics_v1_Metric_gauge_tag;
  metric.data.gauge.data_points.arg = esphome_metric;
  metric.data.gauge.data_points.funcs.encode = nanopb_encode_dpt;

  // Build the submessage tag
  if (!pb_encode_tag_for_field(stream, field)) {
    ESP_LOGE(TAG, "ScopeMetrics pb_encode_tag_for_field error: %s", PB_GET_ERROR(stream));
    return false;
  }
  // Build the submessage payload
  if (!pb_encode_submessage(stream, opentelemetry_proto_metrics_v1_Metric_fields, &metric)) {
    ESP_LOGE(TAG, "ScopeMetrics pb_encode_submessage error: %s", PB_GET_ERROR(stream));
    return false;
  }
  return true;
}

BinaryMetric::BinaryMetric(MetricsRecorder* otel, binary_sensor::BinarySensor* sensor, bool name_from_device_class, uint_fast16_t max_samples)
    : Metric(otel, max_samples) {
  this->sensor = sensor;
  if (name_from_device_class) {
    if (!this->sensor->get_device_class_ref().empty()) {
      this->set_name(sensor->get_device_class_ref());
    } else {
      this->set_name("unknown");
    }
  } else {
    this->set_name(sensor->get_name());
  }
  this->add_attribute(STR_TAG_SENSOR_NAME, sensor->get_name());
}

EntityBase* BinaryMetric::get_entity() { return this->sensor; }

void BinaryMetric::install_sample_hook() {
  this->sensor->add_on_state_callback([this](bool state) { this->sample(); });
}

void BinaryMetric::sample() {
  float value = this->sensor->get_state();
  uint64_t timestamp = get_otlp_timestamp();
#ifdef OTLP_DROP_UNSYNCHRONIZED
  if (timestamp == 0) return;
#endif
  ESP_LOGV(TAG, "Recording sensor value to OTLP of %s: %f", this->sensor->get_name(), value);
  this->samples.push_back(std::pair(timestamp, value));
  if (samples.size() > this->max_samples) samples.pop_front();
  this->otel->enable_loop();
}

binary_sensor::BinarySensor* BinaryMetric::get_sensor() { return this->sensor; }

encoding_function BinaryMetric::get_nanopb_encoder() { return nanopb_encode_binarymetric; }

}  // namespace otel
}  // namespace esphome

#endif
