#!/bin/bash
# Build wrapper script - builds and organizes firmware
# Usage: ./build.sh [clean] [-k|--keep <count>]

cd "$(dirname "$0")"

KEEP_COUNT=1
DO_CLEAN=0

while [[ $# -gt 0 ]]; do
    case "$1" in
        clean)
            DO_CLEAN=1
            shift
            ;;
        -k|--keep)
            KEEP_COUNT="$2"
            shift 2
            ;;
        *)
            echo "[BUILD] Unknown argument: $1"
            echo "[BUILD] Usage: ./build.sh [clean] [-k|--keep <count>]"
            exit 1
            ;;
    esac
done

if [ "$DO_CLEAN" -eq 1 ]; then
    echo "[BUILD] Cleaning..."
    pio run --target clean
fi

echo "[BUILD] Building firmware..."
pio run

if [ $? -eq 0 ]; then
    echo "[BUILD] Build successful, organizing firmware (keep $KEEP_COUNT)..."
    KEEP_FIRMWARE="$KEEP_COUNT" ./organize_firmware.sh --keep "$KEEP_COUNT"
else
    echo "[BUILD] Build failed!"
    exit 1
fi
