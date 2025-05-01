## ESP32-Simple-Thermostat

ESP32 (Espressif WROOM-32 Developer Kit) based, using a DHT temperature/humidity sensor and ILI 9341 TFT LCD Touch display. Thermostat can be controlled by via MQTT (Home Assistant) or directly by touch on the LCD. Thermostat can operate without WiFi but limited to basic functions. Project is free to use and created in Visual Studio Code with PlatfomIO IDE.

A factory reset can be done by pressing the boot button for more than 10sec once thermostat is running.

## TFT Display is connected via these pins.
```
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    2  // Data Command control pin
#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
#define TOUCH_CS  4  // Chip select pin (T_CS) of touch screen
```
# DHT Sensor
```
#define DHTPIN 22 // Define the pin where the DHT11 is connected
```
# Optional Dallas Sensor for hydronic heating.
```
#define ONE_WIRE_BUS 27 // Define the pin where the DS18B20 is connected
```
# Relays
```
// GPIO pins for relays
const int heatRelay1Pin = 13;
const int heatRelay2Pin = 12;
const int coolRelay1Pin = 14;
const int coolRelay2Pin = 26;
const int fanRelayPin = 25;
```

# Web server
![](https://github.com/jrtaylor71/ESP32-Simple-Thermostat/blob/main/img/web-status.png)
![](https://github.com/jrtaylor71/ESP32-Simple-Thermostat/blob/main/img/web-settings.png)

# Dev board
![](https://github.com/jrtaylor71/ESP32-Simple-Thermostat/blob/main/img/IMG_20250501_151129758.jpg)
