// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <esphome/core/defines.h>

#ifdef USE_SENSOR

#include <cstdint>
#include <list>
#include <map>
#include <pb.h>
#include <string>

#include <esphome/core/entity_base.h>

namespace esphome {
namespace otel {

class MetricsRecorder;
enum MetricsNamingScheme : uint8_t;

typedef bool (*encoding_function)(pb_ostream_t* stream, const pb_field_t* field, void* const* arg);
bool nanopb_encode_dpt(pb_ostream_t* stream, const pb_field_t* field, void* const* arg);

class Metric {
 public:
  Metric(MetricsRecorder* otel, EntityBase* entity, const char* entity_type, MetricsNamingScheme naming_scheme, uint_fast16_t max_samples);

  virtual EntityBase* get_entity() = 0;
  virtual void install_sample_hook() = 0;
  virtual void sample() = 0;

  std::map<const char*, const char*>* get_attributes();
  void add_attribute(const char* attr_key, const char* attr_value);
  void set_name(const char* name);
  const char* get_name();

  const std::list<std::pair<uint64_t, float>>& get_samples() const;
  void clear_samples();

  virtual encoding_function get_nanopb_encoder();

 protected:
  MetricsRecorder* otel;
  const char* name;
  std::string device_class; // TODO this should not be stored here as a copy
  std::map<const char*, const char*> attributes;
  std::list<std::pair<uint64_t, float>> samples;
  uint_fast16_t max_samples = 50;
};

}  // namespace otel
}  // namespace esphome

#endif
