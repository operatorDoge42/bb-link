#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/.." && pwd)
CLI=${ARDUINO_CLI:-arduino-cli}
BUILD_DIR="$ROOT/build/atom-lite"

mkdir -p "$BUILD_DIR"

if [ -n "${ARDUINO_CLI_CONFIG:-}" ]; then
  "$CLI" --config-file "$ARDUINO_CLI_CONFIG" compile \
    --fqbn esp32:esp32:m5stack-atom:PartitionScheme=default \
    --build-path "$BUILD_DIR" \
    "$ROOT/src/bb-link"
else
  "$CLI" compile \
    --fqbn esp32:esp32:m5stack-atom:PartitionScheme=default \
    --build-path "$BUILD_DIR" \
    "$ROOT/src/bb-link"
fi
