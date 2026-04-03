// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstdint>
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>

#include <esphome/components/http_request/http_request.h>
#include <esphome/core/component.h>
#include <esphome/core/defines.h>

#include "metric.h"

namespace esphome {
namespace otel {

enum MetricsAutoSensorDetection : uint8_t {
  SENSORS_NONE = 0b00,
  SENSORS_INTERNAL = 0b01,
  SENSORS_EXTERNAL = 0b10,
  SENSORS_ALL = 0b11,
};

enum MetricsNamingScheme : uint8_t {
  ENTITY_NAME = 0,
  ENTITY_TYPE = 1,
  ENTITY_DEVICE_CLASS = 2,
};

class MetricsRecorder : public Component {
 public:
  MetricsRecorder(http_request::HttpRequestComponent* http, bool enable_sample_on_change, uint32_t sample_interval, uint_fast16_t max_samples_per_metric,
                  MetricsAutoSensorDetection autodetection, bool autodetect_text_sensors, MetricsNamingScheme naming_scheme);

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void loop() override;

  // void set_protocol( const std::string& protocol );
  void set_endpoint(const char* endpoint);
  void add_header(const char* header_key, const char* header_value);
  void add_resource_attribute(const char* attr_key, const char* attr_value);
  std::map<const char*, const char*>* get_resource_attributes();

  void add_metric(Metric* metric);
  const std::vector<Metric*> get_metrics();
  bool submit_metrics();
  void sample_metrics();

 protected:
  std::string endpoint;
  http_request::HttpRequestComponent* http;
  std::vector<http_request::Header> headers;
  std::map<const char*, const char*> resource_attributes;

  MetricsNamingScheme naming_scheme = MetricsNamingScheme::ENTITY_NAME;
  MetricsAutoSensorDetection autodetection = MetricsAutoSensorDetection::SENSORS_ALL;
  bool autodetect_text_sensors = false;
  bool enable_sample_on_change = false;
  uint32_t sample_interval = 0;
  uint_fast16_t max_samples_per_metric = 50;
  std::vector<Metric*> metrics;
};

}  // namespace otel
}  // namespace esphome
