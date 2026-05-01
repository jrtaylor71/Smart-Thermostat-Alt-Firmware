#!/bin/bash
# Post-build firmware organization script

BUILD_DIR=".pio/build/esp32-s3-wroom-1-n16"
PROJECT_DIR="$(pwd)"
FIRMWARE_DIR="$PROJECT_DIR/firmware"
VERSION_SOURCE_FILE="$PROJECT_DIR/src/Main-Thermostat.cpp"

BOARD_VARIANT_LABEL="${BOARD_VARIANT_LABEL:-N16}"

extract_firmware_version() {
    if [[ -f "$VERSION_SOURCE_FILE" ]]; then
        sed -n 's/^const String sw_version = "\([^"]*\)".*/\1/p' "$VERSION_SOURCE_FILE" | head -n 1
    fi
}

FIRMWARE_VERSION="${FIRMWARE_VERSION:-$(extract_firmware_version)}"

if [[ -z "$FIRMWARE_VERSION" ]]; then
    echo "[POST-BUILD] Failed to determine firmware version from $VERSION_SOURCE_FILE"
    exit 1
fi

# How many historical firmware builds to keep (default: 1). Set via -k/--keep or KEEP_FIRMWARE env.
KEEP_FIRMWARE="${KEEP_FIRMWARE:-1}"

while [[ $# -gt 0 ]]; do
    case "$1" in
        -k|--keep)
            KEEP_FIRMWARE="$2"
            shift 2
            ;;
        *)
            echo "[POST-BUILD] Unknown argument: $1"
            echo "[POST-BUILD] Usage: ./organize_firmware.sh [-k|--keep <count>]"
            exit 1
            ;;
    esac
done

if [[ ! "$KEEP_FIRMWARE" =~ ^[0-9]+$ ]] || [[ "$KEEP_FIRMWARE" -lt 1 ]]; then
    echo "[POST-BUILD] Invalid keep count '$KEEP_FIRMWARE', defaulting to 1"
    KEEP_FIRMWARE=1
fi

# Create firmware directory if it doesn't exist
VARIANT_FIRMWARE_DIR="$FIRMWARE_DIR/$BOARD_VARIANT_LABEL"
mkdir -p "$VARIANT_FIRMWARE_DIR"

# Create version directory with build date and time
BUILD_TIMESTAMP=$(date +"%Y%m%d-%H%M%S")
VERSION_DIR_NAME="build_${BUILD_TIMESTAMP}_v${FIRMWARE_VERSION}"
VERSION_DIR="$VARIANT_FIRMWARE_DIR/$VERSION_DIR_NAME"

BOOTLOADER_BIN_NAME="bootloader_v${FIRMWARE_VERSION}_${BUILD_TIMESTAMP}.bin"
FIRMWARE_BIN_NAME="firmware_v${FIRMWARE_VERSION}_${BUILD_TIMESTAMP}.bin"
PARTITIONS_BIN_NAME="partitions_v${FIRMWARE_VERSION}_${BUILD_TIMESTAMP}.bin"
FIRMWARE_ELF_NAME="firmware_v${FIRMWARE_VERSION}_${BUILD_TIMESTAMP}.elf"

mkdir -p "$VERSION_DIR"

# Copy bootloader, firmware, partitions, and ELF file
if [ -f "$BUILD_DIR/bootloader.bin" ]; then
    cp "$BUILD_DIR/bootloader.bin" "$VERSION_DIR/$BOOTLOADER_BIN_NAME"
    echo "[POST-BUILD] Copied bootloader.bin to $BOARD_VARIANT_LABEL/$VERSION_DIR_NAME/$BOOTLOADER_BIN_NAME"
fi

if [ -f "$BUILD_DIR/firmware.bin" ]; then
    cp "$BUILD_DIR/firmware.bin" "$VERSION_DIR/$FIRMWARE_BIN_NAME"
    echo "[POST-BUILD] Copied firmware.bin to $BOARD_VARIANT_LABEL/$VERSION_DIR_NAME/$FIRMWARE_BIN_NAME"
fi

if [ -f "$BUILD_DIR/partitions.bin" ]; then
    cp "$BUILD_DIR/partitions.bin" "$VERSION_DIR/$PARTITIONS_BIN_NAME"
    echo "[POST-BUILD] Copied partitions.bin to $BOARD_VARIANT_LABEL/$VERSION_DIR_NAME/$PARTITIONS_BIN_NAME"
fi

if [ -f "$BUILD_DIR/firmware.elf" ]; then
    cp "$BUILD_DIR/firmware.elf" "$VERSION_DIR/$FIRMWARE_ELF_NAME"
    echo "[POST-BUILD] Copied firmware.elf to $BOARD_VARIANT_LABEL/$VERSION_DIR_NAME/$FIRMWARE_ELF_NAME"
fi

# Create a flash script for easy esptool flashing
FLASH_SCRIPT="$VERSION_DIR/flash.sh"
cat > "$FLASH_SCRIPT" << FLASHEOF
#!/bin/bash
# Flash script for ESP32-S3 Simple Thermostat
# Usage: ./flash.sh [port]
# Default port: /dev/ttyACM0

PORT=${1:-/dev/ttyACM0}
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BOOTLOADER_FILE="$BOOTLOADER_BIN_NAME"
PARTITIONS_FILE="$PARTITIONS_BIN_NAME"
FIRMWARE_FILE="$FIRMWARE_BIN_NAME"

echo "[FLASH] Using port: $PORT"
echo "[FLASH] Flashing ESP32-S3 from ELF file..."

esptool.py --chip esp32s3 --port "$PORT" --baud 460800 --before default_reset --after hard_reset write_flash -z \
    --flash_mode dio --flash_freq 80m --flash_size 16MB \
    0x0 "$SCRIPT_DIR/$BOOTLOADER_FILE" \
    0x8000 "$SCRIPT_DIR/$PARTITIONS_FILE" \
    0xe000 "$BOOT_APP0" \
    0x10000 "$SCRIPT_DIR/$FIRMWARE_FILE"

if [ $? -eq 0 ]; then
    echo "[FLASH] Successfully flashed!"
else
    echo "[FLASH] Flashing failed!"
    exit 1
fi
FLASHEOF

chmod +x "$FLASH_SCRIPT"
echo "[POST-BUILD] Created flash.sh script"

# Update latest symlinks
cd "$VARIANT_FIRMWARE_DIR"
rm -f latest_bootloader.bin latest_firmware.bin latest_firmware.elf latest_partitions.bin latest_flash.sh
ln -s "$VERSION_DIR_NAME/$BOOTLOADER_BIN_NAME" latest_bootloader.bin
ln -s "$VERSION_DIR_NAME/$FIRMWARE_BIN_NAME" latest_firmware.bin
ln -s "$VERSION_DIR_NAME/$FIRMWARE_ELF_NAME" latest_firmware.elf
ln -s "$VERSION_DIR_NAME/$PARTITIONS_BIN_NAME" latest_partitions.bin
ln -s "$VERSION_DIR_NAME/flash.sh" latest_flash.sh

cd "$FIRMWARE_DIR"
rm -f latest_bootloader.bin latest_firmware.bin latest_firmware.elf latest_partitions.bin latest_flash.sh
ln -s "$BOARD_VARIANT_LABEL/latest_bootloader.bin" latest_bootloader.bin
ln -s "$BOARD_VARIANT_LABEL/latest_firmware.bin" latest_firmware.bin
ln -s "$BOARD_VARIANT_LABEL/latest_firmware.elf" latest_firmware.elf
ln -s "$BOARD_VARIANT_LABEL/latest_partitions.bin" latest_partitions.bin
ln -s "$BOARD_VARIANT_LABEL/latest_flash.sh" latest_flash.sh

echo "[POST-BUILD] Firmware files organized in: firmware/$BOARD_VARIANT_LABEL/$VERSION_DIR_NAME"

# Prune old firmware builds, keeping the newest $KEEP_FIRMWARE builds
cd "$VARIANT_FIRMWARE_DIR"
mapfile -t BUILD_DIRS < <(ls -1dt build_* 2>/dev/null)
if [[ ${#BUILD_DIRS[@]} -gt $KEEP_FIRMWARE ]]; then
    for OLD_DIR in "${BUILD_DIRS[@]:$KEEP_FIRMWARE}"; do
        rm -rf "$OLD_DIR"
        echo "[POST-BUILD] Removed old firmware build: $OLD_DIR"
    done
fi

# Return to project dir
cd "$PROJECT_DIR"
