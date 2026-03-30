// SPDX-License-Identifier: GPL-3.0-or-later

#include "metric.h"

#include <esphome/core/entity_base.h>
#include <esphome/core/log.h>
#include <opentelemetry/proto/metrics/v1/metrics.pb.h>

#include "utils.h"
#include <pb_encode.h>

namespace esphome {
namespace otel {

static const char* const TAG = "OTLP.metric";

static const std::string STR_TAG_SENSOR_NAME = "sensor_name";
static const std::string STR_TAG_UNIT = "unit";
static const std::string STR_TAG_DEVICE_CLASS = "device_class";
static const std::string STR_TAG_STATE_CLASS = "state_class";

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

bool nanopb_encode_Metric(pb_ostream_t* stream, const pb_field_t* field, void* const* arg) {
  Metric* esphome_metric = (Metric*)(*arg);

  opentelemetry_proto_metrics_v1_Metric metric = opentelemetry_proto_metrics_v1_Metric_init_zero;
  metric.name.arg = esphome_metric->get_name();
  metric.name.funcs.encode = nanopb_encode_std_string;
  metric.description.arg = (void*)(esphome_metric->get_entity()->get_name().c_str());
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

Metric::Metric(MetricsRecorder* otel, EntityBase* entity, bool name_from_device_class, uint_fast16_t max_samples) {
  this->otel = otel;
  this->max_samples = max_samples;

  // This is not efficient ...
  char dc_buf[MAX_DEVICE_CLASS_LENGTH];
  const char* dc = entity->get_device_class_to(dc_buf);
  std::string device_class(dc);

  if (name_from_device_class) {
    if (!device_class.empty()) {
      this->set_name(device_class);
    } else {
      this->set_name("unknown");
    }
  } else {
    this->set_name(entity->get_name());
  }

  this->add_attribute(STR_TAG_SENSOR_NAME, entity->get_name());

  if (!entity->get_unit_of_measurement_ref().empty()) {
    this->add_attribute(STR_TAG_UNIT, entity->get_unit_of_measurement_ref());
  }
  if (!device_class.empty()) {
    this->add_attribute(STR_TAG_DEVICE_CLASS, device_class);
  }
}

void Metric::set_name(std::string name) { this->name = name; }

std::string* Metric::get_name() { return &this->name; }

void Metric::add_attribute(const std::string& attr_key, const std::string& attr_value) { this->attributes.insert_or_assign(attr_key, attr_value); }

const std::map<std::string, std::string>& Metric::get_attributes() const { return this->attributes; }

const std::list<std::pair<uint64_t, float>>& Metric::get_samples() const { return this->samples; }

void Metric::clear_samples() { this->samples.clear(); }

encoding_function Metric::get_nanopb_encoder() { return nanopb_encode_Metric; }

}  // namespace otel
}  // namespace esphome
