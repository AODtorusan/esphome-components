// SPDX-License-Identifier: MIT

// File: knx_sensor.cpp
// Author: Simon Billemont (Since 2026)

#include "esphome/core/log.h"
#include "knx_sensor.h"
#include "../knx_component.h"

namespace esphome {
namespace knx {

static const char *TAG = "knx.sensor";

void KnxSensor::setup() {
    this->knx->add_listen_group_address(this->target_main_group, this->target_middle_group, this->target_sub_group);
    this->knx->add_processor(this->target_address, this);
}

void KnxSensor::loop() {}

void KnxSensor::dump_config() {
    ESP_LOGCONFIG(TAG,
        "KNX sensor for group %d/%d/%d using DPT %d.%03d %s",
        this->target_main_group,
        this->target_middle_group,
        this->target_sub_group,
        std::get<0>(dpt_table[this->dpt]),
        std::get<1>(dpt_table[this->dpt]),
        std::get<3>(dpt_table[this->dpt]).c_str()
    );
}

void KnxSensor::set_knx( KnxComponent* knx ) {
    this->knx = knx;
}

void KnxSensor::set_group_address( std::string group_address ) {
    uint8_t result = sscanf( group_address.c_str(), "%hhu/%hhu/%hhu", &this->target_main_group, &this->target_middle_group, &this->target_sub_group );
    assert( result == 3 );
    this->target_address = ((target_main_group << 3) | target_middle_group) << 8 | target_sub_group;
}

void KnxSensor::set_dpt( KnxDpt dpt )  {
    this->dpt = dpt;
    this->dpt_format = std::get<2>(dpt_table[dpt]);
}

void KnxSensor::handle_knx_telegram( KnxTelegram* telegram ) {
    switch (this->dpt_format) {
    case KnxDptFormat::KNX_DPT_FORMAT_B1:
        this->publish_state( telegram->get_1byte_uint_value() );
        break;
    case KnxDptFormat::KNX_DPT_FORMAT_B2:
        this->publish_state( telegram->get_1byte_uint_value() );
        break;
    case KnxDptFormat::KNX_DPT_FORMAT_B1U3:
        this->publish_state( telegram->get_1byte_uint_value() );
        break;
    case KnxDptFormat::KNX_DPT_FORMAT_U8:
        this->publish_state( telegram->get_1byte_uint_value() );
        break;
    case KnxDptFormat::KNX_DPT_FORMAT_V8:
        this->publish_state( telegram->get_1byte_int_value() );
        break;
    case KnxDptFormat::KNX_DPT_FORMAT_B5N3:
        this->publish_state( telegram->get_1byte_uint_value() );
        break;
    case KnxDptFormat::KNX_DPT_FORMAT_U16:
        this->publish_state( telegram->get_2byte_uint_value() );
        break;
    case KnxDptFormat::KNX_DPT_FORMAT_V16:
        this->publish_state( telegram->get_2byte_int_value() );
        break;
    case KnxDptFormat::KNX_DPT_FORMAT_F16:
        this->publish_state( telegram->get_2byte_float_value() );
        break;
    case KnxDptFormat::KNX_DPT_FORMAT_U32:
        this->publish_state( telegram->get_4byte_uint_value() );
        break;
    case KnxDptFormat::KNX_DPT_FORMAT_V32:
        this->publish_state( telegram->get_4byte_int_value() );
        break;
    case KnxDptFormat::KNX_DPT_FORMAT_F32:
        this->publish_state( telegram->get_4byte_float_value() );
        break;
    default:
        ESP_LOGE(TAG, "Saving DPT format %d in sensor is not supported for DPT %d.%03d %s", this->dpt_format, std::get<0>(dpt_table[this->dpt]), std::get<1>(dpt_table[this->dpt]), std::get<3>(dpt_table[this->dpt]).c_str());
        break;
    }
}


} //namespace knx
} //namespace esphome