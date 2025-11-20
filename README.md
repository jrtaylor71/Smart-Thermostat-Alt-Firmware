# ESP32 Smart Thermostat - Alternative Firmware

A comprehensive, feature-rich smart thermostat system built on the ESP32 platform with professional PCB design. Perfect for DIY smart home automation with full Home Assistant integration.

## 🏗️ Hardware Credits

This project is **alternative firmware** for the excellent smart-thermostat hardware designed by **Stefan Meisner**.

**Original Hardware Project**: https://github.com/smeisner/smart-thermostat

We use Stefan's hardware design and provide an enhanced firmware implementation with advanced features including multi-core architecture, centralized display management, and comprehensive Home Assistant integration.

![Thermostat Display](https://github.com/jrtaylor71/ESP32-Simple-Thermostat/blob/main/img/IMG_20250501_151129758.png)

## 🌟 Key Features

- **📱 Local Touch Control**: ILI9341 TFT LCD with intuitive touch interface
- **🏠 Smart Home Ready**: Full MQTT integration with Home Assistant auto-discovery
- **🌡️ Dual Sensors**: DHT11 for ambient conditions + DS18B20 for hydronic systems
- **⚡ Multi-Stage HVAC**: Support for 2-stage heating and cooling systems
- **💨 Advanced Fan Control**: Auto, continuous, and scheduled cycling modes
- **🌐 Web Interface**: Complete web-based configuration and monitoring
- **📡 Offline Operation**: Full functionality without WiFi connection
- **🔧 Professional PCB**: Custom PCB design for clean, permanent installation
- **🔄 OTA Updates**: Over-the-air firmware updates
- **🔒 Factory Reset**: Built-in reset capability via boot button

## 🚀 Quick Start

### Hardware Requirements
- ESP32-S3-WROOM-1-N16 (16MB Flash, No PSRAM) 
- ILI9341 320x240 TFT LCD with XPT2046 Touch Controller
- AHT20 Temperature/Humidity Sensor (I2C)
- DS18B20 Temperature Sensor (optional, for hydronic heating)
- 5x Relay Module for HVAC control
- Custom PCB by Stefan Meisner (files included)

### Software Setup
1. Install [PlatformIO](https://platformio.org/) IDE
2. Clone this repository
3. Open project in PlatformIO
4. Configuration optimized for ESP32-S3-WROOM-1-N16 (16MB flash)
5. Flash utilization: 30.3% (3.14MB available with huge_app.csv partition)
6. Build and upload to ESP32-S3
7. Use touch interface to configure WiFi and settings

### Pin Configuration (ESP32-S3)

#### TFT Display (ILI9341 + XPT2046 Touch)
```cpp
#define TFT_MISO 21    // SPI MISO (shared with touch)
#define TFT_MOSI 12    // SPI MOSI (shared with touch)
#define TFT_SCLK 13    // SPI Clock (shared with touch)
#define TFT_CS    9    // TFT Chip Select
#define TFT_DC   11    // Data/Command
#define TFT_RST  10    // Reset
#define TFT_BL   14    // Backlight
#define TOUCH_CS 47    // Touch Chip Select
#define TOUCH_IRQ 48   // Touch Interrupt
```

#### Sensors
```cpp
#define AHT20_SDA 36        // I2C SDA for AHT20
#define AHT20_SCL 35        // I2C SCL for AHT20  
#define ONE_WIRE_BUS 34     // DS18B20 data pin (optional)
#define LIGHT_SENSOR 8      // Light sensor
```

#### Relay Outputs
```cpp
const int heatRelay1Pin = 5;    // Stage 1 heating
const int heatRelay2Pin = 7;    // Stage 2 heating
const int coolRelay1Pin = 6;    // Stage 1 cooling
const int coolRelay2Pin = 39;   // Stage 2 cooling
const int fanRelayPin = 4;      // Fan control
```

## 💻 Web Interface

Access the thermostat's web interface by navigating to its IP address:

### Status Page
![Web Status](https://github.com/jrtaylor71/ESP32-Simple-Thermostat/blob/main/img/web-status.png)

Real-time monitoring of:
- Current temperature and humidity
- Thermostat and fan modes
- Relay states
- System status

### Settings Page
![Web Settings](https://github.com/jrtaylor71/ESP32-Simple-Thermostat/blob/main/img/web-settings.png)

Complete configuration interface for:
- Temperature setpoints and control modes
- MQTT/Home Assistant integration
- WiFi network settings
- Multi-stage HVAC parameters
- Hydronic heating controls
- Fan scheduling options

## 🏠 Home Assistant Integration

Automatic discovery and integration with Home Assistant:

1. Enable MQTT in thermostat settings
2. Configure MQTT broker details
3. Thermostat appears automatically in Home Assistant
4. Full control via Home Assistant interface
5. Supports climate entity with heating/cooling modes

### MQTT Topics
- **Commands**: `esp32_thermostat/mode/set`, `esp32_thermostat/target_temperature/set`
- **Status**: `esp32_thermostat/current_temperature`, `esp32_thermostat/mode`
- **Discovery**: Automatic Home Assistant device discovery

## 🔧 Professional PCB

The project includes a complete custom PCB design:

![PCB Top](https://github.com/jrtaylor71/ESP32-Simple-Thermostat/blob/main/ESP32-Simple-Thermostat-PCB/ESP32-Simple-Thermostat-PCB-top.png)
![PCB Bottom](https://github.com/jrtaylor71/ESP32-Simple-Thermostat/blob/main/ESP32-Simple-Thermostat-PCB/ESP32-Simple-Thermostat-PCB-bottom.png)

### PCB Features
- Professional 4-layer design
- All components integrated on single board
- Labeled terminal blocks for easy HVAC wiring
- Proper grounding and signal integrity
- Standard mounting holes for enclosure installation

### Manufacturing Files
Complete manufacturing package in `ESP32-Simple-Thermostat-PCB/jlcpcb/`:
- Gerber files for PCB fabrication
- Drill files for via and hole placement
- Bill of Materials (BOM) for component sourcing
- Component Placement List (CPL) for assembly

## 📖 Documentation

- **[Complete Documentation](DOCUMENTATION.md)**: Comprehensive technical documentation
- **[Copilot File](.copilot)**: GitHub Copilot context for development
- **Inline Comments**: Extensive code documentation
- **Serial Debug**: Detailed debug output for troubleshooting

## 🛠️ Advanced Features

### Multi-Stage Operation
- Intelligent staging based on time and temperature
- Configurable stage 2 activation parameters
- Prevents system short-cycling
- Optimizes energy efficiency

### Hydronic Heating Support
- DS18B20 water temperature monitoring
- Safety interlocks prevent operation when water is too cold
- Configurable high/low temperature thresholds
- Perfect for radiant floor heating systems

### Fan Control Options
- **Auto**: Fan runs only with heating/cooling
- **On**: Continuous fan operation
- **Cycle**: Scheduled fan operation (configurable minutes per hour)

### Safety Features
- Watchdog timer prevents system lockups
- Factory reset via boot button (10+ seconds)
- Temperature limit enforcement
- Graceful offline operation

## 🔧 Factory Reset

Press and hold the boot button for more than 10 seconds while the thermostat is running to restore all settings to defaults.

## 📄 License

This project is released under the GNU General Public License v3.0. Free to use, modify, and distribute.

## 🤝 Contributing

Contributions welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## 📞 Support

- **Issues**: Use GitHub Issues for bug reports
- **Discussions**: GitHub Discussions for questions
- **Documentation**: Comprehensive docs included
- **Serial Debug**: Detailed logging at 115200 baud

## ⭐ Version

**Current Version**: 1.0.7
- ESP32-S3-WROOM-1-N16 platform with 16MB flash optimization
- Modern Material Design color scheme with enhanced readability
- Complete thermostat functionality with Option C display system
- Enhanced MQTT/Home Assistant integration with temperature precision
- Professional PCB design (hardware by Stefan Meisner)
- Multi-stage HVAC support with intelligent staging
- Web interface and OTA updates
- Dual-core FreeRTOS architecture for ESP32-S3

## 🙏 Credits & Acknowledgments

**Hardware Design**: Stefan Meisner - [smart-thermostat](https://github.com/smeisner/smart-thermostat)  
**Alternative Firmware**: Jonn Taylor - Enhanced firmware implementation

This project demonstrates the power of open-source collaboration - combining excellent hardware design with advanced firmware capabilities.

---

**Created for the DIY smart home community**
