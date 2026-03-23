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

class MetricsRecorder : public Component {
 public:
  MetricsRecorder(http_request::HttpRequestComponent* http, bool enable_sample_on_change, uint32_t sample_interval, uint_fast16_t max_samples_per_metric,
                  MetricsAutoSensorDetection autodetection, bool name_from_device_class);

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void loop() override;

  // void set_protocol( const std::string& protocol );
  void set_endpoint(const std::string& endpoint);
  void add_header(const std::string& header_key, const std::string& header_value);
  void add_resource_attribute(const std::string& attr_key, const std::string& attr_value);
  std::map<std::string, std::string>* get_resource_attributes();

  void add_metric(Metric* metric);
  const std::vector<Metric*> get_metrics();
  bool submit_metrics();
  void sample_metrics();

 protected:
  std::string endpoint;
  http_request::HttpRequestComponent* http;
  std::vector<http_request::Header> headers;
  std::map<std::string, std::string> resource_attributes;

  MetricsAutoSensorDetection autodetection = MetricsAutoSensorDetection::SENSORS_ALL;
  bool name_from_device_class = true;
  bool enable_sample_on_change = false;
  uint32_t sample_interval = 0;
  uint_fast16_t max_samples_per_metric = 50;
  std::vector<Metric*> metrics;
};

}  // namespace otel
}  // namespace esphome