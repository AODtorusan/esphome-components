# knx - Component for esphome

<a href="https://opensource.org"><img height="150" align="left" src="https://opensource.org/files/OSIApprovedCropped.png" alt="Open Source Initiative Approved License logo"></a>
[![License - GNU GPL v3.0+](https://img.shields.io/badge/License-GNU%20GPL%203.0%2b-9400d3.svg)](https://spdx.org/licenses/GPL-3.0-or-later.html)

This repo contains an ESPHome component for publishing metrics and logs to an [opentelemetry](https://opentelemetry.io/) endpoint over OTLP/http encoded with protobuf.

This allows you to push send metrics/logs for example to:
* opentelemtry-collector
* Grafana cloud: [Send and ingest OTLP data
](https://grafana.com/docs/opentelemetry/ingest/)
* prometheus: [Using Prometheus as your OpenTelemetry backend](https://prometheus.io/docs/guides/opentelemetry/)
* victoriametrics / victorialogs: [How to use OpenTelemetry with VictoriaMetrics and VictoriaLogs](https://docs.victoriametrics.com/guides/getting-started-with-opentelemetry/)

Open Source software: [GNU General Public License v3.0 or later](https://spdx.org/licenses/GPL-3.0-or-later.html)

## Configuration variables

* `id` (Optional, ID): Manually specify the ID used for code generation.
* `protocol` (Optional, string): Protocol to use when talking to the OTLP endpoint. Currently only `http/protobuf` is supported.
* `endpoint` (Requires, string): HTTP(s) base url to send the metrics to. For example `http://192.168.0.100:4318`
* `headers` (Optional, mapping[str, str]): Map of HTTP headers to add to each reqest. Can for example be used to add authentication or a tenant/x-org id. Defaults to `{}`.
* `attributes` (Optional, mapping[str, str]): Map of string attributes to add the OTLP resource. Commonly this includes `service.name` or `device.name`. Defaults to `{}`.
* `drop_time_unsynchronized` (Optional, boolean): If set to `true` then metrics/logs that are generated before the device has syncronized its clock to an absolute time (over eg ntp) are dropped. Some endpoints do not accept metrics/logs that do not have a correct absolute time. Defaults to `false`
* `metrics` (Optional, mapping): If defined, metrics will be submitted to the endpoint by this component.
  * `id` (Optional, ID): Manually specify the ID used for code generation.
  * `sample_on_change` (Optional, boolean): When set to `true`, each a sensor receives a new sample, a new metric datapoint is submitted to the OTLP endpoint. Defaults to `true`
  * `sample_interval` (Optional, float): If set to a a non-zero value, a scheduller will submit metric datapoints for each selected sensor every specified number of seconds, regardless if the sensor has a new sensor value or not. Default: 0
  * `autodetection` (Optional, enum): Which sensors to automatically collect metrics for. Defaults to `all`. Can be one of:
    * `none`: Do not automatically collect metrics from specific sensors (you can always specify the sensors manually via the `sensors` key).
    * `internal`: Submit metric for all internal sensors in this device (sensors without name).
    * `external`: Submit metric for all external sensors in this device (sensors with name).
    * `all`: Submit metric for all sensors
  * `autodetect_text_sensors` (Optional, boolean): If set to true, text_sensors will emit metrics with a label `text` set to the value of the current text.
  * `naming_scheme` (Optional, enum): Determines how the name of an OTLP metric is derived from an esphome entity. This can always be overwritten per entity under `sensors`.
    * `entity_name`: Sets the name of the metric to the name of the entity
    * `entity_type`: Sets the name of the metric to the type of the entity (eg sensor, binary_sensor, ...)
    * `device_class`: Sets the name of the metric to the device_class of the entity. If no device class is set its set to `unknown`
  * `max_samples` (Optional, integer): Maximum number of historical samples to store in-memory when the connection to the OTLP endpoint is down. Defaults to 50.
  * `sensors` (Optional, mapping): List of sensors to collect metrics for in addition to those specified by the `autodetection` key.
    * `$sensor_id` (Optional, mapping): ID of the sensor to collect metrics for
      * `id` (Optional, ID): Manually specify the ID used for code generation.
      * `name` (Optional, string): Override the name of the metric that this sensor generated. Default: see `naming_scheme`
      * `attributes` (Optional, mapping[str, str]): Map of string attributes to add the OTLP Metric (scope). Defaults to `{}`.
      * `max_samples` (Optional, integer): Maximum number of historical samples to store in-memory when the connection to the OTLP endpoint is down. Defaults to `max_samples` defined under `metrics`.
* `logs` (Optional, mapping): If defined, logs will be submitted to the endpoint by this component.
  * `id` (Optional, ID): Manually specify the ID used for code generation.
  * `level` (Optional, log_level). See [Log Levels](https://esphome.io/components/logger/#log-levels). Defaults to `NONE`.
  * `attributes` (Optional, mapping[str, str]): Map of string attributes to add the OTLP Logs (scope). Defaults to `{}`.

## Minimal example

The following configuration adds the custom component and does the minimal setup to send metrics to a basic HTTP OTLP endpoint:

```yaml
external_components:
  - source:
      type: git
      ref: esphome-latest
      url: https://github.com/AODtorusan/esphome-components
    components: [ otel ]

otel:
  endpoint: http://192.168.0.100:4318
  logs:
    level: DEBUG
  metrics:
    autodetection: all
```

While not required, its also recommended to add a time platform to the device for absolute time. This will cause all datapoints/log records to have the correct absolute time:

```yaml
time:
  - platform: sntp
    id: sntp_time
    servers:
     - 0.pool.ntp.org
     - 1.pool.ntp.org
     - 2.pool.ntp.org
```

## Generated OTLP Structure

Important:

>  The timestamp on the metrics datapoints or logrecrds is either 0 if the device is does not have an absolute clock (eg ntp syncronized) or the absolute time if it is. This allows you to set the timestamp on of datapoints/records on the OTLP receiving side on the time they are received.
>
>  This can be done for example with a local instance of opentelemetry-collector (contrib) and rewrite the samples with a timestamp of 0 to the time of reception by on the collector instance. The see the `transform/fix_timestamps` processor in the [example opentelemetry config](../../examples/otel-collector.yaml).

### Metrics

For metrics, each entity is translated into a `ScopeMetrics`. This is done to avoid having to put the attributes linked to a sensor in each submitted datapoint.

The following entity types can be used to generate metrics:

* [binary_sensor](https://esphome.io/components/binary_sensor/)
* [number](https://esphome.io/components/number/)
* [select](https://esphome.io/components/select/)
  * Every datapoint has an attribute `selection` with value the string representaiton of the selected value (the dpt value is the index of the selection)
* [sensor](https://esphome.io/components/sensor/)
* [switch](https://esphome.io/components/switch/)
* [text_sensor](https://esphome.io/components/text_sensor/)
  * Every datapoint has an attribute `text` with value the string representaiton of the text_sensor (the dpt value is always 1)
  * Autodetection must be explicitly enabled via `autodetect_text_sensors`
  * Be very carefull using this! Use this only if there are limited number of possible text states.
    * See https://opentelemetry.io/docs/languages/dotnet/metrics/best-practices/#cardinality-limits
    * See https://prometheus.io/docs/practices/the_zen/#cardinality-matters

The name of the metric for each sensor is determined by the explicit name set for the sensor under metrics or the set scheme under `naming_scheme` otherwise.

The type of metric that is used depends on the `state_class` of the sensor (`sensor[].state_class`).

  * `total`: Sum metric with `is_monotonic` set to `false` and `aggregation_temporality` set to `AGGREGATION_TEMPORALITY_CUMULATIVE`
  * `total_increasing`: Sum metric with `is_monotonic` set to `true` and `aggregation_temporality` set to `AGGREGATION_TEMPORALITY_CUMULATIVE`
  * all others: Gauge metric

You can set this on the sensor to force a specific metric type:
```yaml
sensor:
  - platform: template
    id: example_sensor
    state_class: "total_increasing"
```

Every sensor by-default has the following attributes (if they are not empty):

* `entity_name`: Name of the entity, set `entity[].name` (or then encoded id)
* `entity_id`: Sanitized id / name of the entity
* `entity_type`: Type of the entity, like sensor, binary_sensor, number, select, ...
* `unit`: Unit of the entity, set `entity[].unit_of_measurement`
* `device_class`: The device class for the entity, set `entity[].device_class`
* `state_class`: The state class for the sensor, set `sensor[].state_class`

### Logs

For logs, each log line is translated into a `LogRecord`. All the log records are stored in a single `ScopeLogs`, which contains the specified logs specific attributes.

The severity number and text in the logrecord are based on the loglevel in esphome.

The tag used for logging is recorded in the logrecord `event_name` and `tag` attribute (because event_name is quite new and not supported everywhere yet).

## Running an OTLP endpoint

### Opentelemetry-collector

Using the following config for your device:

```
otel:
  endpoint: http://192.168.0.100:4318 # Set the IP/hostname to otel-collector

  resource_attributes:
    test: "1"
  logs:
    level: DEBUG
  metrics: {}
```

You can start otel-collector with the following minimal config to get it to accept and print your metrics / logs:

```
receivers:
  otlp:
    protocols:
      http:
        endpoint: 0.0.0.0:4318
exporters:
  debug:
    verbosity: detailed
service:
  pipelines:
    metrics:
      receivers: [otlp]
      processors: []
      exporters: [debug]
    logs:
      receivers: [otlp]
      processors: []
      exporters: [debug]
```

You can run this config via:

```
docker run -p 4318:4318 -v "$(realpath otel-collector.yaml):/etc/otelcol/config.yaml" otel/opentelemetry-collector:0.147.0
```

A more complete example of a config can be found under [examples/otel-collector.yaml](../../examples/otel-collector.yaml). Note that this

### Grafana cloud example

You need an account on [Grafana Cloud](https://grafana.com/products/cloud/). Afer which you need to use/add an OTLP connection to send the metrics/logs to. See https://grafana.com/docs/grafana-cloud/send-data/otlp/send-data-otlp/.

Once that is done simply set the respective values in you device yaml config and you should see the data appearing in grafana:

```
otel:
  endpoint: https://otlp-gateway-prod-eu-west-2.grafana.net/otlp
  drop_time_unsynchronized: true # Grafana Cloud refuses samples that are not timestamped with absolute time
  headers:
    Authorization: "Basic ...."

  resource_attributes:
    # Grafana cloud uses this to index
    service.namespace: "esphome"
    service.name: "${node_name}"
    # Custom attributes
    test: "1"
  logs:
    level: DEBUG
  metrics: {}
```

Note: For grafana logs, at least one of the special attributes needs to be set under `resource_attributes`, otherwise you logs will be dropped. See https://grafana.com/docs/grafana-cloud/send-data/logs/collect-logs-with-otel/#format-considerations

Note: We need to set `drop_time_unsynchronized` to `true` in Grafana Cloud rejects samples/logs without an absolute timestamp. This will lose the first logs/samples of the device until its NTP synchronized. To avoid this see [Generated OTLP Structure], just add an exporter for Grafana Cloud to opentelemetry-collector.
