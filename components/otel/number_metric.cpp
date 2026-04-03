// SPDX-License-Identifier: GPL-3.0-or-later

#include "number_metric.h"

#ifdef USE_NUMBER

#include <esphome/core/log.h>
#include <opentelemetry/proto/metrics/v1/metrics.pb.h>

#include "metrics_recorder.h"
#include "utils.h"

namespace esphome {
namespace otel {

static const char *const TAG = "OTLP.NumberMetric";

NumberMetric::NumberMetric(MetricsRecorder *otel, number::Number *number, MetricsNamingScheme naming_scheme, uint_fast16_t max_samples)
    : Metric(otel, number, "number", naming_scheme, max_samples) {
  this->number = number;
}

EntityBase *NumberMetric::get_entity() { return this->number; }

void NumberMetric::install_sample_hook() {
  this->number->add_on_state_callback([this](float state) { this->sample(); });
}

void NumberMetric::sample() {
  float value = this->number->state;
  uint64_t timestamp = get_otlp_timestamp();
#ifdef OTLP_DROP_UNSYNCHRONIZED
  if (timestamp == 0) return;
#endif
  ESP_LOGV(TAG, "Recording number value to OTLP of %s: %f", this->number->get_name().c_str(), value);
  this->samples.push_back(std::pair(timestamp, value));
  if (samples.size() > this->max_samples) samples.pop_front();
  this->otel->enable_loop();
}

number::Number *NumberMetric::get_number() { return this->number; }

}  // namespace otel
}  // namespace esphome

#endif
