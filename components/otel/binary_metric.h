// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <esphome/core/defines.h>

#ifdef USE_BINARY_SENSOR

#include <esphome/components/binary_sensor/binary_sensor.h>

#include "metric.h"

namespace esphome {
namespace otel {

class BinaryMetric : public Metric {
 public:
  BinaryMetric(MetricsRecorder* otel, binary_sensor::BinarySensor* sensor, bool name_from_device_class, uint_fast16_t max_samples);

  EntityBase* get_entity() override;
  void install_sample_hook() override;
  void sample() override;

  binary_sensor::BinarySensor* get_sensor();

  encoding_function get_nanopb_encoder() override;

 protected:
  binary_sensor::BinarySensor* sensor;
};

}  // namespace otel
}  // namespace esphome

#endif
