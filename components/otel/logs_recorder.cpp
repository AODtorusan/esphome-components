// SPDX-License-Identifier: GPL-3.0-or-later

#include "logs_recorder.h"

#ifdef USE_LOG_LISTENERS

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <map>
#include <utility>

#include <esphome/components/network/util.h>
#include <esphome/core/log.h>
#include <opentelemetry/proto/common/v1/common.pb.h>
#include <opentelemetry/proto/logs/v1/logs.pb.h>
#include <opentelemetry/proto/resource/v1/resource.pb.h>

#include "utils.h"

namespace esphome {
namespace otel {

static const char* const TAG = "OTLP.logs_recorder";
static const char* const TAG_ATTRIBUTE_NAME = "tag";

std::pair<_opentelemetry_proto_logs_v1_SeverityNumber, const char*> esphome_level_to_LogsRecorder[] = {
    /* 0 = NONE         */ std::pair(opentelemetry_proto_logs_v1_SeverityNumber_SEVERITY_NUMBER_UNSPECIFIED, "NONE"),
    /* 1 = ERROR        */ std::pair(opentelemetry_proto_logs_v1_SeverityNumber_SEVERITY_NUMBER_ERROR, "ERROR"),
    /* 2 = WARN         */ std::pair(opentelemetry_proto_logs_v1_SeverityNumber_SEVERITY_NUMBER_WARN, "WARN"),
    /* 3 = INFO         */ std::pair(opentelemetry_proto_logs_v1_SeverityNumber_SEVERITY_NUMBER_INFO, "INFO"),
    /* 4 = CONFIG       */ std::pair(opentelemetry_proto_logs_v1_SeverityNumber_SEVERITY_NUMBER_DEBUG3, "CONFIG"),
    /* 5 = DEBUG        */ std::pair(opentelemetry_proto_logs_v1_SeverityNumber_SEVERITY_NUMBER_DEBUG, "DEBUG"),
    /* 6 = VERBOSE      */ std::pair(opentelemetry_proto_logs_v1_SeverityNumber_SEVERITY_NUMBER_TRACE3, "VERBOSE"),
    /* 7 = VERY_VERBOSE */ std::pair(opentelemetry_proto_logs_v1_SeverityNumber_SEVERITY_NUMBER_TRACE, "VERY_VERBOSE")};

bool nanopb_encode_logs(pb_ostream_t* stream, const pb_field_t* field, void* const* arg) {
  std::list<std::tuple<time_t, int, const char*, std::string>>* logs = (std::list<std::tuple<time_t, int, const char*, std::string>>*)(*arg);
  for (auto& [time, level, tag, log_line] : *logs) {
    opentelemetry_proto_logs_v1_LogRecord log_record = opentelemetry_proto_logs_v1_LogRecord_init_zero;
    log_record.time_unix_nano = time;
    log_record.observed_time_unix_nano = log_record.time_unix_nano;
    std::pair<_opentelemetry_proto_logs_v1_SeverityNumber, const char*> log_level = esphome_level_to_LogsRecorder[level];
    log_record.severity_number = log_level.first;
    log_record.severity_text.arg = (void*)log_level.second;
    log_record.severity_text.funcs.encode = nanopb_encode_c_string;
    log_record.has_body = true;
    log_record.body.which_value = opentelemetry_proto_common_v1_AnyValue_string_value_tag;
    log_record.body.value.string_value.arg = (void*)&log_line;
    log_record.body.value.string_value.funcs.encode = nanopb_encode_std_string;
    // This should not be needed, but loki doesnt read the OTLP event_name field yet
    std::map<const char* const, const char* const> log_record_attributes = {{TAG_ATTRIBUTE_NAME, tag}};
    log_record.attributes.arg = &log_record_attributes;
    log_record.attributes.funcs.encode = nanopb_encode_c_str_attributes;
    log_record.event_name.arg = (void*)tag;
    log_record.event_name.funcs.encode = nanopb_encode_c_string;

    // Build the submessage tag
    if (!pb_encode_tag_for_field(stream, field)) {
      ESP_LOGE(TAG, "LogRecord pb_encode_tag_for_field error: %s", PB_GET_ERROR(stream));
      return false;
    }
    // Build the submessage payload
    if (!pb_encode_submessage(stream, opentelemetry_proto_logs_v1_LogRecord_fields, &log_record)) {
      ESP_LOGE(TAG, "LogRecord pb_encode_submessage error: %s", PB_GET_ERROR(stream));
      return false;
    }
  }
  return true;
}

bool nanopb_encode_scope_logs(pb_ostream_t* stream, const pb_field_t* field, void* const* arg) {
  LogsRecorder* logs_recorder = (LogsRecorder*)(*arg);

  opentelemetry_proto_logs_v1_ScopeLogs scope_logs = opentelemetry_proto_logs_v1_ScopeLogs_init_zero;
  scope_logs.has_scope = true;
  scope_logs.scope.attributes.arg = logs_recorder->get_logging_attributes();
  scope_logs.scope.attributes.funcs.encode = nanopb_encode_attributes;
  scope_logs.log_records.arg = logs_recorder->get_logs();
  scope_logs.log_records.funcs.encode = nanopb_encode_logs;

  // Build the submessage tag
  if (!pb_encode_tag_for_field(stream, field)) {
    ESP_LOGE(TAG, "ScopeLogs pb_encode_tag_for_field error: %s", PB_GET_ERROR(stream));
    return false;
  }
  // Build the submessage payload
  if (!pb_encode_submessage(stream, opentelemetry_proto_logs_v1_ScopeLogs_fields, &scope_logs)) {
    ESP_LOGE(TAG, "ScopeLogs pb_encode_submessage error: %s", PB_GET_ERROR(stream));
    return false;
  }
  return true;
}

bool nanopb_encode_resource_logs(pb_ostream_t* stream, const pb_field_t* field, void* const* arg) {
  LogsRecorder* logs_recorder = (LogsRecorder*)(*arg);

  opentelemetry_proto_logs_v1_ResourceLogs resource_logs = opentelemetry_proto_logs_v1_ResourceLogs_init_zero;
  resource_logs.has_resource = true;
  resource_logs.resource.dropped_attributes_count = 0;
  resource_logs.resource.attributes.arg = logs_recorder->get_resource_attributes();
  resource_logs.resource.attributes.funcs.encode = nanopb_encode_attributes;
  resource_logs.scope_logs.arg = logs_recorder;
  resource_logs.scope_logs.funcs.encode = nanopb_encode_scope_logs;

  // Build the submessage tag
  if (!pb_encode_tag_for_field(stream, field)) {
    ESP_LOGE(TAG, "ResourceLogs pb_encode_tag_for_field error: %s", PB_GET_ERROR(stream));
    return false;
  }
  // Build the submessage payload
  if (!pb_encode_submessage(stream, opentelemetry_proto_logs_v1_ResourceLogs_fields, &resource_logs)) {
    ESP_LOGE(TAG, "ResourceLogs pb_encode_submessage error: %s", PB_GET_ERROR(stream));
    return false;
  }
  return true;
}

LogsRecorder::LogsRecorder(http_request::HttpRequestComponent* http, int log_level) {
  this->http = http;
  this->log_level = log_level;
  logger::global_logger->add_log_callback(this, [](void* self, uint8_t level, const char* tag, const char* message, size_t message_len) {
    static_cast<LogsRecorder*>(self)->on_log(level, tag, message, message_len);
  });
}

void LogsRecorder::setup() {
  http_request::Header header;
  header.name = "Content-Type";
  header.value = "application/x-protobuf";
  this->headers.push_back(header);
}

void LogsRecorder::dump_config() {
  ESP_LOGCONFIG(TAG, "OTEL LogsRecorder:");
  ESP_LOGCONFIG(TAG, "    Endpoint: %s", this->endpoint.c_str());
  ESP_LOGCONFIG(TAG, "    Log level: %s", esphome_level_to_LogsRecorder[this->log_level].second);
  ESP_LOGCONFIG(TAG, "    Max logs: %lu", this->logs_max_depth);

  ESP_LOGCONFIG(TAG, "    Resource attributes:");
  for (auto& [key, value] : this->resource_attributes) {
    ESP_LOGCONFIG(TAG, "      %s=%s", key.c_str(), value.c_str());
  }

  ESP_LOGCONFIG(TAG, "    Logging scope attributes:");
  for (auto& [key, value] : this->logging_attributes) {
    ESP_LOGCONFIG(TAG, "      %s=%s", key.c_str(), value.c_str());
  }
}

float LogsRecorder::get_setup_priority() const { return setup_priority::LATE; }

void LogsRecorder::loop() {
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

  bool success = submit_logs();
  if (success) {
    this->disable_loop();
  }
}

// void LogsRecorder::set_protocol( const std::string& protocol ) {}

void LogsRecorder::set_endpoint(const std::string& endpoint) { this->endpoint = endpoint + "/v1/logs"; }

void LogsRecorder::add_header(const std::string& header_key, const std::string& header_value) {
  http_request::Header header;
  header.name = header_key;
  header.value = header_value;
  this->headers.push_back(header);
}

void LogsRecorder::add_resource_attribute(const std::string& attr_key, const std::string& attr_value) {
  this->resource_attributes.insert_or_assign(attr_key, attr_value);
}

std::map<std::string, std::string>* LogsRecorder::get_resource_attributes() { return &this->resource_attributes; }

bool LogsRecorder::submit_logs() {
  if (this->logs.empty()) {
    return true;
  }

  ESP_LOGD(TAG, "Publishing OTLP logs");
  if (dropped_logs) {
#ifdef OTLP_DROP_UNSYNCHRONIZED
    ESP_LOGW(TAG, "Dropped %lu log lines due to logs_max_depth or the time not being syncronized being exceeded.",
             (dropped_logs + 1)); /* the +1 is for this message */
#else
    ESP_LOGW(TAG, "Dropped %lu log lines due to logs_max_depth being exceeded.", (dropped_logs + 1)); /* the +1 is for this message */
#endif
  }

  // Create a Logs message
  opentelemetry_proto_logs_v1_LogsData logsdata = opentelemetry_proto_logs_v1_LogsData_init_zero;
  logsdata.resource_logs.arg = this;
  logsdata.resource_logs.funcs.encode = nanopb_encode_resource_logs;

  // Serialize the protobuf message
  std::string buffer;
  pb_ostream_t stream = pb_ostream_from_string(buffer);
  bool status = pb_encode(&stream, opentelemetry_proto_logs_v1_LogsData_fields, &logsdata);
  if (!status) {
    ESP_LOGE(TAG, "Failed to encode logs protobuf message");
    ESP_LOGE(TAG, "error: %s", PB_GET_ERROR(&stream));
    return false;
  }
  size_t message_length = stream.bytes_written;

  auto response = this->http->post(this->endpoint, buffer, this->headers);

  if (response == nullptr) {
    ESP_LOGW(TAG, "Error sending to %s", this->endpoint.c_str());
    return false;
  }

  auto status_code = response->status_code;
  response->end();
  if (!(status_code == 200 || status_code == 204)) {
    ESP_LOGW(TAG, "Error sending to %s: HTTP %d", this->endpoint.c_str(), status_code);
    return false;
  }

  this->logs.clear();
  dropped_logs = 0;
  return true;
}

void LogsRecorder::on_log(uint8_t level, const char* tag, const char* message, size_t message_len) {
  if (level > this->log_level) return;
  uint64_t timestamp = get_otlp_timestamp();
#ifdef OTLP_DROP_UNSYNCHRONIZED
  if (timestamp == 0) {
    dropped_logs += 1;
    return;
  }
#endif
  logs.push_back(std::tuple(timestamp, level, tag, std::string(message)));
  if (logs.size() > this->logs_max_depth) {
    dropped_logs += 1;
    logs.pop_front();
  }
  this->enable_loop();
}

void LogsRecorder::set_logger_max_depth(uint_fast16_t depth) { this->logs_max_depth = depth; }

void LogsRecorder::add_logger_attribute(const std::string& attr_key, const std::string& attr_value) {
  this->logging_attributes.insert_or_assign(attr_key, attr_value);
}

std::map<std::string, std::string>* LogsRecorder::get_logging_attributes() { return &this->logging_attributes; }

std::list<std::tuple<time_t, int, const char*, std::string>>* LogsRecorder::get_logs() { return &this->logs; }

}  // namespace otel
}  // namespace esphome

#endif
