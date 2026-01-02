// SPDX-License-Identifier: MIT

// File: knx_telegram.h
// Author: Daniel Kleine-Albers (Since 2012)
// Modified: Thorsten Gehrig (Since 2014)
// Modified: Michael Werski (Since 2014)
// Modified: Katja Blankenheim (Since 2014)
// Modified: Mag Gyver (Since 2016)
// Modified: Dulgheru Mihaita (Since 2022)
// Modified: Simon Billemont (Since 2026)

#pragma once

#include <string>
#include <stdint.h>

#define MAX_KNX_TELEGRAM_SIZE 23
#define KNX_TELEGRAM_HEADER_SIZE 6

namespace esphome {
namespace knx {

// KNX priorities
enum KnxPriorityType {
  KNX_PRIORITY_SYSTEM = 0b00,
  KNX_PRIORITY_ALARM = 0b10,
  KNX_PRIORITY_HIGH = 0b01,
  KNX_PRIORITY_NORMAL = 0b11
};

// KNX commands / APCI Coding
enum KnxCommandType {
  KNX_COMMAND_READ = 0b0000,
  KNX_COMMAND_WRITE = 0b0010,
  KNX_COMMAND_ANSWER = 0b0001,
  KNX_COMMAND_INDIVIDUAL_ADDR_WRITE = 0b0011,
  KNX_COMMAND_INDIVIDUAL_ADDR_REQUEST = 0b0100,
  KNX_COMMAND_INDIVIDUAL_ADDR_RESPONSE = 0b0101,
  KNX_COMMAND_MASK_VERSION_READ = 0b1100,
  KNX_COMMAND_MASK_VERSION_RESPONSE = 0b1101,
  KNX_COMMAND_RESTART = 0b1110,
  KNX_COMMAND_ESCAPE = 0b1111
};

// Extended (escaped) KNX commands
enum KnxExtendedCommandType {
  KNX_EXT_COMMAND_AUTH_REQUEST = 0b010001,
  KNX_EXT_COMMAND_AUTH_RESPONSE = 0b010010
};

// KNX Transport Layer Communication Type
enum KnxCommunicationType {
  KNX_COMM_UDP = 0b00, // Unnumbered Data Packet
  KNX_COMM_NDP = 0b01, // Numbered Data Packet
  KNX_COMM_UCD = 0b10, // Unnumbered Control Data
  KNX_COMM_NCD = 0b11  // Numbered Control Data
};

// KNX Control Data (for UCD / NCD packets)
enum KnxControlDataType {
  KNX_CONTROLDATA_CONNECT = 0b00,      // UCD
  KNX_CONTROLDATA_DISCONNECT = 0b01,   // UCD
  KNX_CONTROLDATA_POS_CONFIRM = 0b10,  // NCD
  KNX_CONTROLDATA_NEG_CONFIRM = 0b11   // NCD
};

class KnxTelegram {
  public:
    KnxTelegram();

    void clear();
    void set_buffer_byte(uint8_t index, uint8_t content);
    uint8_t get_buffer_byte(uint8_t index);
    void set_payload_length(uint8_t size);
    uint8_t get_payload_length();
    void set_repeated(bool repeat);
    bool is_repeated();
    void set_priority(KnxPriorityType prio);
    KnxPriorityType get_priority();
    void set_source_address(uint8_t area, uint8_t line, uint8_t member);
    uint8_t get_source_area();
    uint8_t get_source_line();
    uint8_t get_source_member();
    void set_target_group_address(const std::string& group_address);
    void set_target_group_address(uint8_t main, uint8_t middle, uint8_t sub);
    void set_target_individual_address(const std::string& group_address);
    void set_target_individual_address(uint8_t area, uint8_t line, uint8_t member);
    bool is_target_group();
    std::string get_target_group();
    uint8_t get_target_main_group();
    uint8_t get_target_middle_group();
    uint8_t get_target_sub_group();
    uint8_t get_target_area();
    uint8_t get_target_line();
    uint8_t get_target_member();
    uint16_t get_target_address();
    void set_routing_counter(uint8_t counter);
    uint8_t get_routing_counter();
    void set_command(KnxCommandType command);
    KnxCommandType get_command();

    void set_first_data_byte(uint8_t data);
    uint8_t get_first_data_byte();
    bool get_bool();

    uint8_t get_4bit_int_value();
    bool get_4bit_direction_value();
    uint8_t get_4bit_steps_value();

    void set_1byte_int_value(uint8_t value);
    int8_t get_1byte_int_value();
    uint8_t get_1byte_uint_value();

    void set_2byte_int_value(uint8_t value);
    int16_t get_2byte_int_value();
    uint16_t get_2byte_uint_value();
    void set_2byte_float_value(float value);
    float get_2byte_float_value();

    void set_3byte_time(uint8_t weekday, uint8_t hour, uint8_t minute, uint8_t second);
    uint8_t get_3byte_weekday_value();
    uint8_t get_3byte_hour_value();
    uint8_t get_3byte_minute_value();
    uint8_t get_3byte_second_value();
    void set_3byte_date(uint8_t day, uint8_t month, uint8_t year);
    uint8_t get_3byte_day_value();
    uint8_t get_3byte_month_value();
    uint8_t get_3byte_year_value();

    int32_t get_4byte_int_value();
    uint32_t get_4byte_uint_value();
    void set_4byte_float_value(float value);
    float get_4byte_float_value();

    void set_14byte_value(const std::string& value);
    std::string get_14byte_value();

    void create_checksum();
    bool verify_checksum();
    uint8_t get_checksum();
    void print();
    uint8_t get_total_length();
    KnxCommunicationType get_communication_type();
    void set_communication_type(KnxCommunicationType);
    uint8_t get_sequence_number();
    void set_sequence_number(uint8_t);
    KnxControlDataType get_control_data();
    void set_control_data(KnxControlDataType);

  private:
    uint8_t buffer[MAX_KNX_TELEGRAM_SIZE];
    uint8_t calculate_checksum();

};

}  // namespace knx
}  // namespace esphome
