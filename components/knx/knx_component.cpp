// SPDX-License-Identifier: MIT

// File: knx_component.cpp
// Author: Dulgheru Mihaita (Since 2022)
// Modified: Simon Billemont (Since 2026)

#include "knx_component.h"
#include "esphome/core/util.h"
#include "esphome/core/log.h"
#include "esphome/components/uart/uart_component.h"

namespace esphome {
namespace knx {

  static const char *const TAG = "knx";

  int buffer[MAX_KNX_TELEGRAM_SIZE];
  void KnxComponent::loop() {
    KnxComponentserial_eventType eType = this->serial_event();
    //Evaluation of the received telegram -> only KNX telegrams are accepted
    if (eType == KNX_TELEGRAM) {
      KnxTelegram* telegram = this->get_received_telegram();
      ESP_LOGD(TAG, "Received event for group %s.", telegram->get_target_group().c_str());

      uint16_t telegram_target_address = telegram->get_target_address();
      for (auto [target_address, processor] : this->processors)
        if (telegram_target_address == target_address)
          processor->handle_knx_telegram( telegram );

      if (this->lambda_writer_.has_value())  // insert Labda function if available
        (*this->lambda_writer_)(*this);
    }
  }

  void KnxComponent::setup() {
    uint8_t result = sscanf( this->use_address_.c_str(), "%hhu.%hhu.%hhu", &this->_source_area, &this->_source_line, &this->_source_member );
    assert(result == 3);
    this->_tg = new KnxTelegram();
    this->_tg_ptp = new KnxTelegram();
    this->_listen_to_broadcasts = false;

    this->uart_reset();
  }

  void KnxComponent::dump_config(){
    ESP_LOGCONFIG(TAG, " Knx use_address: %s", this->use_address_.c_str());
    for(int i = 0 ; i < this->_listen_group_address_count; i++){
      ESP_LOGCONFIG(TAG, " Knx is listening for group address: %d/%d/%d ",
        this->_listen_group_addresses[i][0], this->_listen_group_addresses[i][1], this->_listen_group_addresses[i][2]
      );
    }
  }

  void KnxComponent::set_use_address(const std::string &use_address) { this->use_address_ = use_address; }

  void KnxComponent::set_serial_timeout(const uint32_t &serial_timeout) {
    this->serial_timeout_ = serial_timeout;
  }
  /* ============== ADAPTED ======================= */

  void KnxComponent::set_listen_to_broadcasts(bool listen) {
    this->_listen_to_broadcasts = listen;
  }

  void KnxComponent::uart_reset() {
    uint8_t sendByte = 0x01;
    this->write(sendByte);
  }

  void KnxComponent::uart_state_request() {
    uint8_t sendByte = 0x02;
    this->write(sendByte);
  }

  void KnxComponent::set_individual_address(int area, int line, int member) {
    this->_source_area = area;
    this->_source_line = line;
    this->_source_member = member;
  }

  KnxComponentserial_eventType KnxComponent::serial_event() {
    while (this->available() > 0) {
      this->check_errors();
      int incomingByte = this->peek();
      print_byte(incomingByte);

      if (this->is_knx_control_byte(incomingByte)) {
        ESP_LOGD(TAG, "We have KNX CONTROL BYTE");
        bool interested = this->read_knx_telegram();
        if (interested) {
          ESP_LOGD(TAG, "Event KNX_TELEGRAM");
          return KNX_TELEGRAM;
        }
        else {
          ESP_LOGD(TAG, "Event IRRELEVANT_KNX_TELEGRAM");
          return IRRELEVANT_KNX_TELEGRAM;
        }
      }
      else if (incomingByte == TPUART_RESET_INDICATION_BYTE) {
        this->serial_read();
        ESP_LOGD(TAG, "Event TPUART_RESET_INDICATION");
        return TPUART_RESET_INDICATION;
      }
      else {
        this->serial_read();
        ESP_LOGV(TAG, "UNKNOWN");
        return UNKNOWN;
      }
    }
    return UNKNOWN;
  }


  bool KnxComponent::is_knx_control_byte(int b) {
    return ( (b | 0b00101100) == 0b10111100 ); // Ignore repeat flag and priority flag
  }

  void KnxComponent::check_errors() {
    this->check_uart_settings(19200, 1, esphome::uart::UARTParityOptions::UART_CONFIG_PARITY_EVEN, 8);
  }

  void KnxComponent::print_byte(int incomingByte) {
    ESP_LOGV(TAG,"Incoming Byte: %i (0x%x)", incomingByte, incomingByte);
  }

  bool KnxComponent::read_knx_telegram() {
    // Receive header
    for (int i = 0; i < 6; i++) {
      this->_tg->set_buffer_byte(i, this->serial_read());
    }
    ESP_LOGV(TAG,"Payload Length: %d", this->_tg->get_payload_length());

    int bufpos = 6;
    for (int i = 0; i < this->_tg->get_payload_length(); i++) {
      _tg->set_buffer_byte(bufpos, this->serial_read());
      bufpos++;
    }

    // Checksum
    this->_tg->set_buffer_byte(bufpos, this->serial_read());

    // Verify if we are interested in this message - GroupAddress
    bool interested = this->_tg->is_target_group() && this->is_listening_to_group_address(this->_tg->get_target_main_group(), this->_tg->get_target_middle_group(), this->_tg->get_target_sub_group());

    // Physical address
    interested = interested || ((!this->_tg->is_target_group()) && this->_tg->get_target_area() == _source_area && this->_tg->get_target_line() == _source_line && this->_tg->get_target_member() == _source_member);

    // Broadcast (Programming Mode)
    interested = interested || (this->_listen_to_broadcasts && this->_tg->is_target_group() && this->_tg->get_target_main_group() == 0 && this->_tg->get_target_middle_group() == 0 && this->_tg->get_target_sub_group() == 0);

    if (interested) {
      this->send_ack();
    }
    else {
      this->send_not_addressed();
    }

    if (this->_tg->get_communication_type() == KNX_COMM_UCD) {
      ESP_LOGV(TAG,"UCD Telegram received");
    }
    else if (_tg->get_communication_type() == KNX_COMM_NCD) {
      ESP_LOGV(TAG,"NCD Telegram %d received", this->_tg->get_sequence_number());

      if (interested) {
        this->send_ncd_pos_confirm(this->_tg->get_sequence_number(), this->_tg->get_source_area(), this->_tg->get_source_line(), this->_tg->get_source_member());
      }
    }

    // Returns if we are interested in this diagram
    return interested;
  }

  KnxTelegram* KnxComponent::get_received_telegram() {
    return this->_tg;
  }

  // Command Write

  bool KnxComponent::group_write_bool(const std::string& address, bool value) {
    int valueAsInt = 0;
    if (value) {
      valueAsInt = 0b00000001;
    }

    this->create_knx_message_frame(2, KNX_COMMAND_WRITE, address, valueAsInt);
    return this->send_message();
  }

  bool KnxComponent::group_write_4bit_int(const std::string& address, int value) {
    int out_value = 0;
    if (value) {
      out_value = value & 0b00001111;
    }

    this->create_knx_message_frame(2, KNX_COMMAND_WRITE, address, out_value);
    return this->send_message();
  }

  bool KnxComponent::group_write_4Bit_dim(const std::string& address, bool direction, uint8_t steps) {
    int value = 0;
    if (direction || steps) {
      value = (direction << 3) + (steps & 0b00000111);
    }

    this->create_knx_message_frame(2, KNX_COMMAND_WRITE, address, value);
    return this->send_message();
  }

  bool KnxComponent::group_write_1byte_int(const std::string& address, int value) {
    this->create_knx_message_frame(2, KNX_COMMAND_WRITE, address, 0);
    this->_tg->set_1byte_int_value(value);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::group_write_2byte_int(const std::string& address, int value) {
    this->create_knx_message_frame(2, KNX_COMMAND_WRITE, address, 0);
    this->_tg->set_2byte_int_value(value);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::group_write_2byte_float(const std::string& address, float value) {
    this->create_knx_message_frame(2, KNX_COMMAND_WRITE, address, 0);
    this->_tg->set_2byte_float_value(value);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::group_write_3byte_time(const std::string& address, int weekday, int hour, int minute, int second) {
    this->create_knx_message_frame(2, KNX_COMMAND_WRITE, address, 0);
    this->_tg->set_3byte_time(weekday, hour, minute, second);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::group_write_3byte_date(const std::string& address, int day, int month, int year) {
    this->create_knx_message_frame(2, KNX_COMMAND_WRITE, address, 0);
    this->_tg->set_3byte_date(day, month, year);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::group_write_4byte_float(const std::string& address, float value) {
    this->create_knx_message_frame(2, KNX_COMMAND_WRITE, address, 0);
    this->_tg->set_4byte_float_value(value);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::group_write_14byte_text(const std::string& address, const std::string& value) {
    this->create_knx_message_frame(2, KNX_COMMAND_WRITE, address, 0);
    this->_tg->set_14byte_value(value);
    this->_tg->create_checksum();
    return this->send_message();
  }

  // Command Answer

  bool KnxComponent::group_answer_bool(const std::string& address, bool value) {
    int valueAsInt = 0;
    if (value) {
      valueAsInt = 0b00000001;
    }

    create_knx_message_frame(2, KNX_COMMAND_ANSWER, address, valueAsInt);
    return this->send_message();
  }

  /*
    bool KnxComponent::groupAnswerBitInt(const std::string& address, int value) {
    int out_value = 0;
    if (value) {
      out_value = value & 0b00001111;
    }

    this->create_knx_message_frame(2, KNX_COMMAND_ANSWER, address, out_value);
    return this->send_message();
    }

    bool KnxComponent::group_answer_4bit_dim(const std::string& address, bool direction, uint8_t steps) {
    int value = 0;
    if (direction || steps) {
      value = (direction << 3) + (steps & 0b00000111);
    }

    this->create_knx_message_frame(2, KNX_COMMAND_ANSWER, address, value);
    return this->send_message();
    }
  */

  bool KnxComponent::group_answer_1byte_int(const std::string& address, int value) {
    this->create_knx_message_frame(2, KNX_COMMAND_ANSWER, address, 0);
    this->_tg->set_1byte_int_value(value);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::group_answer_2byte_int(const std::string& address, int value) {
    this->create_knx_message_frame(2, KNX_COMMAND_ANSWER, address, 0);
    this->_tg->set_2byte_int_value(value);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::group_answer_2byte_float(const std::string& address, float value) {
    this->create_knx_message_frame(2, KNX_COMMAND_ANSWER, address, 0);
    this->_tg->set_2byte_float_value(value);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::group_answer_3byte_time(const std::string& address, int weekday, int hour, int minute, int second) {
    this->create_knx_message_frame(2, KNX_COMMAND_ANSWER, address, 0);
    this->_tg->set_3byte_time(weekday, hour, minute, second);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::group_answer_3byte_date(const std::string& address, int day, int month, int year) {
    this->create_knx_message_frame(2, KNX_COMMAND_ANSWER, address, 0);
    this->_tg->set_3byte_date(day, month, year);
    this->_tg->create_checksum();
    return this->send_message();
  }
  bool KnxComponent::group_answer_4byte_float(const std::string& address, float value) {
    this->create_knx_message_frame(2, KNX_COMMAND_ANSWER, address, 0);
    this->_tg->set_4byte_float_value(value);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::group_answer_14byte_text(const std::string& address, const std::string& value) {
    this->create_knx_message_frame(2, KNX_COMMAND_ANSWER, address, 0);
    this->_tg->set_14byte_value(value);
    this->_tg->create_checksum();
    return this->send_message();
  }

  // Command Read

  bool KnxComponent::group_read(const std::string& address) {
    this->create_knx_message_frame(2, KNX_COMMAND_READ, address, 0);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::individual_answer_address() {
    this->create_knx_message_frame(2, KNX_COMMAND_INDIVIDUAL_ADDR_RESPONSE, "0/0/0", 0);
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::individual_answer_mask_version(int area, int line, int member) {
    std::string address;
    address += area;
    address += "/";
    address += line;
    address += "/";
    address += member;
    this->create_knx_message_frame_individual(4, KNX_COMMAND_MASK_VERSION_RESPONSE, address, 0);
    this->_tg->set_communication_type(KNX_COMM_NDP);
    this->_tg->set_buffer_byte(8, 0x07); // Mask version part 1 for BIM M 112
    this->_tg->set_buffer_byte(9, 0x01); // Mask version part 2 for BIM M 112
    this->_tg->create_checksum();
    return this->send_message();
  }

  bool KnxComponent::individual_answer_auth(int accessLevel, int sequenceNo, int area, int line, int member) {
    std::string address;
    address += area;
    address += "/";
    address += line;
    address += "/";
    address += member;
    this->create_knx_message_frame_individual(3, KNX_COMMAND_ESCAPE, address, KNX_EXT_COMMAND_AUTH_RESPONSE);
    this->_tg->set_communication_type(KNX_COMM_NDP);
    this->_tg->set_sequence_number(sequenceNo);
    this->_tg->set_buffer_byte(8, accessLevel);
    this->_tg->create_checksum();
    return this->send_message();
  }

  void KnxComponent::create_knx_message_frame(int payloadlength, KnxCommandType command, const std::string& address, int firstDataByte) {
    this->_tg->clear();
    this->_tg->set_source_address(_source_area, _source_line, _source_member);
    this->_tg->set_target_group_address(address);
    this->_tg->set_first_data_byte(firstDataByte);
    this->_tg->set_command(command);
    this->_tg->set_payload_length(payloadlength);
    this->_tg->create_checksum();
  }

  void KnxComponent::create_knx_message_frame_individual(int payloadlength, KnxCommandType command, const std::string& address, int firstDataByte) {
    this->_tg->clear();
    this->_tg->set_source_address(_source_area, _source_line, _source_member);
    this->_tg->set_target_individual_address(address);
    this->_tg->set_first_data_byte(firstDataByte);
    this->_tg->set_command(command);
    this->_tg->set_payload_length(payloadlength);
    this->_tg->create_checksum();
  }

  bool KnxComponent::send_ncd_pos_confirm(int sequenceNo, int area, int line, int member) {
    this->_tg_ptp->clear();
    this->_tg_ptp->set_source_address(_source_area, _source_line, _source_member);
    this->_tg_ptp->set_target_individual_address(area, line, member);
    this->_tg_ptp->set_sequence_number(sequenceNo);
    this->_tg_ptp->set_communication_type(KNX_COMM_NCD);
    this->_tg_ptp->set_control_data(KNX_CONTROLDATA_POS_CONFIRM);
    this->_tg_ptp->set_payload_length(1);
    this->_tg_ptp->create_checksum();


    int messageSize = this->_tg_ptp->get_total_length();

    uint8_t sendbuf[2];
    for (int i = 0; i < messageSize; i++) {
      if (i == (messageSize - 1)) {
        sendbuf[0] = TPUART_DATA_END;
      }
      else {
        sendbuf[0] = TPUART_DATA_START_CONTINUE;
      }

      sendbuf[0] |= i;
      sendbuf[1] = this->_tg_ptp->get_buffer_byte(i);

      this->write_array(sendbuf, 2);
    }


    int confirmation;
    while (true) {
      confirmation = serial_read();
      if (confirmation == 0b10001011) {
        return true; // Sent successfully
      }
      else if (confirmation == 0b00001011) {
        return false;
      }
      else if (confirmation == -1) {
        // Read timeout
        return false;
      }
    }

    return false;
  }

  bool KnxComponent::send_message() {
    int messageSize = this->_tg->get_total_length();

    uint8_t sendbuf[2];
    for (int i = 0; i < messageSize; i++) {
      if (i == (messageSize - 1)) {
        sendbuf[0] = TPUART_DATA_END;
      }
      else {
        sendbuf[0] = TPUART_DATA_START_CONTINUE;
      }

      sendbuf[0] |= i;
      sendbuf[1] = this->_tg->get_buffer_byte(i);

      this->write_array(sendbuf, 2);
    }


    int confirmation;
    while (true) {
      confirmation = this->serial_read();
      if (confirmation == 0b10001011) {
        delay (SERIAL_WRITE_DELAY_MS);
        return true; // Sent successfully
      }
      else if (confirmation == 0b00001011) {
        delay (SERIAL_WRITE_DELAY_MS);
        return false;
      }
      else if (confirmation == -1) {
        // Read timeout
        ESP_LOGD(TAG, "Serial read timeout !");
        delay (SERIAL_WRITE_DELAY_MS);
        return false;
      }
    }

    return false;
  }

  void KnxComponent::send_ack() {
    uint8_t sendByte = 0b00010001;
    this->write(sendByte);
    delay(SERIAL_WRITE_DELAY_MS);
  }

  void KnxComponent::send_not_addressed() {
    uint8_t sendByte = 0b00010000;
    this->write(sendByte);
    delay(SERIAL_WRITE_DELAY_MS);
  }

  int KnxComponent::serial_read() {
    unsigned long startTime = millis();
    bool isSerialAvailable = true;
    while (! ( this->available() > 0)) {
      delay(1);
      if(millis() - startTime > this->serial_timeout_){
        ESP_LOGW(TAG, "Serial timeout reached (%d ms) ! Unable to read from serial ! Aborting.", this->serial_timeout_);
        isSerialAvailable = false;
        break;
      }
    }
    int inByte = -1;
    if (isSerialAvailable){
      inByte = this->read();
      this->check_errors();
      this->print_byte(inByte);
    }

    return inByte;
  }

  void KnxComponent::add_listen_group_address(const std::string& group_address) {
    uint8_t mainGroup = 0;
    uint8_t middleGroup = 0;
    uint8_t subGroup = 0;
    uint8_t result = sscanf( group_address.c_str(), "%hhu/%hhu/%hhu", &mainGroup, &middleGroup, &subGroup );
    assert( result == 3 );
    this->add_listen_group_address(mainGroup, middleGroup, subGroup);
  }

  void KnxComponent::add_listen_group_address(uint8_t main, uint8_t middle, uint8_t sub) {
    if (_listen_group_address_count >= MAX_LISTEN_GROUP_ADDRESSES) {
      ESP_LOGW(TAG, "Already listening to MAX_LISTEN_GROUP_ADDRESSES, cannot listen to another.");
      return;
    }
    this->_listen_group_addresses[this->_listen_group_address_count][0] = main;
    this->_listen_group_addresses[this->_listen_group_address_count][1] = middle;
    this->_listen_group_addresses[this->_listen_group_address_count][2] = sub;
    this->_listen_group_address_count++;
  }

  bool KnxComponent::is_listening_to_group_address(int main, int middle, int sub) {
    for (int i = 0; i < this->_listen_group_address_count; i++) {
      if ( (_listen_group_addresses[i][0] == main) && (_listen_group_addresses[i][1] == middle) && (_listen_group_addresses[i][2] == sub)) {
        return true;
      }
    }
    return false;
  }

  void KnxComponent::add_processor(uint16_t address, KnxProcessor* processor) {
    this->processors.push_back( std::tuple(address, processor) );
  }

}  // namespace knx
}  // namespace esphome
