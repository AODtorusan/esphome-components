
#pragma once

#include <cstdio>
#include <stdint.h>
#include <string>

#include <pb_encode.h>

namespace esphome {
namespace otel {

uint64_t get_otlp_timestamp();

bool nanopb_write_to_string(pb_ostream_t *stream, const pb_byte_t *buf, size_t count);
pb_ostream_t pb_ostream_from_string(std::string &dest);

bool nanopb_encode_c_string(pb_ostream_t *stream, const pb_field_t *field, void *const *arg);
bool nanopb_encode_std_string(pb_ostream_t *stream, const pb_field_t *field, void *const *arg);
bool nanopb_encode_strref(pb_ostream_t *stream, const pb_field_t *field, void *const *arg);

bool nanopb_encode_attributes(pb_ostream_t *stream, const pb_field_t *field, void *const *arg);
bool nanopb_encode_c_str_attributes(pb_ostream_t *stream, const pb_field_t *field, void *const *arg);
bool nanopb_encode_strref_attributes(pb_ostream_t *stream, const pb_field_t *field, void *const *arg);

}  // namespace otel
}  // namespace esphome
