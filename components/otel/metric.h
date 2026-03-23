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

typedef bool (*encoding_function)(pb_ostream_t* stream, const pb_field_t* field, void* const* arg);
bool nanopb_encode_dpt(pb_ostream_t* stream, const pb_field_t* field, void* const* arg);

class Metric {
 public:
  Metric(MetricsRecorder* otel, uint_fast16_t max_samples);

  virtual EntityBase* get_entity() = 0;
  virtual void install_sample_hook() = 0;
  virtual void sample() = 0;

  const std::map<std::string, std::string>& get_attributes() const;
  void add_attribute(const std::string& attr_key, const std::string& attr_value);

  void set_name(std::string name);
  std::string* get_name();

  const std::list<std::pair<uint64_t, float>>& get_samples() const;
  void clear_samples();

  virtual encoding_function get_nanopb_encoder() = 0;

 protected:
  MetricsRecorder* otel;
  std::string name = "unknown";
  std::map<std::string, std::string> attributes;
  std::list<std::pair<uint64_t, float>> samples;
  uint_fast16_t max_samples = 50;
};

}  // namespace otel
}  // namespace esphome

#endif
