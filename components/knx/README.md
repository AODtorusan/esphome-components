# knx - Component for esphome

<a href="https://opensource.org"><img height="150" align="left" src="https://opensource.org/files/OSIApprovedCropped.png" alt="Open Source Initiative Approved License logo"></a>
[![License - MIT License](https://img.shields.io/badge/License-MIT-9400d3.svg)](https://spdx.org/licenses/MIT.html)

This component is written for ESPHome to enable communication via KNX TP(Twisted Pair).

Open Source software: [MIT License](https://spdx.org/licenses/MIT.html)

## &nbsp;

This is based on [custom-esphome](https://github.com/fxmike08/custom-esphome) and was updated and extended for better esphome integratation. (The work of fxmike08 is based on [KnxTpUart](https://github.com/majuss/KnxTpUart) )

### Knx component configuration:


*  **id (Required** , ID): Specifies the ID used for the KNX component.
*  **uart_id (Optional**, ID): Specifies the ID of the UART hub.
*  **use_address (Required**, string): Defines the KNX device address. The format is group.subgroup.address (e.g., 10.22.10).
*  **listen_group_address (Optional**, Array[string]): An array of addresses that the component will listen to (sensors will automatically be added).
*  **serial_timeout** (Optional, int): Sets the serial read timeout in milliseconds. The default is 1000 ms.
*  **lambda (Optional**): Lambda to execute on each received group address KNX event. The KNX event will have one of the addresses specified in the `listen_group_address` entries.


Usage example :
```yaml
  external_components:
    - source:
        type: git
        ref: esphome-latest
        url: https://github.com/AODtorusan/esphome-components
      components: [ knx ]
      refresh: 0s
...
  uart:
    - id: uart_bus
      tx_pin: 1 # carefull with strapping pins
      rx_pin: 3 # carefull with strapping pins
      baud_rate: 19200
      parity: even
...
  knx:
    id: knx
    uart_id: uart_bus
    use_address: 10.10.1
    listen_group_address: ["0/0/3", "0/0/1"]
    lambda: |-
      KnxTelegram* telegram = knx.get_received_telegram();
      ESP_LOGD("KNX", "group: %s",telegram->get_target_group());
      if(telegram->get_target_group() == "0/0/3" ){
        if(telegram->get_bool()){
           ESP_LOGD("KNX", "KNX ON");
        } else {
           ESP_LOGD("KNX", "KNX OFF");
        }
      }
      return;
...
  api:
    services:
      - service: knx_write_group_bool
        variables:
          address: string
          value: bool
        then:
          - lambda: |-
              id(knxd).group_write_bool(address.c_str(), value);
...
  - platform: gpio
    name: $friendly_devicename Relay 2
    id: relay_2
    pin:
      number: 19
    on_turn_on:
      - lambda: |-
          id(knxd).group_write_bool("0/0/3", true);
...
  binary_sensor:
  - platform: knx
    id: knx_sensor
    name: $friendly_devicename Switch
    group_address: "1/04/11"
    dpt: "1.001"
...
  sensor:
    - platform: knx
      id: knx_sensor_2
      name: $friendly_devicename Temperature
      group_address: "1/04/08"
      dpt: "9.001"
...
```
