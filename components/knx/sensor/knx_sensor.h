// SPDX-License-Identifier: MIT

// File: knx_sensor.h
// Author: Simon Billemont (Since 2026)

#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

#include "../dpt.h"
#include "../knx_processor.h"

namespace esphome {
namespace knx {

class KnxComponent;
class KnxTelegram;

class KnxSensor : public sensor::Sensor, public Component, public KnxProcessor {
public:
    void setup() override;
    void loop() override;
    void dump_config() override;

    void set_knx( KnxComponent* knx );
    void set_group_address( std::string group_address );
    void set_dpt( KnxDpt dpt );

    void handle_knx_telegram( KnxTelegram* telegram );

protected:
    KnxComponent* knx;
    uint8_t target_main_group;
    uint8_t target_middle_group;
    uint8_t target_sub_group;
    uint16_t target_address;
    KnxDpt dpt;
    KnxDptFormat dpt_format;
};

} //namespace knx
} //namespace esphome