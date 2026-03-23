#!/usr/bin/env bash

# This regenerates the compiledb used by the cland plugin based on the last built esphome device
# This requires that the device has:
#
#esphome:
#  build_path: ./../../build
#  environment_variables:
#    COMPILATIONDB_INCLUDE_TOOLCHAIN: "True"

cd build
pio run -t compiledb
