# ESP32-S3 Thermostat Backup Files

This directory contains backup versions of the thermostat code during ESP32-S3 migration.

## Files

### src/Main-Thermostat-Original.cpp
- **Purpose**: Complete original thermostat functionality (2292 lines)
- **Hardware**: ESP32 WROOM-32 with DHT11 sensor
- **Features**: Full web server, MQTT, multi-stage HVAC, hydronic heating, touch interface
- **Status**: Original working code before ESP32-S3 migration
- **Libraries**: DHT11, AsyncWebServer, PubSubClient, ArduinoJson, OTA updates

### src/Main-Thermostat-Test-Working.cpp  
- **Purpose**: ESP32-S3 hardware validation and display test code
- **Hardware**: ESP32-S3-WROOM-1-N16 with AHT20 sensor
- **Features**: Display functionality, backlight control, diagnostic UART, sensor testing
- **Status**: WORKING - Proven ESP32-S3 hardware configuration
- **Libraries**: Basic sensors, TFT_eSPI with custom ESP32-S3 pin configuration
- **Key Success**: GPIO14 backlight, diagnostic UART (TX=43,RX=44), smart-thermostat pin layout

## Hardware Configuration Changes

### Original ESP32 WROOM-32:
- DHT11 sensor on GPIO22
- DS18B20 on GPIO27
- Standard ESP32 relay pins

### ESP32-S3 Smart-Thermostat:
- AHT20 sensor via I2C (SDA=8, SCL=9)
- DS18B20 on GPIO34 (confirmed working)  
- TFT Display: MISO=21, MOSI=12, SCLK=13, CS=9, DC=11, RST=10, BL=14
- Touch: CS=3 (conditional compilation for ESP32-S3)
- Relays: Heat1=5, Heat2=6, Cool1=7, Cool2=15, Fan=16
- Diagnostic UART: TX=43, RX=44 for /dev/ttyUSB0 monitoring

## Migration Notes

1. **Display**: TFT_eSPI required custom ESP32-S3 pin configuration via build flags
2. **Touch Screen**: ESP32-S3 needs conditional compilation due to library compatibility
3. **Sensors**: DHT11 → AHT20 migration completed
4. **Diagnostic**: Serial1 UART support added for hardware debugging
5. **Pin Layout**: Smart-thermostat hardware configuration validated and working

## Build Configuration

See `/platformio.ini` for:
- ESP32-S3 board configuration (esp32-s3-thermostat)
- TFT_eSPI build flags with custom pin assignments
- Library dependencies for full thermostat functionality
- Touch screen conditional compilation support