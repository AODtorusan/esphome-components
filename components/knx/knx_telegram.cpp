// SPDX-License-Identifier: MIT

// File: knx_telegram.cpp
// Author: Daniel Kleine-Albers (Since 2012)
// Modified: Thorsten Gehrig (Since 2014)
// Modified: Michael Werski (Since 2014)
// Modified: Katja Blankenheim (Since 2014)
// Modified: Mag Gyver (Since 2016)
// Modified: Rouven Raudzus (Since 2017)
// Modified: Dulgheru Mihaita (Since 2022)
// Modified: Dulgheru Mihaita (Since 2022)
// Modified: Simon Billemont (Since 2026)

#include <algorithm>
#include <math.h>
#include <format>
#include <cassert>

#include "knx_telegram.h"

namespace esphome {
namespace knx {

KnxTelegram::KnxTelegram() {
  clear();
}

void KnxTelegram::clear() {
  for (uint8_t i = 0; i < MAX_KNX_TELEGRAM_SIZE; i++) {
    buffer[i] = 0;
  }

  // Control Field, Normal Priority, No Repeat
  buffer[0] = 0b10111100;

  // Target Group Address, Routing Counter = 6, Length = 1 (= 2 Bytes)
  buffer[5] = 0b11100001;
}

uint8_t KnxTelegram::get_buffer_byte(uint8_t index) {
  return buffer[index];
}

void KnxTelegram::set_buffer_byte(uint8_t index, uint8_t content) {
  buffer[index] = content;
}

bool KnxTelegram::is_repeated() {
  // Parse Repeat Flag
  if (buffer[0] & 0b00100000) {
    return false;
  }
  else {
    return true;
  }
}

void KnxTelegram::set_repeated(bool repeat) {
  if (repeat) {
    buffer[0] = buffer[0] & 0b11011111;
  }
  else {
    buffer[0] = buffer[0] | 0b00100000;
  }
}

void KnxTelegram::set_priority(KnxPriorityType prio) {
  buffer[0] = buffer[0] & 0b11110011;
  buffer[0] = buffer[0] | (prio << 2);
}

KnxPriorityType KnxTelegram::get_priority() {
  // Priority
  return (KnxPriorityType) ((buffer[0] & 0b00001100) >> 2);
}

void KnxTelegram::set_source_address(uint8_t area, uint8_t line, uint8_t member) {
  buffer[1] = (area << 4) | line;	// Source Address
  buffer[2] = member; // Source Address
}

uint8_t KnxTelegram::get_source_area() {
  return (buffer[1] >> 4);
}

uint8_t KnxTelegram::get_source_line() {
  return (buffer[1] & 0b00001111);
}

uint8_t KnxTelegram::get_source_member() {
  return buffer[2];
}

void KnxTelegram::set_target_group_address(const std::string& group_address) {
  uint8_t mainGroup = 0;
  uint8_t middleGroup = 0;
  uint8_t subGroup = 0;
  uint8_t result = sscanf( group_address.c_str(), "%hhu/%hhu/%hhu", &mainGroup, &middleGroup, &subGroup );
  assert( result == 3 );
  return this->set_target_group_address(mainGroup, middleGroup, subGroup);
}

void KnxTelegram::set_target_group_address(uint8_t main, uint8_t middle, uint8_t sub) {
  buffer[3] = (main << 3) | middle;
  buffer[4] = sub;
  buffer[5] = buffer[5] | 0b10000000;
}

void KnxTelegram::set_target_individual_address(const std::string& group_address) {
  uint8_t area = 0;
  uint8_t line = 0;
  uint8_t member = 0;
  uint8_t result = sscanf( group_address.c_str(), "%hhu/%hhu/%hhu", &area, &line, &member );
  assert( result == 3 );
  return this->set_target_individual_address(area, line, member);
}

void KnxTelegram::set_target_individual_address(uint8_t area, uint8_t line, uint8_t member) {
  buffer[3] = (area << 4) | line;
  buffer[4] = member;
  buffer[5] = buffer[5] & 0b01111111;
}

bool KnxTelegram::is_target_group() {
  return buffer[5] & 0b10000000;
}

std::string KnxTelegram::get_target_group(){
  return std::format("{}/{}/{}", this->get_target_main_group(), this->get_target_middle_group(), this->get_target_sub_group());
}

uint8_t KnxTelegram::get_target_main_group() {
  return ((buffer[3] & 0b01111000) >> 3);
}

uint8_t KnxTelegram::get_target_middle_group() {
  return (buffer[3] & 0b00000111);
}

uint8_t KnxTelegram::get_target_sub_group() {
  return buffer[4];
}

uint8_t KnxTelegram::get_target_area() {
  return ((buffer[3] & 0b11110000) >> 4);
}

uint8_t KnxTelegram::get_target_line() {
  return (buffer[3] & 0b00001111);
}

uint8_t KnxTelegram::get_target_member() {
  return buffer[4];
}

uint16_t KnxTelegram::get_target_address() {
  return buffer[3] << 8 | buffer[4];
}


void KnxTelegram::set_routing_counter(uint8_t counter) {
  buffer[5] = buffer[5] & 0b10000000;
  buffer[5] = buffer[5] | (counter << 4);
}

uint8_t KnxTelegram::get_routing_counter() {
  return ((buffer[5] & 0b01110000) >> 4);
}

void KnxTelegram::set_payload_length(uint8_t length) {
  buffer[5] = buffer[5] & 0b11110000;
  buffer[5] = buffer[5] | (length - 1);
}

uint8_t KnxTelegram::get_payload_length() {
  uint8_t length = (buffer[5] & 0b00001111) + 1;
  return length;
}

void KnxTelegram::set_command(KnxCommandType command) {
  buffer[6] = buffer[6] & 0b11111100;
  buffer[7] = buffer[7] & 0b00111111;

  buffer[6] = buffer[6] | (command >> 2); // Command first two bits
  buffer[7] = buffer[7] | (command << 6); // Command last two bits
}

KnxCommandType KnxTelegram::get_command() {
  return (KnxCommandType) (((buffer[6] & 0b00000011) << 2) | ((buffer[7] & 0b11000000) >> 6));
}

void KnxTelegram::set_control_data(KnxControlDataType cd) {
  buffer[6] = buffer[6] & 0b11111100;
  buffer[6] = buffer[6] | cd;
}

KnxControlDataType KnxTelegram::get_control_data() {
  return (KnxControlDataType) (buffer[6] & 0b00000011);
}

KnxCommunicationType KnxTelegram::get_communication_type() {
  return (KnxCommunicationType) ((buffer[6] & 0b11000000) >> 6);
}

void KnxTelegram::set_communication_type(KnxCommunicationType type) {
  buffer[6] = buffer[6] & 0b00111111;
  buffer[6] = buffer[6] | (type << 6);
}

uint8_t KnxTelegram::get_sequence_number() {
  return (buffer[6] & 0b00111100) >> 2;
}

void KnxTelegram::set_sequence_number(uint8_t number) {
  buffer[6] = buffer[6] & 0b11000011;
  buffer[6] = buffer[6] | (number << 2);
}

void KnxTelegram::create_checksum() {
  uint8_t checksumPos = get_payload_length() + KNX_TELEGRAM_HEADER_SIZE;
  buffer[checksumPos] = calculate_checksum();
}

uint8_t KnxTelegram::get_checksum() {
  uint8_t checksumPos = get_payload_length() + KNX_TELEGRAM_HEADER_SIZE;
  return buffer[checksumPos];
}

bool KnxTelegram::verify_checksum() {
  uint8_t calculatedChecksum = calculate_checksum();
  return (get_checksum() == calculatedChecksum);
}

void KnxTelegram::print() {
#if defined(TPUART_DEBUG)
  // serial->print("Repeated: ");
  // serial->println(is_repeated());

  // serial->print("Priority: ");
  // serial->println(get_priority());

  // serial->print("Source: ");
  // serial->print(get_source_area());
  // serial->print(".");
  // serial->print(get_source_line());
  // serial->print(".");
  // serial->println(get_source_member());

  // if (is_target_group()) {
  //   serial->print("Target Group: ");
  //   serial->print(get_target_main_group());
  //   serial->print("/");
  //   serial->print(get_target_middle_group());
  //   serial->print("/");
  //   serial->println(get_target_sub_group());
  // }
  // else {
  //   serial->print("Target Physical: ");
  //   serial->print(get_target_area());
  //   serial->print(".");
  //   serial->print(get_target_line());
  //   serial->print(".");
  //   serial->println(get_target_member());
  // }

  // serial->print("Routing Counter: ");
  // serial->println(get_routing_counter());

  // serial->print("Payload Length: ");
  // serial->println(get_payload_length());

  // serial->print("Command: ");
  // serial->println(get_command());

  // serial->print("First Data Byte: ");
  // serial->println(get_first_data_byte());

  // for (uint8_t i = 2; i < get_payload_length(); i++) {
  //   serial->print("Data Byte ");
  //   serial->print(i);
  //   serial->print(": ");
  //   serial->println(buffer[6 + i], BIN);
  // }


  // if (verify_checksum()) {
  //   serial->println("Checksum matches");
  // }
  // else {
  //   serial->println("Checksum mismatch");
  //   serial->println(get_checksum(), BIN);
  //   serial->println(calculate_checksum(), BIN);
  // }
#endif
}

uint8_t KnxTelegram::calculate_checksum() {
  uint8_t bcc = 0xFF;
  uint8_t size = get_payload_length() + KNX_TELEGRAM_HEADER_SIZE;

  for (uint8_t i = 0; i < size; i++) {
    bcc ^= buffer[i];
  }

  return bcc;
}

uint8_t KnxTelegram::get_total_length() {
  return KNX_TELEGRAM_HEADER_SIZE + get_payload_length() + 1;
}

void KnxTelegram::set_first_data_byte(uint8_t data) {
  buffer[7] = buffer[7] & 0b11000000;
  buffer[7] = buffer[7] | data;
}

uint8_t KnxTelegram::get_first_data_byte() {
  return (buffer[7] & 0b00111111);
}

bool KnxTelegram::get_bool() {
  if (get_payload_length() != 2) {
    // Wrong payload length
    return 0;
  }

  return (get_first_data_byte() & 0b00000001);
}

uint8_t KnxTelegram::get_4bit_int_value() {
  if (get_payload_length() != 2) {
    // Wrong payload length
    return 0;
  }

  return (get_first_data_byte() & 0b00001111);
}

bool KnxTelegram::get_4bit_direction_value() {
  if (get_payload_length() != 2) {
    // Wrong payload length
    return 0;
  }

  return ((get_first_data_byte() & 0b00001000)) >> 3;
}

uint8_t KnxTelegram::get_4bit_steps_value() {
  if (get_payload_length() != 2) {
    // Wrong payload length
    return 0;
  }

  return (get_first_data_byte() & 0b00000111);
}

void KnxTelegram::set_1byte_int_value(uint8_t value) {
  set_payload_length(3);
  buffer[8] = value;
}

int8_t KnxTelegram::get_1byte_int_value() {
  if (get_payload_length() != 3) {
    // Wrong payload length
    return 0;
  }

  return (buffer[8]);
}

uint8_t KnxTelegram::get_1byte_uint_value() {
  if (get_payload_length() != 3) {
    // Wrong payload length
    return 0;
  }

  return (buffer[8]);
}

void KnxTelegram::set_2byte_int_value(uint8_t value) {
  set_payload_length(4);

  buffer[8] = uint8_t(value >> 8);
  buffer[9] = uint8_t(value & 0x00FF);
}

int16_t KnxTelegram::get_2byte_int_value() {
  return (int16_t) this->get_2byte_uint_value();
}

uint16_t KnxTelegram::get_2byte_uint_value() {
  if (get_payload_length() != 4) {
    // Wrong payload length
    return 0;
  }
  uint16_t value = uint16_t(buffer[8] << 8) + uint16_t(buffer[9]);

  return (value);
}

void KnxTelegram::set_2byte_float_value(float value) {
  set_payload_length(4);

  float v = value * 100.0f;
  int exponent = 0;
  for (; v < -2048.0f; v /= 2) exponent++;
  for (; v > 2047.0f; v /= 2) exponent++;
  long m = (int)round(v) & 0x7FF;
  short msb = (short) (exponent << 3 | m >> 8);
  if (value < 0.0f) msb |= 0x80;
  buffer[8] = msb;
  buffer[9] = (uint8_t) m;
}

float KnxTelegram::get_2byte_float_value() {
  if (get_payload_length() != 4) {
    // Wrong payload length
    return 0;
  }

  int exponent = (buffer[8] & 0b01111000) >> 3;
  int mantissa = ((buffer[8] & 0b00000111) << 8) | (buffer[9]);

  if (buffer[8] & 0b10000000) {
    return ((-2048 + mantissa) * 0.01) * pow(2.0, exponent);
  }

  return (mantissa * 0.01) * pow(2.0, exponent);
}

void KnxTelegram::set_3byte_time(uint8_t weekday, uint8_t hour, uint8_t minute, uint8_t second) {
  set_payload_length(5);

  // Move the weekday by 5 bits to the left
  weekday = weekday << 5;

  // Buffer [8] bit 5-7 for weekday, bit 0-4 for hour
  buffer[8] = (weekday & 0b11100000) + (hour & 0b00011111);

  // Buffer [9] bit 6-7 empty, bit 0-5 for minutes
  buffer[9] =  minute & 0b00111111;

  // Buffer [10] bit 6-7 empty, bit 0-5 for seconds
  buffer[10] = second & 0b00111111;
}

uint8_t KnxTelegram::get_3byte_weekday_value() {
  if (get_payload_length() != 5) {
    // Wrong payload length
    return 0;
  }
  return (buffer[8] & 0b11100000) >> 5;
}

uint8_t KnxTelegram::get_3byte_hour_value() {
  if (get_payload_length() != 5) {
    // Wrong payload length
    return 0;
  }
  return (buffer[8] & 0b00011111);
}

uint8_t KnxTelegram::get_3byte_minute_value() {
  if (get_payload_length() != 5) {
    // Wrong payload length
    return 0;
  }
  return (buffer[9] & 0b00111111);
}

uint8_t KnxTelegram::get_3byte_second_value() {
  if (get_payload_length() != 5) {
    // Wrong payload length
    return 0;
  }
  return (buffer[10] & 0b00111111);
}

void KnxTelegram::set_3byte_date(uint8_t day, uint8_t month, uint8_t year) {
  set_payload_length(5);

  // Buffer [8] bit 5-7 empty, bit 0-4 for month days
  buffer[8] = day & 0b00011111;

  // Buffer [9] bit 4-7 empty, bit 0-3 for months
  buffer[9] =  month & 0b00001111;

  // Buffer [10] fill with year
  buffer[10] = year;
}

uint8_t KnxTelegram::get_3byte_day_value() {
  if (get_payload_length() != 5) {
    // Wrong payload length
    return 0;
  }
  return (buffer[8] & 0b00011111);
}

uint8_t KnxTelegram::get_3byte_month_value() {
  if (get_payload_length() != 5) {
    // Wrong payload length
    return 0;
  }
  return (buffer[9] & 0b00001111);
}

uint8_t KnxTelegram::get_3byte_year_value() {
  if (get_payload_length() != 5) {
    // Wrong payload length
    return 0;
  }
  return (buffer[10]);
}

int32_t KnxTelegram::get_4byte_int_value() {
  return (int32_t) this->get_4byte_uint_value();
}

uint32_t KnxTelegram::get_4byte_uint_value() {
  if (get_payload_length() != 6) {
    // Wrong payload length
    return 0;
  }
  uint32_t value = uint32_t(buffer[8] << 24) + uint32_t(buffer[9] << 16) + uint32_t(buffer[9] << 8) + uint32_t(buffer[9] << 0);
  return value;
}

void KnxTelegram::set_4byte_float_value(float value) {
  set_payload_length(6);

  uint8_t b[4];
  float *f = (float*)(void*) & (b[0]);
  *f = value;

  buffer[8 + 3] = b[0];
  buffer[8 + 2] = b[1];
  buffer[8 + 1] = b[2];
  buffer[8 + 0] = b[3];
}

float KnxTelegram::get_4byte_float_value() {
  if (get_payload_length() != 6) {
    // Wrong payload length
    return 0;
  }
  uint8_t b[4];
  b[0] = buffer[8 + 3];
  b[1] = buffer[8 + 2];
  b[2] = buffer[8 + 1];
  b[3] = buffer[8 + 0];
  float *f = (float*)(void*) & (b[0]);
  float  r = *f;
  return r;
}

void KnxTelegram::set_14byte_value(const std::string& value) {
  // Define
  char _load[15];

  // Empty/Initialize with space
  for (uint8_t i = 0; i < 14; ++i)
  {
    _load[i] = 0;
  }
  set_payload_length(16);
  std::size_t idx = 0;
  for ( std::size_t i = 0; i < std::min(value.size(), (size_t) 14); i++ ) {
    buffer[8 + idx] = value[i];
    idx++;
  }
  for (;idx < 15; idx++)
    buffer[8 + idx] = 0;
}

std::string KnxTelegram::get_14byte_value() {
  if (get_payload_length() != 16) {
    // Wrong payload length
    return "";
  }
  std::string _load;
  _load += buffer[8 + 0];
  _load += buffer[8 + 1];
  _load += buffer[8 + 2];
  _load += buffer[8 + 3];
  _load += buffer[8 + 4];
  _load += buffer[8 + 5];
  _load += buffer[8 + 6];
  _load += buffer[8 + 7];
  _load += buffer[8 + 8];
  _load += buffer[8 + 9];
  _load += buffer[8 + 10];
  _load += buffer[8 + 11];
  _load += buffer[8 + 12];
  _load += buffer[8 + 13];
  return _load;
}

}  // namespace knx
}  // namespace esphome
