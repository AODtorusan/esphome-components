
#include "utils.h"

#include <chrono>
#include <cstdio>
#include <map>

#include <esphome/core/log.h>
#include <opentelemetry/proto/common/v1/common.pb.h>

namespace esphome {
namespace otel {

static const uint64_t J2000_EPOCH = UINT64_C(946728000) * UINT64_C(1000000000);  // This is 1 Jan 2000 12:00:00 UTC
static const char *TAG = "OTLP.utils";

uint64_t get_otlp_timestamp() {
  auto time = std::chrono::system_clock::now();
  auto duration = time.time_since_epoch();
  uint64_t timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
  // If the time is not an absolute time, set the time to 0 so it can be set at the receiving side
  if (timestamp > J2000_EPOCH) {
    return timestamp;
  } else {
    return 0;
  }
}

bool nanopb_write_to_string(pb_ostream_t *stream, const pb_byte_t *buf, size_t count) {
  std::string *dest = static_cast<std::string *>(stream->state);
  dest->append(reinterpret_cast<const char *>(buf), count);
  return true;
}

pb_ostream_t pb_ostream_from_string(std::string &dest) { return {&nanopb_write_to_string, &dest, SIZE_MAX, 0}; }

bool nanopb_encode_c_string(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
  const char *str = (const char *)(*arg);
  if (!pb_encode_tag_for_field(stream, field)) return false;
  return pb_encode_string(stream, (const uint8_t *)str, strlen(str));
}

bool nanopb_encode_std_string(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
  const std::string *str = (const std::string *)(*arg);
  if (!pb_encode_tag_for_field(stream, field)) return false;
  return pb_encode_string(stream, (const uint8_t *)(str->c_str()), str->length());
}

bool nanopb_encode_attributes(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
  const std::map<const std::string, const std::string> *attrs = (const std::map<const std::string, const std::string> *)(*arg);
  for (auto &[key, value] : *attrs) {
    opentelemetry_proto_common_v1_KeyValue keyvalue = opentelemetry_proto_common_v1_KeyValue_init_zero;
    keyvalue.key.arg = (void *)&key;
    keyvalue.key.funcs.encode = nanopb_encode_std_string;
    keyvalue.has_value = true;
    keyvalue.value.which_value = opentelemetry_proto_common_v1_AnyValue_string_value_tag;
    keyvalue.value.value.string_value.arg = (void *)&value;
    keyvalue.value.value.string_value.funcs.encode = nanopb_encode_std_string;
    // Build the submessage tag
    if (!pb_encode_tag_for_field(stream, field)) {
      ESP_LOGE(TAG, "Attributes pb_encode_tag_for_field error: %s", PB_GET_ERROR(stream));
      return false;
    }
    // Build the submessage payload
    if (!pb_encode_submessage(stream, opentelemetry_proto_common_v1_KeyValue_fields, &keyvalue)) {
      ESP_LOGE(TAG, "Attributes pb_encode_submessage error: %s", PB_GET_ERROR(stream));
      return false;
    }
  }
  return true;
}

bool nanopb_encode_c_str_attributes(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
  const std::map<const char *const, const char *const> *attrs = (const std::map<const char *const, const char *const> *)(*arg);
  for (auto [key, value] : *attrs) {
    opentelemetry_proto_common_v1_KeyValue keyvalue = opentelemetry_proto_common_v1_KeyValue_init_zero;
    keyvalue.key.arg = (void *)key;
    keyvalue.key.funcs.encode = nanopb_encode_c_string;
    keyvalue.has_value = true;
    keyvalue.value.which_value = opentelemetry_proto_common_v1_AnyValue_string_value_tag;
    keyvalue.value.value.string_value.arg = (void *)value;
    keyvalue.value.value.string_value.funcs.encode = nanopb_encode_c_string;
    // Build the submessage tag
    if (!pb_encode_tag_for_field(stream, field)) {
      ESP_LOGE(TAG, "Attributes pb_encode_tag_for_field error: %s", PB_GET_ERROR(stream));
      return false;
    }
    // Build the submessage payload
    if (!pb_encode_submessage(stream, opentelemetry_proto_common_v1_KeyValue_fields, &keyvalue)) {
      ESP_LOGE(TAG, "Attributes pb_encode_submessage error: %s", PB_GET_ERROR(stream));
      return false;
    }
  }
  return true;
}

}  // namespace otel
}  // namespace esphome
