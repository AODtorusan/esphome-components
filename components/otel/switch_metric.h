// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <esphome/core/defines.h>

#ifdef USE_SWITCH

#include <esphome/components/switch/switch.h>

#include "metric.h"

namespace esphome {
namespace otel {

class SwitchMetric : public Metric {
 public:
  SwitchMetric(MetricsRecorder* otel, switch_::Switch* switch_, MetricsNamingScheme naming_scheme, uint_fast16_t max_samples);

  EntityBase* get_entity() override;
  void install_sample_hook() override;
  void sample() override;

  switch_::Switch* get_switch();

 protected:
  switch_::Switch* switch_;
};

}  // namespace otel
}  // namespace esphome

#endif
