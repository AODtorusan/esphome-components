// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <esphome/core/defines.h>

#ifdef USE_LOG_LISTENERS

#include <cstdio>
#include <ctime>
#include <list>
#include <string>
#include <tuple>

#include <esphome/components/http_request/http_request.h>
#include <esphome/components/logger/logger.h>
#include <esphome/core/component.h>

namespace esphome {
namespace otel {

class LogsRecorder : public Component {
 public:
  LogsRecorder(http_request::HttpRequestComponent* http, int level);

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void loop() override;

  // void set_protocol( const std::string& protocol );
  void set_endpoint(const std::string& endpoint);
  void add_header(const std::string& header_key, const std::string& header_value);
  void add_resource_attribute(const std::string& attr_key, const std::string& attr_value);
  std::map<std::string, std::string>* get_resource_attributes();

  bool submit_logs();
  void on_log(uint8_t level, const char* tag, const char* message, size_t message_len);
  void set_logger_max_depth(uint_fast16_t depth);
  void add_logger_attribute(const std::string& attr_key, const std::string& attr_value);
  std::map<std::string, std::string>* get_logging_attributes();
  std::list<std::tuple<time_t, int, const char*, std::string>>* get_logs();

 protected:
  std::string endpoint;
  http_request::HttpRequestComponent* http;
  std::vector<http_request::Header> headers;
  std::map<std::string, std::string> resource_attributes;

  int log_level;
  uint_fast16_t logs_max_depth = 10;
  uint_fast16_t dropped_logs = 0;
  std::list<std::tuple<time_t, int, const char*, std::string>> logs;
  std::map<std::string, std::string> logging_attributes;
};

}  // namespace otel
}  // namespace esphome

#endif
