// SPDX-License-Identifier: GPL-3.0-or-later

#include "binary_sensor_metric.h"

#ifdef USE_BINARY_SENSOR

#include <esphome/core/log.h>
#include <opentelemetry/proto/metrics/v1/metrics.pb.h>

#include "metrics_recorder.h"
#include "utils.h"

namespace esphome {
namespace otel {

static const char* const TAG = "OTLP.BinarySensorMetric";

BinarySensorMetric::BinarySensorMetric(MetricsRecorder* otel, binary_sensor::BinarySensor* sensor, bool name_from_device_class, uint_fast16_t max_samples)
    : Metric(otel, sensor, name_from_device_class, max_samples) {
  this->sensor = sensor;
}

EntityBase* BinarySensorMetric::get_entity() { return this->sensor; }

void BinarySensorMetric::install_sample_hook() {
  this->sensor->add_on_state_callback([this](bool state) { this->sample(); });
}

void BinarySensorMetric::sample() {
  if (!this->sensor->has_state()) return;
  float value = this->sensor->get_state();
  uint64_t timestamp = get_otlp_timestamp();
#ifdef OTLP_DROP_UNSYNCHRONIZED
  if (timestamp == 0) return;
#endif
  ESP_LOGV(TAG, "Recording binary_sensor value to OTLP of %s: %f", this->sensor->get_name().c_str(), value);
  this->samples.push_back(std::pair(timestamp, value));
  if (samples.size() > this->max_samples) samples.pop_front();
  this->otel->enable_loop();
}

binary_sensor::BinarySensor* BinarySensorMetric::get_sensor() { return this->sensor; }

}  // namespace otel
}  // namespace esphome

#endif
