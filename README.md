# Smart Thermostat Alt Firmware

A comprehensive, feature-rich smart thermostat system built on the ESP32 platform with professional PCB design. Perfect for DIY smart home automation with full Home Assistant integration.

## 🏗️ Hardware Credits

This project is **alternative firmware** for the excellent smart-thermostat hardware designed by **Stefan Meisner**.

**Original Hardware Project**: https://github.com/smeisner/smart-thermostat

We use Stefan's hardware design and provide an enhanced firmware implementation with advanced features including multi-core architecture, centralized display management, and comprehensive Home Assistant integration.


## 🌟 Key Features

- **📱 Local Touch Control**: ILI9341 TFT LCD with intuitive touch interface
- **🏠 Smart Home Ready**: Full MQTT integration with Home Assistant auto-discovery
- **📅 7-Day Scheduling**: Comprehensive inline scheduling with day/night periods and editable Heat/Cool/Auto temperatures
- **🌡️ Dual Sensors**: AHT20 for ambient conditions + DS18B20 for hydronic systems
- **⚡ Multi-Stage HVAC**: Support for 2-stage heating and cooling systems
- **💨 Advanced Fan Control**: Auto, continuous, and scheduled cycling modes
- **🌐 Modern Web Interface**: Complete tabbed interface with embedded scheduling - no separate pages
- **📡 Offline Operation**: Full functionality without WiFi connection
- **🔧 Professional PCB**: Custom PCB design for clean, permanent installation
- **🔄 OTA Updates**: Over-the-air firmware updates
- **🔒 Factory Reset**: Built-in reset capability via boot button

## 🚀 Quick Start

![Hardware-Main-Display](img/KIMG20251120_183508010.JPG)

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

## 💻 Web Interface

Access the thermostat's web interface by navigating to its IP address:

### Tabbed Interface with Embedded Features
![Web Status](<img/Screenshot from 2025-11-20 18-39-53.png>)

**Status Tab**: Real-time monitoring of:
- Current temperature and humidity
- Thermostat and fan modes
- Relay states and system status

**Settings Tab**: Complete configuration interface for:
![Web Settings](<img/Screenshot from 2025-11-20 18-40-24.png>)
![Web Settings2](<img/Screenshot from 2025-11-20 18-40-45.png>)
![Web Settings3](<img/Screenshot from 2025-11-20 18-41-20.png>)
- Temperature setpoints and control modes
- MQTT/Home Assistant integration
- WiFi network settings
- Multi-stage HVAC parameters
- Hydronic heating controls
- Fan scheduling options

**Schedule Tab**: Comprehensive 7-day scheduling:
- Day and night periods for each day of the week
- Editable Heat, Cool, and Auto temperatures for each period
- Time controls for period transitions
- Schedule enable/disable and override controls
- All options always visible - no hidden menus

![Schedule](<img/Screenshot from 2025-11-21 11-53-09.png>)
![Schedule1](<img/Screenshot from 2025-11-21 11-54-01.png>)
![Schedule2](<img/Screenshot from 2025-11-21 11-54-33.png>)

**System Tab**: Device information and firmware updates

## 🏠 Home Assistant Integration

Automatic discovery and integration with Home Assistant:

1. Enable MQTT in thermostat settings
2. Configure MQTT broker details
3. Thermostat appears automatically in Home Assistant
4. Full control via Home Assistant interface
5. Supports climate entity with heating/cooling modes

## 🛠️ Advanced Features

![System Tab](<img/Screenshot from 2025-11-20 18-41-44.png>)
![System Tab1](<img/Screenshot from 2025-11-20 18-42-15.png>)

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

**Current Version**: 1.1.0 (November 2025)
- **7-Day Scheduling System**: Complete inline scheduling with day/night periods and editable Heat/Cool/Auto temperatures
- **Modern Tabbed Web Interface**: All features embedded in main page - Status, Settings, Schedule, and System tabs
- **MQTT Alert Improvements**: Non-retained alerts with hysteresis-based reset logic
- **Hydronic Safety Enhancements**: [LOCKOUT] labels and improved temperature monitoring
- ESP32-S3-WROM-1-N16 platform with 16MB flash optimization
- Modern Material Design color scheme with enhanced readability
- Complete thermostat functionality with Option C display system
- Enhanced MQTT/Home Assistant integration with temperature precision
- Professional PCB design (hardware by Stefan Meisner)
- Multi-stage HVAC support with intelligent staging
- Dual-core FreeRTOS architecture for ESP32-S3

## 🙏 Credits & Acknowledgments

**Hardware Design**: Stefan Meisner - [smart-thermostat](https://github.com/smeisner/smart-thermostat)  
**Alternative Firmware**: Jonn Taylor - Enhanced firmware implementation

This project demonstrates the power of open-source collaboration - combining excellent hardware design with advanced firmware capabilities.

---

**Created for the DIY smart home community**
