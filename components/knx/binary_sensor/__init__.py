# SPDX-License-Identifier: MIT

# File: __init__.py
# Author: Simon Billemont (Since 2026)

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor

from .. import knx_component, knx_ns, KnxDpt, KNX_DPT_ENUM

KnxBinarySensor = knx_ns.class_("KnxBinarySensor", binary_sensor.BinarySensor, cg.Component)

CONF_KNX_ID = "knx_id"
CONF_GROUP_ADDRESS = "group_address"
CONF_DPT = "dpt"


CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(
    KnxBinarySensor,
).extend( {
    cv.GenerateID(CONF_KNX_ID): cv.use_id(knx_component),
    cv.Required(CONF_GROUP_ADDRESS): cv.string,
    cv.Required(CONF_DPT): cv.enum(KNX_DPT_ENUM),
} ).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    knx = await cg.get_variable(config[CONF_KNX_ID])
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)

    cg.add(var.set_knx(knx))
    cg.add(var.set_group_address(config[CONF_GROUP_ADDRESS]))
    cg.add(var.set_dpt(config[CONF_DPT]))
