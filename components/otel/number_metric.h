// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <esphome/core/defines.h>

#ifdef USE_NUMBER

#include <esphome/components/number/number.h>

#include "metric.h"

namespace esphome {
namespace otel {

class NumberMetric : public Metric {
 public:
  NumberMetric(MetricsRecorder* otel, number::Number* number, bool name_from_device_class, uint_fast16_t max_samples);

  EntityBase* get_entity() override;
  void install_sample_hook() override;
  void sample() override;

  number::Number* get_number();

 protected:
  number::Number* number;
};

}  // namespace otel
}  // namespace esphome

#endif
