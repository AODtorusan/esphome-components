#!/usr/bin/env bash

set -ex
rm -rf src/*

options=(
    "-D" "$PWD/src"
    "--proto-path" "$PWD/opentelemetry-proto"
    "--options-file" "$(realpath nanopb.options)"
    "--cpp-descriptors"
)

files=(
    opentelemetry-proto/opentelemetry/proto/common/v1/common.proto
    opentelemetry-proto/opentelemetry/proto/metrics/v1/metrics.proto
    opentelemetry-proto/opentelemetry/proto/resource/v1/resource.proto
    opentelemetry-proto/opentelemetry/proto/logs/v1/logs.proto
)

for f in ${files[@]}; do
    nanopb_generator "${options[@]}" "$(realpath $f)"
done
