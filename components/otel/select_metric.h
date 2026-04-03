// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <esphome/core/defines.h>

#ifdef USE_SELECT

#include <esphome/components/select/select.h>

#include "metric.h"

namespace esphome {
namespace otel {

class SelectMetric : public Metric {
 public:
  SelectMetric(MetricsRecorder* otel, select::Select* sensor, MetricsNamingScheme naming_scheme, uint_fast16_t max_samples);

  EntityBase* get_entity() override;
  void install_sample_hook() override;
  void sample() override;

  select::Select* get_select();

  encoding_function get_nanopb_encoder() override;

 protected:
  select::Select* select;
};

}  // namespace otel
}  // namespace esphome

#endif
