// SPDX-License-Identifier: MIT

// File: knx_binary_sensor.h
// Author: Simon Billemont (Since 2026)

#include "esphome/core/log.h"
#include "knx_binary_sensor.h"
#include "../knx_component.h"

namespace esphome {
namespace knx {

static const char *TAG = "knx.binary_sensor";

void KnxBinarySensor::setup() {
    this->knx->add_listen_group_address(this->target_main_group, this->target_middle_group, this->target_sub_group);
    this->knx->add_processor(this->target_address, this);
}

void KnxBinarySensor::loop() {}

void KnxBinarySensor::dump_config() {
    ESP_LOGCONFIG(TAG,
        "KNX binary sensor for group %d/%d/%d using DPT %d.%03d %s",
        this->target_main_group,
        this->target_middle_group,
        this->target_sub_group,
        std::get<0>(dpt_table[this->dpt]),
        std::get<1>(dpt_table[this->dpt]),
        std::get<3>(dpt_table[this->dpt]).c_str()
    );
}

void KnxBinarySensor::set_knx( KnxComponent* knx ) {
    this->knx = knx;
}

void KnxBinarySensor::set_group_address( std::string group_address ) {
    uint8_t result = sscanf( group_address.c_str(), "%d/%d/%d", &this->target_main_group, &this->target_middle_group, &this->target_sub_group );
    assert( result == 3 );
    this->target_address = ((target_main_group << 3) | target_middle_group) << 8 | target_sub_group;
}

void KnxBinarySensor::set_dpt( KnxDpt dpt )  {
    this->dpt = dpt;
    this->dpt_format = std::get<2>(dpt_table[dpt]);
}

void KnxBinarySensor::handle_knx_telegram( KnxTelegram* telegram ) {
    switch (this->dpt_format) {
    case KnxDptFormat::KNX_DPT_FORMAT_B1:
        this->publish_state( (bool) telegram->get_1byte_uint_value() );
        break;
    default:
        ESP_LOGE(TAG, "Saving DPT format %d in binary sensor is not supported for DPT %d.%03d %s", this->dpt_format, std::get<0>(dpt_table[this->dpt]), std::get<1>(dpt_table[this->dpt]), std::get<3>(dpt_table[this->dpt]).c_str());
        break;
    }
}


} //namespace knx
} //namespace esphome