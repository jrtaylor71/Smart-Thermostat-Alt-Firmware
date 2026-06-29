#!/bin/bash
# Flash script for ESP32-S3 Simple Thermostat
# Usage: ./flash.sh [port]
# Default port: /dev/ttyACM0

PORT=/dev/ttyACM0
SCRIPT_DIR="/home/jonnt/Documents/Smart-Thermostat-Alt-Firmware"

BOOTLOADER_FILE="bootloader_v1.5.007_20260629-144856.bin"
PARTITIONS_FILE="partitions_v1.5.007_20260629-144856.bin"
FIRMWARE_FILE="firmware_v1.5.007_20260629-144856.bin"

echo "[FLASH] Using port: "
echo "[FLASH] Flashing ESP32-S3 from ELF file..."

esptool.py --chip esp32s3 --port "" --baud 460800 --before default_reset --after hard_reset write_flash -z     --flash_mode dio --flash_freq 80m --flash_size 16MB     0x0 "/"     0x8000 "/"     0xe000 ""     0x10000 "/"

if [ 0 -eq 0 ]; then
    echo "[FLASH] Successfully flashed!"
else
    echo "[FLASH] Flashing failed!"
    exit 1
fi
