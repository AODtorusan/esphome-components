#!/usr/bin/env bash

# docker run \
#   -p 4317:4317 \
#   -p 4318:4318 \
#   -v "$(realpath otel-collector.yaml):/etc/otelcol/config.yaml" \
#   otel/opentelemetry-collector:0.147.0

# We need the 'opentelemetry-collector-contrib' image for the transform processor
docker run \
  -p 4317:4317 \
  -p 4318:4318 \
  -v "$(realpath otel-collector.yaml):/etc/otelcol-contrib/config.yaml" \
  otel/opentelemetry-collector-contrib:0.147.0
