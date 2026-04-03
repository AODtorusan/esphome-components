// SPDX-License-Identifier: GPL-3.0-or-later

#include "sensor_metric.h"

#ifdef USE_SENSOR

#include <esphome/core/log.h>
#include <opentelemetry/proto/metrics/v1/metrics.pb.h>

#include "metrics_recorder.h"
#include "utils.h"

namespace esphome {
namespace otel {

static const char *const TAG = "OTLP.SensorMetric";

static const char *STR_TAG_STATE_CLASS = "state_class";

bool nanopb_encode_SensorMetric(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
  SensorMetric *esphome_metric = (SensorMetric *)(*arg);

  opentelemetry_proto_metrics_v1_Metric metric = opentelemetry_proto_metrics_v1_Metric_init_zero;
  metric.name.arg = (void*) esphome_metric->get_name();
  metric.name.funcs.encode = nanopb_encode_c_string;
  metric.description.arg = (void *)(esphome_metric->get_sensor()->get_name().c_str());
  metric.description.funcs.encode = nanopb_encode_c_string;
  metric.unit.arg = (void *)(esphome_metric->get_sensor()->get_unit_of_measurement_ref().c_str());
  metric.unit.funcs.encode = nanopb_encode_c_string;
  switch (esphome_metric->get_sensor()->get_state_class()) {
    case sensor::STATE_CLASS_TOTAL:
      metric.which_data = opentelemetry_proto_metrics_v1_Metric_sum_tag;
      metric.data.sum.is_monotonic = false;
      metric.data.sum.aggregation_temporality = opentelemetry_proto_metrics_v1_AggregationTemporality_AGGREGATION_TEMPORALITY_CUMULATIVE;
      metric.data.sum.data_points.arg = esphome_metric;
      metric.data.sum.data_points.funcs.encode = nanopb_encode_dpt;
      break;
    case sensor::STATE_CLASS_TOTAL_INCREASING:
      metric.which_data = opentelemetry_proto_metrics_v1_Metric_sum_tag;
      metric.data.sum.is_monotonic = true;
      metric.data.sum.aggregation_temporality = opentelemetry_proto_metrics_v1_AggregationTemporality_AGGREGATION_TEMPORALITY_CUMULATIVE;
      metric.data.sum.data_points.arg = esphome_metric;
      metric.data.sum.data_points.funcs.encode = nanopb_encode_dpt;
      break;
    default:
      metric.which_data = opentelemetry_proto_metrics_v1_Metric_gauge_tag;
      metric.data.gauge.data_points.arg = esphome_metric;
      metric.data.gauge.data_points.funcs.encode = nanopb_encode_dpt;
  }

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

SensorMetric::SensorMetric(MetricsRecorder *otel, sensor::Sensor *sensor, MetricsNamingScheme naming_scheme, uint_fast16_t max_samples)
    : Metric(otel, sensor, "sensor", naming_scheme, max_samples) {
  this->sensor = sensor;
  this->add_attribute(STR_TAG_STATE_CLASS, LOG_STR_ARG(state_class_to_string(this->sensor->get_state_class())));
}

EntityBase *SensorMetric::get_entity() { return this->sensor; }

void SensorMetric::install_sample_hook() {
  this->sensor->add_on_state_callback([this](float state) { this->sample(); });
}

void SensorMetric::sample() {
  if (!this->sensor->has_state()) return;
  float value = this->sensor->get_state();
  uint64_t timestamp = get_otlp_timestamp();
#ifdef OTLP_DROP_UNSYNCHRONIZED
  if (timestamp == 0) return;
#endif
  ESP_LOGV(TAG, "Recording sensor value to OTLP of %s: %f", this->sensor->get_name().c_str(), value);
  this->samples.push_back(std::pair(timestamp, value));
  if (samples.size() > this->max_samples) samples.pop_front();
  this->otel->enable_loop();
}

sensor::Sensor *SensorMetric::get_sensor() { return this->sensor; }

encoding_function SensorMetric::get_nanopb_encoder() { return nanopb_encode_SensorMetric; }

}  // namespace otel
}  // namespace esphome

#endif
