// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <esphome/core/defines.h>

#ifdef USE_TEXT_SENSOR

#include <esphome/components/text_sensor/text_sensor.h>

#include "metric.h"

namespace esphome {
namespace otel {

class TextSensorMetric : public Metric {
 public:
  TextSensorMetric(MetricsRecorder* otel, text_sensor::TextSensor* sensor, MetricsNamingScheme naming_scheme, uint_fast16_t max_samples);

  EntityBase* get_entity() override;
  void install_sample_hook() override;
  void sample() override;

  text_sensor::TextSensor* get_sensor();
  std::list<std::pair<uint64_t, std::string>>& get_values();

  encoding_function get_nanopb_encoder() override;

 protected:
  text_sensor::TextSensor* sensor;
  std::list<std::pair<uint64_t, std::string>> values;
};

}  // namespace otel
}  // namespace esphome

#endif
