// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <esphome/core/defines.h>

#ifdef USE_SENSOR

#include <esphome/components/sensor/sensor.h>

#include "metric.h"

namespace esphome {
namespace otel {

class SensorMetric : public Metric {
 public:
  SensorMetric(MetricsRecorder* otel, sensor::Sensor* sensor, bool name_from_device_class, uint_fast16_t max_samples);

  EntityBase* get_entity() override;
  void install_sample_hook() override;
  void sample() override;

  sensor::Sensor* get_sensor();

  encoding_function get_nanopb_encoder() override;

 protected:
  sensor::Sensor* sensor;
};

}  // namespace otel
}  // namespace esphome

#endif
