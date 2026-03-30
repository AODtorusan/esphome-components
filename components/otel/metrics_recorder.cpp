// SPDX-License-Identifier: GPL-3.0-or-later

#include "metrics_recorder.h"

#include <esphome/components/network/util.h>
#include <opentelemetry/proto/common/v1/common.pb.h>
#include <opentelemetry/proto/metrics/v1/metrics.pb.h>
#include <opentelemetry/proto/resource/v1/resource.pb.h>

#include "binary_sensor_metric.h"
#include "number_metric.h"
#include "select_metric.h"
#include "sensor_metric.h"
#include "switch_metric.h"
#include "utils.h"

namespace esphome {
namespace otel {

static const char* const TAG = "OTLP.metrics_recorder";

bool nanopb_encode_scope_metrics(pb_ostream_t* stream, const pb_field_t* field, void* const* arg) {
  MetricsRecorder* metrics_recorder = (MetricsRecorder*)(*arg);
  for (auto* esphome_metric : metrics_recorder->get_metrics()) {
    if (esphome_metric->get_samples().empty()) continue;
    opentelemetry_proto_metrics_v1_ScopeMetrics scope_metric = opentelemetry_proto_metrics_v1_ScopeMetrics_init_zero;
    scope_metric.has_scope = true;
    scope_metric.scope.attributes.arg = (void*)(&(esphome_metric->get_attributes()));
    scope_metric.scope.attributes.funcs.encode = nanopb_encode_attributes;
    scope_metric.metrics.arg = esphome_metric;
    scope_metric.metrics.funcs.encode = esphome_metric->get_nanopb_encoder();

    // Build the submessage tag
    if (!pb_encode_tag_for_field(stream, field)) {
      ESP_LOGE(TAG, "ScopeMetrics pb_encode_tag_for_field error: %s", PB_GET_ERROR(stream));
      return false;
    }
    // Build the submessage payload
    if (!pb_encode_submessage(stream, opentelemetry_proto_metrics_v1_ScopeMetrics_fields, &scope_metric)) {
      ESP_LOGE(TAG, "ScopeMetrics pb_encode_submessage error: %s", PB_GET_ERROR(stream));
      return false;
    }
  }
  return true;
}

bool nanopb_encode_resource_metrics(pb_ostream_t* stream, const pb_field_t* field, void* const* arg) {
  MetricsRecorder* metrics_recorder = (MetricsRecorder*)(*arg);

  opentelemetry_proto_metrics_v1_ResourceMetrics resource_metrics = opentelemetry_proto_metrics_v1_ResourceMetrics_init_zero;
  resource_metrics.has_resource = true;
  resource_metrics.resource.dropped_attributes_count = 0;
  resource_metrics.resource.attributes.arg = metrics_recorder->get_resource_attributes();
  resource_metrics.resource.attributes.funcs.encode = nanopb_encode_attributes;
  resource_metrics.scope_metrics.arg = metrics_recorder;
  resource_metrics.scope_metrics.funcs.encode = nanopb_encode_scope_metrics;

  // Build the submessage tag
  if (!pb_encode_tag_for_field(stream, field)) {
    ESP_LOGE(TAG, "ResourceMetrics pb_encode_tag_for_field error: %s", PB_GET_ERROR(stream));
    return false;
  }
  // Build the submessage payload
  if (!pb_encode_submessage(stream, opentelemetry_proto_metrics_v1_ResourceMetrics_fields, &resource_metrics)) {
    ESP_LOGE(TAG, "ResourceMetrics pb_encode_submessage error: %s", PB_GET_ERROR(stream));
    return false;
  }
  return true;
}

MetricsRecorder::MetricsRecorder(http_request::HttpRequestComponent* http, bool enable_sample_on_change, uint32_t sample_interval,
                                 uint_fast16_t max_samples_per_metric, MetricsAutoSensorDetection autodetection, bool name_from_device_class) {
  this->http = http;
  this->enable_sample_on_change = enable_sample_on_change;
  this->sample_interval = sample_interval;
  this->max_samples_per_metric = max_samples_per_metric;
  this->autodetection = autodetection;
  this->name_from_device_class = name_from_device_class;
}

void MetricsRecorder::setup() {
  http_request::Header header;
  header.name = "Content-Type";
  header.value = "application/x-protobuf";
  this->headers.push_back(header);

  if (this->autodetection) {
    bool load_internal = (this->autodetection & MetricsAutoSensorDetection::SENSORS_INTERNAL) > 0;
    bool load_external = (this->autodetection & MetricsAutoSensorDetection::SENSORS_EXTERNAL) > 0;

#ifdef USE_BINARY_SENSOR
    for (auto* obj : App.get_binary_sensors()) {
      App.feed_wdt();
      if ((load_internal && obj->is_internal()) || (load_external && !obj->is_internal())) {
        // Check if the sensor was not manually defined
        if (std::none_of(this->metrics.begin(), this->metrics.end(), [&obj](Metric* m) { return m->get_entity() == obj; })) {
          ESP_LOGV(TAG, "Monitoring sensor: %s", obj->get_name().c_str());
          BinarySensorMetric* metric = new BinarySensorMetric(this, obj, this->name_from_device_class, this->max_samples_per_metric);
          this->metrics.push_back(metric);
        }
      }
    }
#endif
#ifdef USE_SENSOR
    for (auto* obj : App.get_sensors()) {
      App.feed_wdt();
      if ((load_internal && obj->is_internal()) || (load_external && !obj->is_internal())) {
        // Check if the sensor was not manually defined
        if (std::none_of(this->metrics.begin(), this->metrics.end(), [&obj](Metric* m) { return m->get_entity() == obj; })) {
          ESP_LOGV(TAG, "Monitoring sensor: %s", obj->get_name().c_str());
          SensorMetric* metric = new SensorMetric(this, obj, this->name_from_device_class, this->max_samples_per_metric);
          this->metrics.push_back(metric);
        }
      }
    }
#endif
#ifdef USE_NUMBER
    for (auto* obj : App.get_numbers()) {
      App.feed_wdt();
      if ((load_internal && obj->is_internal()) || (load_external && !obj->is_internal())) {
        // Check if the sensor was not manually defined
        if (std::none_of(this->metrics.begin(), this->metrics.end(), [&obj](Metric* m) { return m->get_entity() == obj; })) {
          ESP_LOGV(TAG, "Monitoring number: %s", obj->get_name().c_str());
          NumberMetric* metric = new NumberMetric(this, obj, this->name_from_device_class, this->max_samples_per_metric);
          this->metrics.push_back(metric);
        }
      }
    }
#endif
#ifdef USE_SELECT
    for (auto* obj : App.get_selects()) {
      App.feed_wdt();
      if ((load_internal && obj->is_internal()) || (load_external && !obj->is_internal())) {
        // Check if the sensor was not manually defined
        if (std::none_of(this->metrics.begin(), this->metrics.end(), [&obj](Metric* m) { return m->get_entity() == obj; })) {
          ESP_LOGV(TAG, "Monitoring select: %s", obj->get_name().c_str());
          SelectMetric* metric = new SelectMetric(this, obj, this->name_from_device_class, this->max_samples_per_metric);
          this->metrics.push_back(metric);
        }
      }
    }
#endif
#ifdef USE_SWITCH
    for (auto* obj : App.get_switches()) {
      App.feed_wdt();
      if ((load_internal && obj->is_internal()) || (load_external && !obj->is_internal())) {
        // Check if the sensor was not manually defined
        if (std::none_of(this->metrics.begin(), this->metrics.end(), [&obj](Metric* m) { return m->get_entity() == obj; })) {
          ESP_LOGV(TAG, "Monitoring switch: %s", obj->get_name().c_str());
          SwitchMetric* metric = new SwitchMetric(this, obj, this->name_from_device_class, this->max_samples_per_metric);
          this->metrics.push_back(metric);
        }
      }
    }
#endif
  }

  if (this->enable_sample_on_change) {
    for (auto* metric : this->metrics) {
      metric->install_sample_hook();
    }
  }

  if (this->sample_interval) {
    ESP_LOGV(TAG, "Enabling sampling of metrics every %d ms", this->sample_interval);
    this->set_interval("OTEL Sampling", this->sample_interval, std::bind(&MetricsRecorder::sample_metrics, this));
  }
}

void MetricsRecorder::dump_config() {
  ESP_LOGCONFIG(TAG, "OTEL MetricsRecorder:");
  ESP_LOGCONFIG(TAG, "    Endpoint: %s", this->endpoint.c_str());
  ESP_LOGCONFIG(TAG, "    Autodetection: %d", this->autodetection);
  ESP_LOGCONFIG(TAG, "    Sample on change: %d", this->enable_sample_on_change);
  ESP_LOGCONFIG(TAG, "    Samele interval: %d ms", this->sample_interval);
  ESP_LOGCONFIG(TAG, "    Max samples per metric: %lu", this->max_samples_per_metric);

  ESP_LOGCONFIG(TAG, "    Resource attributes:");
  for (auto& [key, value] : this->resource_attributes) {
    ESP_LOGCONFIG(TAG, "      %s=%s", key.c_str(), value.c_str());
  }

  ESP_LOGCONFIG(TAG, "    Entity metrics:");
  for (auto* esphome_metric : this->get_metrics()) {
    ESP_LOGCONFIG(TAG, "      %s", esphome_metric->get_entity()->get_name().c_str());
    ESP_LOGCONFIG(TAG, "        Name: %s", esphome_metric->get_name()->c_str());
    ESP_LOGCONFIG(TAG, "        Scope attributes:");
    for (auto& [key, value] : esphome_metric->get_attributes()) {
      ESP_LOGCONFIG(TAG, "          %s=%s", key.c_str(), value.c_str());
    }
  }
}

float MetricsRecorder::get_setup_priority() const { return setup_priority::LATE; }

void MetricsRecorder::loop() {
  if (!network::is_connected()) {
    // We need to wait for network to be available
    static uint32_t t_last = 0;
    uint32_t t_now = millis();
    if (t_now - t_last > 3000) {
      ESP_LOGW(TAG, "Cant submit, network not connected");
      t_last = t_now;
    }
    return;
  }

  bool success = submit_metrics();
  if (success) {
    this->disable_loop();
  }
}

// void OTEL::set_protocol( const std::string& protocol ) {}

void MetricsRecorder::set_endpoint(const std::string& endpoint) { this->endpoint = endpoint + "/v1/metrics"; }

void MetricsRecorder::add_header(const std::string& header_key, const std::string& header_value) {
  http_request::Header header;
  header.name = header_key;
  header.value = header_value;
  this->headers.push_back(header);
}

void MetricsRecorder::add_resource_attribute(const std::string& attr_key, const std::string& attr_value) {
  this->resource_attributes.insert_or_assign(attr_key, attr_value);
}

std::map<std::string, std::string>* MetricsRecorder::get_resource_attributes() { return &this->resource_attributes; }

void MetricsRecorder::add_metric(Metric* metric) { this->metrics.push_back(metric); }

const std::vector<Metric*> MetricsRecorder::get_metrics() { return this->metrics; }

bool MetricsRecorder::submit_metrics() {
  bool no_samples = true;
  for (auto* esphome_metric : this->metrics) {
    if (!esphome_metric->get_samples().empty()) {
      no_samples = false;
      break;
    }
  }

  if (no_samples) {
    ESP_LOGD(TAG, "No metrics to submit");
    return true;
  }
  ESP_LOGD(TAG, "Publishing OTLP metrics");

  // Create a Metrics message
  opentelemetry_proto_metrics_v1_MetricsData metricsdata = opentelemetry_proto_metrics_v1_MetricsData_init_zero;
  metricsdata.resource_metrics.arg = this;
  metricsdata.resource_metrics.funcs.encode = nanopb_encode_resource_metrics;

  // Serialize the protobuf message
  std::string buffer;
  pb_ostream_t stream = pb_ostream_from_string(buffer);
  App.feed_wdt();
  bool status = pb_encode(&stream, opentelemetry_proto_metrics_v1_MetricsData_fields, &metricsdata);
  App.feed_wdt();
  if (!status) {
    ESP_LOGE(TAG, "Failed to encode metrics protobuf message");
    ESP_LOGE(TAG, "error: %s", PB_GET_ERROR(&stream));
    return false;
  }
  size_t message_length = stream.bytes_written;

  auto response = this->http->post(this->endpoint, buffer, this->headers);

  if (response == nullptr) {
    ESP_LOGW(TAG, "Error sending metrics to %s", this->endpoint.c_str());
    return false;
  }
  auto status_code = response->status_code;
  response->end();
  if (!(status_code == 200 || status_code == 204)) {
    ESP_LOGW(TAG, "Error sending metrics to %s: HTTP %d", this->endpoint.c_str(), status_code);
    return false;
  }

  for (auto* esphome_metric : this->metrics) {
    esphome_metric->clear_samples();
  }
  return true;
}

void MetricsRecorder::sample_metrics() {
  ESP_LOGD(TAG, "Sampling metrics");
  for (auto* esphome_metric : this->metrics) {
    esphome_metric->sample();
  }
}

}  // namespace otel
}  // namespace esphome
