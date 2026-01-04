/*
 * HardwarePins.h - Hardware pin definitions for ESP32-S3 Thermostat
 * 
 * Central location for all GPIO pin assignments
 * ESP32-S3-WROOM-1 variant with 16MB flash
 */

#ifndef HARDWARE_PINS_H
#define HARDWARE_PINS_H

// ============================================================================
// BOOT AND RESET BUTTONS
// ============================================================================
#define BOOT_BUTTON 0  // GPIO0 - Boot button for factory reset (10 second hold)

// ============================================================================
// MOTION SENSOR (LD2410)
// ============================================================================
#define LD2410_RX_PIN 16   // UART2 RX (connect to LD2410 TX)
#define LD2410_TX_PIN 15   // UART2 TX (connect to LD2410 RX)
#define LD2410_MOTION_PIN 18  // Digital motion output pin (HIGH when motion detected)

// ============================================================================
// TEMPERATURE SENSORS
// ============================================================================
#define ONE_WIRE_BUS 34    // 1-Wire bus for DS18B20 hydronic temperature sensor

// ============================================================================
// DISPLAY (TFT_eSPI)
// ============================================================================
// SPI pins for TFT display (configured in TFT_Setup_ESP32_S3_Thermostat.h)
#define TFT_BACKLIGHT_PIN 14  // GPIO14 - PWM control for TFT backlight brightness

// ============================================================================
// LIGHT SENSOR
// ============================================================================
#define LIGHT_SENSOR_PIN 8    // ADC pin for photocell/light sensor

// ============================================================================
// RELAY CONTROL PINS (HVAC Outputs)
// ============================================================================
// Heat pump/heating relays
const int heatRelay1Pin = 5;    // GPIO5  - Heat Stage 1 (primary heating)
const int heatRelay2Pin = 7;    // GPIO7  - Heat Stage 2 (auxiliary/2nd stage heating)

// Cooling relays
const int coolRelay1Pin = 6;    // GPIO6  - Cool Stage 1 (primary cooling)
const int coolRelay2Pin = 39;   // GPIO39 - Cool Stage 2 (auxiliary/2nd stage cooling)

// Fan control
const int fanRelayPin = 4;      // GPIO4  - Fan relay (continuous fan operation)
// Pump control
const int pumpRelayPin = 40;    // GPIO40 - Pump relay (for hydronic heating systems)

// ============================================================================
// STATUS LED PINS (Visual Feedback)
// ============================================================================
const int ledFanPin = 37;       // GPIO37 - Fan status LED (green)
const int ledHeatPin = 38;      // GPIO38 - Heat status LED (red)
const int ledCoolPin = 2;       // GPIO2  - Cool status LED (blue)

// ============================================================================
// AUDIO OUTPUT
// ============================================================================
const int buzzerPin = 17;       // GPIO17 - Buzzer (5V through 2N7002 MOSFET)

// ============================================================================
// COMPATIBILITY MACROS (for unified codebase)
// ============================================================================
// Relay control pins - uppercase macro names for compatibility with main code
#define HEAT_RELAY_1_PIN heatRelay1Pin
#define HEAT_RELAY_2_PIN heatRelay2Pin
#define COOL_RELAY_1_PIN coolRelay1Pin
#define COOL_RELAY_2_PIN coolRelay2Pin
#define FAN_RELAY_PIN fanRelayPin
#define PUMP_RELAY_PIN pumpRelayPin

// Status LED pins - uppercase macro names
#define LED_FAN_PIN ledFanPin
#define LED_HEAT_PIN ledHeatPin
#define LED_COOL_PIN ledCoolPin

// Buzzer pin - uppercase macro name
#define BUZZER_PIN buzzerPin

// I2C pins for sensor communication (not used in Alt - only AHT20 which uses default I2C)
// These are defined for compatibility with copied main code
#define I2C_SDA_PIN 36  // Not physically connected in Alt hardware
#define I2C_SCL_PIN 35  // Not physically connected in Alt hardware

// OneWire pin for DS18B20 (maps to ONE_WIRE_BUS)
#define ONEWIRE_PIN ONE_WIRE_BUS

// ============================================================================
// PWM CONFIGURATION
// ============================================================================
// PWM channels for various outputs
const int PWM_CHANNEL = 0;            // Channel 0 - TFT backlight brightness
const int PWM_CHANNEL_HEAT = 1;       // Channel 1 - Heat LED brightness
const int PWM_CHANNEL_COOL = 2;       // Channel 2 - Cool LED brightness
const int PWM_CHANNEL_FAN = 3;        // Channel 3 - Fan LED brightness
const int PWM_CHANNEL_BUZZER = 4;     // Channel 4 - Buzzer PWM (tone generation)

// PWM frequency and resolution
const int PWM_FREQ = 5000;            // 5 kHz frequency
const int PWM_RESOLUTION = 8;         // 8-bit resolution (0-255)

// ============================================================================
// BRIGHTNESS CONTROL (Display Backlight)
// ============================================================================
const int MIN_BRIGHTNESS = 30;        // Minimum backlight brightness (0-255)
const int MAX_BRIGHTNESS = 255;       // Maximum backlight brightness (0-255)

// Light sensor thresholds for automatic brightness control
const int LIGHT_SENSOR_MIN = 100;     // Minimum useful light sensor reading
const int LIGHT_SENSOR_MAX = 3500;    // Maximum useful light sensor reading

// ============================================================================
// TIMING CONSTANTS
// ============================================================================
const unsigned long BRIGHTNESS_UPDATE_INTERVAL = 1000;  // Update brightness every 1 second
const unsigned long FACTORY_RESET_PRESS_TIME = 10000;   // 10 seconds for factory reset

// ============================================================================
// PROJECT NAMING AND BRANDING
// ============================================================================
#define PROJECT_NAME_SHORT "Smart Thermostat Alt Firmware"
#define UI_PRODUCT_LINE "Smart Thermostat Alt Firmware"
#define DEFAULT_HOSTNAME "Smart-Thermostat"
#define MQTT_MANUFACTURER "Tah Der"
#define MQTT_MODEL "Truly Smart Thermostat"

#endif // HARDWARE_PINS_H
