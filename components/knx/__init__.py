# SPDX-License-Identifier: MIT

# File: __init__.py
# Author: Dulgheru Mihaita (Since 2022)
# Modified: Simon Billemont (Since 2026)

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID, CONF_UART_ID, CONF_USE_ADDRESS, CONF_LAMBDA


CODEOWNERS = ["@aodtorusan"]
DEPENDENCIES = ["uart"]

knx_ns = cg.esphome_ns.namespace("knx")
knx_component = knx_ns.class_("KnxComponent", cg.Component, uart.UARTDevice)
KnxDpt = knx_ns.enum("KnxDpt")

KNX_DPT_ENUM = {
    "1.001": KnxDpt.KNX_DPT_1_001,
    "1.002": KnxDpt.KNX_DPT_1_002,
    "1.003": KnxDpt.KNX_DPT_1_003,
    "1.004": KnxDpt.KNX_DPT_1_004,
    "1.005": KnxDpt.KNX_DPT_1_005,
    "1.006": KnxDpt.KNX_DPT_1_006,
    "1.007": KnxDpt.KNX_DPT_1_007,
    "1.008": KnxDpt.KNX_DPT_1_008,
    "1.009": KnxDpt.KNX_DPT_1_009,
    "1.010": KnxDpt.KNX_DPT_1_010,
    "1.011": KnxDpt.KNX_DPT_1_011,
    "1.012": KnxDpt.KNX_DPT_1_012,
    "1.013": KnxDpt.KNX_DPT_1_013,
    "1.014": KnxDpt.KNX_DPT_1_014,
    "1.015": KnxDpt.KNX_DPT_1_015,
    "1.016": KnxDpt.KNX_DPT_1_016,
    "1.017": KnxDpt.KNX_DPT_1_017,
    "1.018": KnxDpt.KNX_DPT_1_018,
    "1.019": KnxDpt.KNX_DPT_1_019,
    "1.021": KnxDpt.KNX_DPT_1_021,
    "1.022": KnxDpt.KNX_DPT_1_022,
    "1.023": KnxDpt.KNX_DPT_1_023,
    "1.024": KnxDpt.KNX_DPT_1_024,
    "1.100": KnxDpt.KNX_DPT_1_100,
    "1.1200": KnxDpt.KNX_DPT_1_1200,
    "1.1201": KnxDpt.KNX_DPT_1_1201,
    "2.001": KnxDpt.KNX_DPT_2_001,
    "2.002": KnxDpt.KNX_DPT_2_002,
    "2.003": KnxDpt.KNX_DPT_2_003,
    "2.004": KnxDpt.KNX_DPT_2_004,
    "2.005": KnxDpt.KNX_DPT_2_005,
    "2.006": KnxDpt.KNX_DPT_2_006,
    "2.007": KnxDpt.KNX_DPT_2_007,
    "2.008": KnxDpt.KNX_DPT_2_008,
    "2.009": KnxDpt.KNX_DPT_2_009,
    "2.010": KnxDpt.KNX_DPT_2_010,
    "2.011": KnxDpt.KNX_DPT_2_011,
    "2.012": KnxDpt.KNX_DPT_2_012,
    "3.007": KnxDpt.KNX_DPT_3_007,
    "3.008": KnxDpt.KNX_DPT_3_008,
    "4.001": KnxDpt.KNX_DPT_4_001,
    "4.002": KnxDpt.KNX_DPT_4_002,
    "5.001": KnxDpt.KNX_DPT_5_001,
    "5.003": KnxDpt.KNX_DPT_5_003,
    "5.004": KnxDpt.KNX_DPT_5_004,
    "5.005": KnxDpt.KNX_DPT_5_005,
    "5.006": KnxDpt.KNX_DPT_5_006,
    "5.010": KnxDpt.KNX_DPT_5_010,
    "6.001": KnxDpt.KNX_DPT_6_001,
    "6.010": KnxDpt.KNX_DPT_6_010,
    "6.020": KnxDpt.KNX_DPT_6_020,
    "7.001": KnxDpt.KNX_DPT_7_001,
    "7.002": KnxDpt.KNX_DPT_7_002,
    "7.003": KnxDpt.KNX_DPT_7_003,
    "7.004": KnxDpt.KNX_DPT_7_004,
    "7.005": KnxDpt.KNX_DPT_7_005,
    "7.006": KnxDpt.KNX_DPT_7_006,
    "7.007": KnxDpt.KNX_DPT_7_007,
    "7.010": KnxDpt.KNX_DPT_7_010,
    "7.011": KnxDpt.KNX_DPT_7_011,
    "7.012": KnxDpt.KNX_DPT_7_012,
    "7.013": KnxDpt.KNX_DPT_7_013,
    "7.600": KnxDpt.KNX_DPT_7_600,
    "8.001": KnxDpt.KNX_DPT_8_001,
    "8.002": KnxDpt.KNX_DPT_8_002,
    "8.003": KnxDpt.KNX_DPT_8_003,
    "8.004": KnxDpt.KNX_DPT_8_004,
    "8.005": KnxDpt.KNX_DPT_8_005,
    "8.006": KnxDpt.KNX_DPT_8_006,
    "8.007": KnxDpt.KNX_DPT_8_007,
    "8.010": KnxDpt.KNX_DPT_8_010,
    "8.011": KnxDpt.KNX_DPT_8_011,
    "8.012": KnxDpt.KNX_DPT_8_012,
    "9.001": KnxDpt.KNX_DPT_9_001,
    "9.002": KnxDpt.KNX_DPT_9_002,
    "9.003": KnxDpt.KNX_DPT_9_003,
    "9.004": KnxDpt.KNX_DPT_9_004,
    "9.005": KnxDpt.KNX_DPT_9_005,
    "9.006": KnxDpt.KNX_DPT_9_006,
    "9.007": KnxDpt.KNX_DPT_9_007,
    "9.008": KnxDpt.KNX_DPT_9_008,
    "9.009": KnxDpt.KNX_DPT_9_009,
    "9.010": KnxDpt.KNX_DPT_9_010,
    "9.011": KnxDpt.KNX_DPT_9_011,
    "9.020": KnxDpt.KNX_DPT_9_020,
    "9.021": KnxDpt.KNX_DPT_9_021,
    "9.022": KnxDpt.KNX_DPT_9_022,
    "9.023": KnxDpt.KNX_DPT_9_023,
    "9.024": KnxDpt.KNX_DPT_9_024,
    "9.025": KnxDpt.KNX_DPT_9_025,
    "9.026": KnxDpt.KNX_DPT_9_026,
    "9.027": KnxDpt.KNX_DPT_9_027,
    "9.028": KnxDpt.KNX_DPT_9_028,
    "9.029": KnxDpt.KNX_DPT_9_029,
    "9.030": KnxDpt.KNX_DPT_9_030,
    "10.001": KnxDpt.KNX_DPT_10_001,
    "11.001": KnxDpt.KNX_DPT_11_001,
    "12.001": KnxDpt.KNX_DPT_12_001,
    "12.100": KnxDpt.KNX_DPT_12_100,
    "12.101": KnxDpt.KNX_DPT_12_101,
    "12.102": KnxDpt.KNX_DPT_12_102,
    "12.1200": KnxDpt.KNX_DPT_12_1200,
    "12.1201": KnxDpt.KNX_DPT_12_1201,
    "13.001": KnxDpt.KNX_DPT_13_001,
    "13.002": KnxDpt.KNX_DPT_13_002,
    "13.010": KnxDpt.KNX_DPT_13_010,
    "13.011": KnxDpt.KNX_DPT_13_011,
    "13.012": KnxDpt.KNX_DPT_13_012,
    "13.013": KnxDpt.KNX_DPT_13_013,
    "13.014": KnxDpt.KNX_DPT_13_014,
    "13.015": KnxDpt.KNX_DPT_13_015,
    "13.016": KnxDpt.KNX_DPT_13_016,
    "13.100": KnxDpt.KNX_DPT_13_100,
    "13.1200": KnxDpt.KNX_DPT_13_1200,
    "13.1201": KnxDpt.KNX_DPT_13_1201,
    "14.000": KnxDpt.KNX_DPT_14_000,
    "14.001": KnxDpt.KNX_DPT_14_001,
    "14.002": KnxDpt.KNX_DPT_14_002,
    "14.003": KnxDpt.KNX_DPT_14_003,
    "14.004": KnxDpt.KNX_DPT_14_004,
    "14.005": KnxDpt.KNX_DPT_14_005,
    "14.006": KnxDpt.KNX_DPT_14_006,
    "14.007": KnxDpt.KNX_DPT_14_007,
    "14.008": KnxDpt.KNX_DPT_14_008,
    "14.009": KnxDpt.KNX_DPT_14_009,
    "14.010": KnxDpt.KNX_DPT_14_010,
    "14.011": KnxDpt.KNX_DPT_14_011,
    "14.012": KnxDpt.KNX_DPT_14_012,
    "14.013": KnxDpt.KNX_DPT_14_013,
    "14.014": KnxDpt.KNX_DPT_14_014,
    "14.015": KnxDpt.KNX_DPT_14_015,
    "14.016": KnxDpt.KNX_DPT_14_016,
    "14.017": KnxDpt.KNX_DPT_14_017,
    "14.018": KnxDpt.KNX_DPT_14_018,
    "14.019": KnxDpt.KNX_DPT_14_019,
    "14.020": KnxDpt.KNX_DPT_14_020,
    "14.021": KnxDpt.KNX_DPT_14_021,
    "14.022": KnxDpt.KNX_DPT_14_022,
    "14.023": KnxDpt.KNX_DPT_14_023,
    "14.024": KnxDpt.KNX_DPT_14_024,
    "14.025": KnxDpt.KNX_DPT_14_025,
    "14.026": KnxDpt.KNX_DPT_14_026,
    "14.027": KnxDpt.KNX_DPT_14_027,
    "14.028": KnxDpt.KNX_DPT_14_028,
    "14.029": KnxDpt.KNX_DPT_14_029,
    "14.030": KnxDpt.KNX_DPT_14_030,
    "14.031": KnxDpt.KNX_DPT_14_031,
    "14.032": KnxDpt.KNX_DPT_14_032,
    "14.033": KnxDpt.KNX_DPT_14_033,
    "14.034": KnxDpt.KNX_DPT_14_034,
    "14.035": KnxDpt.KNX_DPT_14_035,
    "14.036": KnxDpt.KNX_DPT_14_036,
    "14.037": KnxDpt.KNX_DPT_14_037,
    "14.038": KnxDpt.KNX_DPT_14_038,
    "14.039": KnxDpt.KNX_DPT_14_039,
    "14.040": KnxDpt.KNX_DPT_14_040,
    "14.041": KnxDpt.KNX_DPT_14_041,
    "14.042": KnxDpt.KNX_DPT_14_042,
    "14.043": KnxDpt.KNX_DPT_14_043,
    "14.044": KnxDpt.KNX_DPT_14_044,
    "14.045": KnxDpt.KNX_DPT_14_045,
    "14.046": KnxDpt.KNX_DPT_14_046,
    "14.047": KnxDpt.KNX_DPT_14_047,
    "14.048": KnxDpt.KNX_DPT_14_048,
    "14.049": KnxDpt.KNX_DPT_14_049,
    "14.050": KnxDpt.KNX_DPT_14_050,
    "14.051": KnxDpt.KNX_DPT_14_051,
    "14.052": KnxDpt.KNX_DPT_14_052,
    "14.053": KnxDpt.KNX_DPT_14_053,
    "14.054": KnxDpt.KNX_DPT_14_054,
    "14.055": KnxDpt.KNX_DPT_14_055,
    "14.056": KnxDpt.KNX_DPT_14_056,
    "14.057": KnxDpt.KNX_DPT_14_057,
    "14.058": KnxDpt.KNX_DPT_14_058,
    "14.059": KnxDpt.KNX_DPT_14_059,
    "14.060": KnxDpt.KNX_DPT_14_060,
    "14.061": KnxDpt.KNX_DPT_14_061,
    "14.062": KnxDpt.KNX_DPT_14_062,
    "14.063": KnxDpt.KNX_DPT_14_063,
    "14.064": KnxDpt.KNX_DPT_14_064,
    "14.065": KnxDpt.KNX_DPT_14_065,
    "14.066": KnxDpt.KNX_DPT_14_066,
    "14.067": KnxDpt.KNX_DPT_14_067,
    "14.068": KnxDpt.KNX_DPT_14_068,
    "14.069": KnxDpt.KNX_DPT_14_069,
    "14.070": KnxDpt.KNX_DPT_14_070,
    "14.071": KnxDpt.KNX_DPT_14_071,
    "14.072": KnxDpt.KNX_DPT_14_072,
    "14.073": KnxDpt.KNX_DPT_14_073,
    "14.074": KnxDpt.KNX_DPT_14_074,
    "14.075": KnxDpt.KNX_DPT_14_075,
    "14.076": KnxDpt.KNX_DPT_14_076,
    "14.077": KnxDpt.KNX_DPT_14_077,
    "14.078": KnxDpt.KNX_DPT_14_078,
    "14.079": KnxDpt.KNX_DPT_14_079,
    "14.080": KnxDpt.KNX_DPT_14_080,
    "14.1200": KnxDpt.KNX_DPT_14_1200,
    "14.1201": KnxDpt.KNX_DPT_14_1201,
}

CONF_LISTENING_ADDRESSES = "listen_group_address"
CONF_SERIAL_TIMEOUT = "serial_timeout"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(knx_component),
            cv.Required(CONF_USE_ADDRESS): cv.string,
            cv.Optional(CONF_LAMBDA): cv.returning_lambda,
            cv.Optional(CONF_LISTENING_ADDRESSES, default=[]): cv.ensure_list(
                cv.string_strict
            ),
            cv.Optional(CONF_SERIAL_TIMEOUT, default=1000): cv.uint32_t,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    uart_component = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config[CONF_ID], uart_component)

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(knx_component, "knx")], return_type=cg.void
        )
        cg.add(var.set_lambda_writer(lambda_))

    cg.add(var.set_use_address(config[CONF_USE_ADDRESS]))
    cg.add(var.set_serial_timeout(config[CONF_SERIAL_TIMEOUT]))

    for addreses in config[CONF_LISTENING_ADDRESSES]:
        cg.add(var.add_listen_group_address(addreses))

    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
