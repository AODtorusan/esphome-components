# SPDX-License-Identifier: GPL-3.0-or-later

import os
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_SENSORS,
    CONF_URL,
    CONF_LEVEL,
)
from esphome.components.http_request import (
    HttpRequestComponent,
)
from esphome.components import binary_sensor, number, select, sensor, switch, text_sensor
from esphome.components.logger import LOG_LEVELS, is_log_level, request_log_listener
from esphome.cpp_types import EntityBase
from esphome.cpp_generator import MockObj, MockObjClass

CODEOWNERS = ["@aodtorusan"]
DEPENDENCIES = ["network", "logger"]

CONF_HTTP_REQUEST_ID = "http_request_id"
CONF_OTEL_PROTOCOL = "protocol"
CONF_OTEL_ENDPOINT = "endpoint"
CONF_OTEL_HEADERS = "headers"
CONF_OTEL_ATTRIBUTES = "attributes"
CONF_OTEL_RESOURCE_ATTRIBUTES = "resource_attributes"
CONF_OTEL_DROP_UNSYNCHRONIZED = "drop_time_unsynchronized"

CONF_OTEL_METRICS = "metrics"
CONF_OTEL_METRICS_SAMPLE_ON_CHANGE = "sample_on_change"
CONF_OTEL_METRICS_SAMPLE_INTERVAL = "sample_interval"
CONF_OTEL_METRICS_AUTODETECTION = "autodetection"
CONF_OTEL_METRICS_AUTODETECT_TEXT_SENSORS = "autodetect_text_sensors"
CONF_OTEL_METRICS_NAMING_SCHEME = "naming_scheme"
CONF_OTEL_METRICS_MAX_SAMPLES = "max_samples"
CONF_OTEL_METRICS_SENSOR_NAME = "name"

CONF_OTEL_LOGS = "logs"
CONF_OTEL_MAX_DEPTH = "max_depth"


ns = cg.esphome_ns.namespace("otel")
OTEL = ns.class_("OTEL", cg.Component)
LogsRecorder = ns.class_("LogsRecorder", cg.Component)
MetricsRecorder = ns.class_("MetricsRecorder", cg.Component)
MetricsAutoSensorDetection = ns.enum("MetricsAutoSensorDetection")
MetricsNamingScheme = ns.enum("MetricsNamingScheme")

Metric = ns.class_("Metric")
BinarySensorMetric = ns.class_("BinarySensorMetric")
NumberMetric = ns.class_("NumberMetric")
SelectMetric = ns.class_("SelectMetric")
SensorMetric = ns.class_("SensorMetric")
SwitchMetric = ns.class_("SwitchMetric")
TextSensorMetric = ns.class_("TextSensorMetric")

OTLP_PROTOCOLS = {
    "http/protobuf": "http/protobuf",
}

OTLP_METRICS_AUTODETECTION = {
    "none":     MetricsAutoSensorDetection.SENSORS_NONE,
    "internal": MetricsAutoSensorDetection.SENSORS_INTERNAL,
    "external": MetricsAutoSensorDetection.SENSORS_EXTERNAL,
    "all":      MetricsAutoSensorDetection.SENSORS_ALL,
}

OTLP_METRICS_NAMING = {
    "entity_name":  MetricsNamingScheme.ENTITY_NAME,
    "entity_type":  MetricsNamingScheme.ENTITY_TYPE,
    "device_class": MetricsNamingScheme.ENTITY_DEVICE_CLASS,
}

SENSOR_SCHEMA = cv.Schema(
    {
        cv.use_id(EntityBase): cv.Schema( {
            cv.GenerateID(): cv.declare_id(Metric),
            cv.Optional(CONF_OTEL_ATTRIBUTES): cv.Schema({cv.string: cv.string}),
            cv.Optional(CONF_OTEL_METRICS_MAX_SAMPLES): cv.int_range(min=2),
            cv.Optional(CONF_OTEL_METRICS_SENSOR_NAME): cv.string
        } )
    }
)

METRICS_SCHEMA = cv.Schema( {
    cv.GenerateID(): cv.declare_id(MetricsRecorder),
    cv.Optional(CONF_OTEL_METRICS_SAMPLE_ON_CHANGE): cv.boolean,
    cv.Optional(CONF_OTEL_METRICS_SAMPLE_INTERVAL): cv.positive_float,
    cv.Optional(CONF_OTEL_METRICS_AUTODETECTION, default="all"): cv.enum( OTLP_METRICS_AUTODETECTION ),
    cv.Optional(CONF_OTEL_METRICS_AUTODETECT_TEXT_SENSORS): cv.boolean,
    cv.Optional(CONF_OTEL_METRICS_NAMING_SCHEME, default="entity_type"): cv.enum( OTLP_METRICS_NAMING ),
    cv.Optional(CONF_OTEL_METRICS_MAX_SAMPLES): cv.int_range(min=2),
    cv.Optional(CONF_SENSORS, default={}): SENSOR_SCHEMA,
} )

LOGS_SCHEMA = cv.Schema( {
    cv.GenerateID(): cv.declare_id(LogsRecorder),
    cv.Optional(CONF_LEVEL, default="NONE"): is_log_level,
    cv.Optional(CONF_OTEL_ATTRIBUTES): cv.Schema({cv.string: cv.string}),
    cv.Optional(CONF_OTEL_MAX_DEPTH, default=100): cv.int_range(min=2),
} )

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_HTTP_REQUEST_ID): cv.use_id(HttpRequestComponent),

        cv.Optional(CONF_OTEL_PROTOCOL, default="http/protobuf"): cv.enum( OTLP_PROTOCOLS ),
        cv.Required(CONF_OTEL_ENDPOINT): cv.url,
        cv.Optional(CONF_OTEL_HEADERS): cv.Schema({cv.string: cv.string}),
        cv.Optional(CONF_OTEL_RESOURCE_ATTRIBUTES): cv.Schema({cv.string: cv.string}),
        cv.Optional(CONF_OTEL_DROP_UNSYNCHRONIZED): cv.boolean,

        cv.Optional(CONF_OTEL_METRICS): METRICS_SCHEMA,
        cv.Optional(CONF_OTEL_LOGS): LOGS_SCHEMA,
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    # Include nanopb itself
    cg.add_library(name="nanopb/Nanopb", version="0.4.91")
    # Include the compiled nanopb proto files
    in_repo_proto_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "libraries", "opentelemetry_proto"))
    if os.path.exists(in_repo_proto_path):
        cg.add_library(in_repo_proto_path, None)
    else:
        cg.add_library("aodtorusan/opentelemetry_proto", "1.9.0")

    # Library specific defines
    if config.get(CONF_OTEL_DROP_UNSYNCHRONIZED, False):
        cg.add_define("OTLP_DROP_UNSYNCHRONIZED")

    http = await cg.get_variable(config[CONF_HTTP_REQUEST_ID])

    # Metrics
    if config_metics := config.get(CONF_OTEL_METRICS):
        sample_on_change = config_metics.get(CONF_OTEL_METRICS_SAMPLE_ON_CHANGE, True)
        sample_interval = int(1000 * config_metics.get(CONF_OTEL_METRICS_SAMPLE_INTERVAL, 0)) # ms
        max_samples_per_metric = config_metics.get(CONF_OTEL_METRICS_MAX_SAMPLES, 50)
        metrics_autodetection = config_metics.get(CONF_OTEL_METRICS_AUTODETECTION)
        autodetect_text_sensors = config_metics.get(CONF_OTEL_METRICS_AUTODETECT_TEXT_SENSORS, False)
        naming_scheme = config_metics.get(CONF_OTEL_METRICS_NAMING_SCHEME)

        metricsRecorder = cg.new_Pvariable(config_metics[CONF_ID], http, sample_on_change, sample_interval, max_samples_per_metric, metrics_autodetection, autodetect_text_sensors, naming_scheme)
        await cg.register_component(metricsRecorder, config_metics)

        ## Common stuff
        #cg.add(metricsRecorder.set_protocol( config.get(CONF_OTEL_PROTOCOL) ))
        cg.add(metricsRecorder.set_endpoint( config.get(CONF_OTEL_ENDPOINT) ))
        for header_key, header_value in config.get(CONF_OTEL_HEADERS, {}).items():
            cg.add(metricsRecorder.add_header( header_key, header_value ))
        for attr_key, attr_value in config.get(CONF_OTEL_RESOURCE_ATTRIBUTES, {}).items():
            cg.add(metricsRecorder.add_resource_attribute( attr_key, attr_value ))

        ## Metrics specifics
        for sensor_id, sensor_config in config_metics.get(CONF_SENSORS, {}).items():
            sensor_var: MockObj = await cg.get_variable(sensor_id)
            sensor_var_type: MockObjClass = sensor_var.base.type

            sample_depth = sensor_config.get(CONF_OTEL_METRICS_MAX_SAMPLES, max_samples_per_metric)

            metric: MockObj = None
            if (sensor_var_type.inherits_from( binary_sensor.BinarySensor )):
                cls = BinarySensorMetric
            elif (sensor_var_type.inherits_from( number.Number )):
                cls = NumberMetric
            elif (sensor_var_type.inherits_from( select.Select )):
                cls = SelectMetric
            elif (sensor_var_type.inherits_from( sensor.Sensor )):
                cls = SensorMetric
            elif (sensor_var_type.inherits_from( switch.Switch )):
                cls = SwitchMetric
            elif (sensor_var_type.inherits_from( text_sensor.TextSensor )):
                cls = TextSensorMetric
            else:
                raise ValueError(f"Could not determine the proper metric type to create for this entity type! '{sensor_var}' of type '{sensor_var_type}'")

            metric_id = sensor_config[CONF_ID]
            metric_id.type = cls # Set the concrete type in the ID correctly as thats used for new now? See https://github.com/esphome/esphome/commit/cd05462e9fc4a295b523412c04fa17acb6c86171#diff-aab8418f271a6750658ca053b5a32a8668972d3b25c2395f485ebce0f8fbb5a2R586
            metric = cg.Pvariable(metric_id, cls.new(metricsRecorder, sensor_var, naming_scheme, sample_depth ))
            cg.add(metricsRecorder.add_metric(metric))
            if sample_on_change:
                cg.add(metric.install_sample_hook())
            for key, value in sensor_config.get(CONF_OTEL_ATTRIBUTES, {}).items():
                cg.add(metric.add_attribute(key, value))
            if sensor_name := sensor_config.get(CONF_OTEL_METRICS_SENSOR_NAME):
                cg.add(metric.set_name(sensor_name))


    # Logs
    if config_logs := config.get(CONF_OTEL_LOGS):
        level = config_logs.get(CONF_LEVEL, "NONE")
        if level != "NONE":
            logsRecorder = cg.new_Pvariable(config_logs[CONF_ID], http, LOG_LEVELS[level])
            await cg.register_component(logsRecorder, config_logs)

            ## Common stuff
            #cg.add(logsRecorder.set_protocol( config.get(CONF_OTEL_PROTOCOL) ))
            cg.add(logsRecorder.set_endpoint( config.get(CONF_OTEL_ENDPOINT) ))
            for header_key, header_value in config.get(CONF_OTEL_HEADERS, {}).items():
                cg.add(logsRecorder.add_header( header_key, header_value ))
            for attr_key, attr_value in config.get(CONF_OTEL_RESOURCE_ATTRIBUTES, {}).items():
                cg.add(logsRecorder.add_resource_attribute( attr_key, attr_value ))

            ## Logs specifics
            # Request a log listener slot for syslog
            request_log_listener()
            # Set the max depth of logs
            cg.add(logsRecorder.set_logger_max_depth( config_logs.get(CONF_OTEL_MAX_DEPTH) ))
            # Add the attributes that are specific to the logger
            for attr_key, attr_value in config_logs.get(CONF_OTEL_ATTRIBUTES, {}).items():
                cg.add(logsRecorder.add_logger_attribute( attr_key, attr_value ))
