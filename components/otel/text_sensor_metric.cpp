// SPDX-License-Identifier: GPL-3.0-or-later

#include "text_sensor_metric.h"

#ifdef USE_TEXT_SENSOR

#include <esphome/core/log.h>
#include <opentelemetry/proto/metrics/v1/metrics.pb.h>

#include "metrics_recorder.h"
#include "utils.h"

namespace esphome {
namespace otel {

static const char *const TAG = "OTLP.TextSensorMetric";

static const char *STR_TAG_TEXT = "text";

// This is basically the same as nanopb_encode_dpt() but adds a per dpt attribute with the selected value as string
bool nanopb_encode_text_dpt(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
  TextSensorMetric *metric = (TextSensorMetric *)(*arg);
  std::map<const char *, const char *> attributes;
  for (auto sample : metric->get_values()) {
    opentelemetry_proto_metrics_v1_NumberDataPoint dpt = opentelemetry_proto_metrics_v1_NumberDataPoint_init_zero;
    dpt.time_unix_nano = sample.first;
    dpt.which_value = opentelemetry_proto_metrics_v1_NumberDataPoint_as_double_tag;
    dpt.value.as_double = 1;

    attributes.clear();
    attributes.insert(std::pair(STR_TAG_TEXT, sample.second.c_str()));
    dpt.attributes.arg = &attributes;
    dpt.attributes.funcs.encode = nanopb_encode_c_str_attributes;

    // Build the submessage tag
    if (!pb_encode_tag_for_field(stream, field)) {
      ESP_LOGE(TAG, "DPT pb_encode_tag_for_field error: %s", PB_GET_ERROR(stream));
      return false;
    }
    // Build the submessage payload
    if (!pb_encode_submessage(stream, opentelemetry_proto_metrics_v1_NumberDataPoint_fields, &dpt)) {
      ESP_LOGE(TAG, "DPT pb_encode_submessage error: %s", PB_GET_ERROR(stream));
      return false;
    }
  }
  return true;
}

bool nanopb_encode_TextSensorMetric(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
  TextSensorMetric *esphome_metric = (TextSensorMetric *)(*arg);

  opentelemetry_proto_metrics_v1_Metric metric = opentelemetry_proto_metrics_v1_Metric_init_zero;
  metric.name.arg = esphome_metric->get_name();
  metric.name.funcs.encode = nanopb_encode_std_string;
  metric.description.arg = (void *)(esphome_metric->get_sensor()->get_name().c_str());
  metric.description.funcs.encode = nanopb_encode_c_string;
  metric.unit.arg = (void *)(esphome_metric->get_sensor()->get_unit_of_measurement_ref().c_str());
  metric.unit.funcs.encode = nanopb_encode_c_string;

  metric.which_data = opentelemetry_proto_metrics_v1_Metric_gauge_tag;
  metric.data.gauge.data_points.arg = esphome_metric;
  metric.data.gauge.data_points.funcs.encode = nanopb_encode_text_dpt;

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

TextSensorMetric::TextSensorMetric(MetricsRecorder *otel, text_sensor::TextSensor *sensor, bool name_from_device_class, uint_fast16_t max_samples)
    : Metric(otel, sensor, name_from_device_class, max_samples) {
  this->sensor = sensor;
}

EntityBase *TextSensorMetric::get_entity() { return this->sensor; }

void TextSensorMetric::install_sample_hook() {
  this->sensor->add_on_state_callback([this](const std::string &state) { this->sample(); });
}

void TextSensorMetric::sample() {
  if (!this->sensor->has_state()) return;
  std::string value = this->sensor->get_state();
  uint64_t timestamp = get_otlp_timestamp();
#ifdef OTLP_DROP_UNSYNCHRONIZED
  if (timestamp == 0) return;
#endif
  ESP_LOGV(TAG, "Recording text value to OTLP of %s: %s", this->sensor->get_name().c_str(), value.c_str());

  this->values.push_back(std::pair(timestamp, value));
  if (values.size() > this->max_samples) values.pop_front();

  // We need to put something in `samples` otherwise we'll not get called to submit
  if (samples.empty()) {
    this->samples.push_back(std::pair(timestamp, 0));
  }

  this->otel->enable_loop();
}

text_sensor::TextSensor *TextSensorMetric::get_sensor() { return this->sensor; }

std::list<std::pair<uint64_t, std::string>> &TextSensorMetric::get_values() { return this->values; }

encoding_function TextSensorMetric::get_nanopb_encoder() { return nanopb_encode_TextSensorMetric; }

}  // namespace otel
}  // namespace esphome

#endif
