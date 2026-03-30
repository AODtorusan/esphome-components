// SPDX-License-Identifier: GPL-3.0-or-later

#include "switch_metric.h"

#ifdef USE_SWITCH

#include <esphome/core/log.h>
#include <opentelemetry/proto/metrics/v1/metrics.pb.h>

#include "metrics_recorder.h"
#include "utils.h"

namespace esphome {
namespace otel {

static const char *const TAG = "OTLP.SwitchMetric";

SwitchMetric::SwitchMetric(MetricsRecorder *otel, switch_::Switch *switch_, bool name_from_device_class, uint_fast16_t max_samples)
    : Metric(otel, switch_, name_from_device_class, max_samples) {
  this->switch_ = switch_;
}

EntityBase *SwitchMetric::get_entity() { return this->switch_; }

void SwitchMetric::install_sample_hook() {
  this->switch_->add_on_state_callback([this](float state) { this->sample(); });
}

void SwitchMetric::sample() {
  float value = this->switch_->state;
  uint64_t timestamp = get_otlp_timestamp();
#ifdef OTLP_DROP_UNSYNCHRONIZED
  if (timestamp == 0) return;
#endif
  ESP_LOGV(TAG, "Recording switch value to OTLP of %s: %f", this->switch_->get_name().c_str(), value);
  this->samples.push_back(std::pair(timestamp, value));
  if (samples.size() > this->max_samples) samples.pop_front();
  this->otel->enable_loop();
}

switch_::Switch *SwitchMetric::get_switch() { return this->switch_; }

}  // namespace otel
}  // namespace esphome

#endif
