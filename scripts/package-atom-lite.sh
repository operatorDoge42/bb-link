#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/.." && pwd)
VERSION=${1:-0.1.0-alpha.1}
BUILD_DIR="$ROOT/build/atom-lite"
RELEASE_DIR="$BUILD_DIR/release"
CLI=${ARDUINO_CLI:-arduino-cli}
ESPTOOL=${ESPTOOL:-esptool}
ESP32_CORE_DIR=${ESP32_CORE_DIR:?Set ESP32_CORE_DIR to the installed esp32/2.0.15 core directory}
BOOT_APP0="$ESP32_CORE_DIR/tools/partitions/boot_app0.bin"

if [ ! -f "$BOOT_APP0" ]; then
  echo "Missing $BOOT_APP0" >&2
  exit 1
fi

ARDUINO_CLI="$CLI" "$SCRIPT_DIR/build-atom-lite.sh"
mkdir -p "$RELEASE_DIR"

IMAGE="$RELEASE_DIR/bb-link-atom-lite-v$VERSION-factory.bin"
"$ESPTOOL" --chip esp32 merge_bin \
  --target-offset 0 \
  --fill-flash-size 4MB \
  --flash_mode keep \
  --flash_freq keep \
  --flash_size keep \
  --output "$IMAGE" \
  0x1000 "$BUILD_DIR/bb-link.ino.bootloader.bin" \
  0x8000 "$BUILD_DIR/bb-link.ino.partitions.bin" \
  0xe000 "$BOOT_APP0" \
  0x10000 "$BUILD_DIR/bb-link.ino.bin"

shasum -a 256 "$IMAGE" > "$IMAGE.sha256.txt"
echo "Factory firmware: $IMAGE"
echo "Checksum: $IMAGE.sha256.txt"
