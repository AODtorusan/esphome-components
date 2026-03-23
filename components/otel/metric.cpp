// SPDX-License-Identifier: GPL-3.0-or-later

#include "metric.h"

#include <esphome/core/log.h>
#include <opentelemetry/proto/metrics/v1/metrics.pb.h>

#include <pb_encode.h>

namespace esphome {
namespace otel {

static const char* const TAG = "OTLP.metric";

bool nanopb_encode_dpt(pb_ostream_t* stream, const pb_field_t* field, void* const* arg) {
  Metric* metric = (Metric*)(*arg);
  for (auto sample : metric->get_samples()) {
    opentelemetry_proto_metrics_v1_NumberDataPoint dpt = opentelemetry_proto_metrics_v1_NumberDataPoint_init_zero;
    dpt.time_unix_nano = sample.first;
    dpt.which_value = opentelemetry_proto_metrics_v1_NumberDataPoint_as_double_tag;
    dpt.value.as_double = sample.second;
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

Metric::Metric(MetricsRecorder* otel, uint_fast16_t max_samples) {
  this->otel = otel;
  this->max_samples = max_samples;
}

void Metric::set_name(std::string name) { this->name = name; }

std::string* Metric::get_name() { return &this->name; }

void Metric::add_attribute(const std::string& attr_key, const std::string& attr_value) { this->attributes.insert_or_assign(attr_key, attr_value); }

const std::map<std::string, std::string>& Metric::get_attributes() const { return this->attributes; }

const std::list<std::pair<uint64_t, float>>& Metric::get_samples() const { return this->samples; }

void Metric::clear_samples() { this->samples.clear(); }

}  // namespace otel
}  // namespace esphome
