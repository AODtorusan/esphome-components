// SPDX-License-Identifier: GPL-3.0-or-later

#include "metric.h"

#include <esphome/core/entity_base.h>
#include <esphome/core/log.h>
#include <opentelemetry/proto/metrics/v1/metrics.pb.h>

#include "metrics_recorder.h"
#include "utils.h"
#include <pb_encode.h>

namespace esphome {
namespace otel {

static const char* const TAG = "OTLP.metric";

static const char* STR_NAME_UNKNOWN = "unknown";

static const char* STR_TAG_ENTITY_NAME  = "entity_name";
static const char* STR_TAG_ENTITY_ID    = "entity_id";
static const char* STR_TAG_ENTITY_TYPE  = "entity_type";
static const char* STR_TAG_UNIT         = "unit";
static const char* STR_TAG_DEVICE_CLASS = "device_class";
static const char* STR_TAG_STATE_CLASS  = "state_class";

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
  metric.name.arg = (void*)esphome_metric->get_name();
  metric.name.funcs.encode = nanopb_encode_c_string;
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

Metric::Metric(MetricsRecorder* otel, EntityBase* entity, const char* entity_type, MetricsNamingScheme naming_scheme, uint_fast16_t max_samples) {
  this->otel = otel;
  this->max_samples = max_samples;

  // This is not efficient ...
  char dc_buf[MAX_DEVICE_CLASS_LENGTH];
  const char* dc = entity->get_device_class_to(dc_buf);
  device_class = dc;

  char oid_buf[OBJECT_ID_MAX_LEN];
  StringRef oid = entity->get_object_id_to(oid_buf);
  object_id = oid;

  switch (naming_scheme) {
    case MetricsNamingScheme::ENTITY_TYPE:
      this->set_name(entity_type);
      break;
    case MetricsNamingScheme::ENTITY_DEVICE_CLASS:
      if (!device_class.empty()) {
        this->set_name(device_class.c_str());
      } else {
        this->set_name(STR_NAME_UNKNOWN);
      }
      break;
    case MetricsNamingScheme::ENTITY_NAME:
    default:
      this->set_name(entity->get_name().c_str());
      break;
  }
  this->add_attribute(STR_TAG_ENTITY_NAME, entity->get_name().c_str());
  this->add_attribute(STR_TAG_ENTITY_ID, object_id.c_str());
  this->add_attribute(STR_TAG_ENTITY_TYPE, entity_type);

  if (!entity->get_unit_of_measurement_ref().empty()) {
    this->add_attribute(STR_TAG_UNIT, entity->get_unit_of_measurement_ref().c_str());
  }
  if (!device_class.empty()) {
    this->add_attribute(STR_TAG_DEVICE_CLASS, device_class.c_str());
  }
}

void Metric::set_name(const char* name) { this->name = name; }

const char* Metric::get_name() { return this->name; }

void Metric::add_attribute(const char* attr_key, const char* attr_value) { this->attributes.insert_or_assign(attr_key, attr_value); }

std::map<const char*, const char*>* Metric::get_attributes() { return &(this->attributes); }

const std::list<std::pair<uint64_t, float>>& Metric::get_samples() const { return this->samples; }

void Metric::clear_samples() { this->samples.clear(); }

encoding_function Metric::get_nanopb_encoder() { return nanopb_encode_Metric; }

}  // namespace otel
}  // namespace esphome
