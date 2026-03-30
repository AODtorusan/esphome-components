// SPDX-License-Identifier: GPL-3.0-or-later

#include "select_metric.h"

#ifdef USE_SELECT

#include <esphome/core/log.h>
#include <opentelemetry/proto/metrics/v1/metrics.pb.h>

#include "metrics_recorder.h"
#include "utils.h"

namespace esphome {
namespace otel {

static const char *const TAG = "OTLP.SelectMetric";

static const char *STR_TAG_SELECTED = "selected";

// This is basically the same as nanopb_encode_dpt() but adds a per dpt attribute with the selected value as string
bool nanopb_encode_select_dpt(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
  SelectMetric *metric = (SelectMetric *)(*arg);
  std::map<const char *, const char *> attributes;
  for (auto sample : metric->get_samples()) {
    opentelemetry_proto_metrics_v1_NumberDataPoint dpt = opentelemetry_proto_metrics_v1_NumberDataPoint_init_zero;
    dpt.time_unix_nano = sample.first;
    dpt.which_value = opentelemetry_proto_metrics_v1_NumberDataPoint_as_double_tag;
    dpt.value.as_double = sample.second;

    attributes.clear();
    attributes.insert(std::pair(STR_TAG_SELECTED, metric->get_select()->option_at(sample.second)));
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

bool nanopb_encode_SelectMetric(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
  SelectMetric *esphome_metric = (SelectMetric *)(*arg);

  opentelemetry_proto_metrics_v1_Metric metric = opentelemetry_proto_metrics_v1_Metric_init_zero;
  metric.name.arg = esphome_metric->get_name();
  metric.name.funcs.encode = nanopb_encode_std_string;
  metric.description.arg = (void *)(esphome_metric->get_select()->get_name().c_str());
  metric.description.funcs.encode = nanopb_encode_c_string;
  metric.unit.arg = (void *)(esphome_metric->get_select()->get_unit_of_measurement_ref().c_str());
  metric.unit.funcs.encode = nanopb_encode_c_string;

  metric.which_data = opentelemetry_proto_metrics_v1_Metric_gauge_tag;
  metric.data.gauge.data_points.arg = esphome_metric;
  metric.data.gauge.data_points.funcs.encode = nanopb_encode_select_dpt;

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

SelectMetric::SelectMetric(MetricsRecorder *otel, select::Select *sensor, bool name_from_device_class, uint_fast16_t max_samples)
    : Metric(otel, sensor, name_from_device_class, max_samples) {
  this->select = sensor;
}

EntityBase *SelectMetric::get_entity() { return this->select; }

void SelectMetric::install_sample_hook() {
  this->select->add_on_state_callback([this](float state) { this->sample(); });
}

void SelectMetric::sample() {
  if (!this->select->has_state()) return;
  float value = this->select->active_index().value();
  uint64_t timestamp = get_otlp_timestamp();
#ifdef OTLP_DROP_UNSYNCHRONIZED
  if (timestamp == 0) return;
#endif
  ESP_LOGV(TAG, "Recording select value to OTLP of %s: %f", this->select->get_name().c_str(), value);
  this->samples.push_back(std::pair(timestamp, value));
  if (samples.size() > this->max_samples) samples.pop_front();
  this->otel->enable_loop();
}

select::Select *SelectMetric::get_select() { return this->select; }

encoding_function SelectMetric::get_nanopb_encoder() { return nanopb_encode_SelectMetric; }

}  // namespace otel
}  // namespace esphome

#endif
