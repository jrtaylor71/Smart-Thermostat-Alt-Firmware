# Version 1.4.001 Release

**Release Date**: January 24, 2026  
**Status**: Production Ready ✓

## 🎯 Major Features

### Bidirectional MQTT Schedule Synchronization
- **Device → Home Assistant**: Thermostat publishes schedule changes to 7 daily MQTT topics
- **Home Assistant → Device**: HA automations send schedule updates back to device
- **Zero Circular Updates**: Built-in safeguards prevent infinite loops
- **Instant Sync**: 1-2 second latency between changes

### Multi-Thermostat Support
- Unlimited thermostat support with single automation
- Automatic hostname normalization (CamelCase-With-Hyphens ↔ lowercase_underscores)
- Per-device helper isolation
- Centralized multi-device management

### Fixed Critical Bug
- **Off-by-One Day Index**: Friday changes no longer incorrectly update Thursday
- **Root Cause**: Proper day index conversion at MQTT boundaries (MQTT 0=Monday, Firmware 0=Sunday)
- **Solution**: Automatic conversion with `(mqttDay + 1) % 7`

## 🔧 Technical Details

### MQTT Protocol v1.4
- **7 State Topics**: `{hostname}/schedule/sunday` through `{hostname}/schedule/saturday`
- **1 Command Topic**: `{hostname}/schedule/set`
- **Payload Format**: JSON with day_period and night_period structures
- **Day Indexing**: MQTT uses 0=Monday through 6=Sunday

### Home Assistant Integration
- **77 Helpers per Thermostat**: 14 times, 42 temperatures, 21 booleans
- **77 Outbound Automations**: Auto-generated from template
- **1 Inbound Automation**: Centralized multi-device handler
- **91 MQTT Entities**: Per device auto-discovery

### Firmware Implementation
- **Schedule Structures**: SchedulePeriod, DaySchedule (C++ classes)
- **MQTT Handlers**: Proper JSON parsing and validation
- **Day Conversion**: Seamless translation between formats
- **State Publishing**: Automatic topic publishing on schedule changes

## 📦 Files Added

- **MQTT_SCHEDULE_SYNC.md** (571 lines): Complete technical reference with architecture diagrams
- **MQTT_SCHEDULE_QUICKSTART.md** (80 lines): 5-minute setup guide with verification checklist
- **SCHEDULE_SYNC_INDEX.md**: Master documentation index with learning paths
- **DOCUMENTATION_UPDATE.md**: Summary of all documentation changes
- **generate_schedule_package.sh**: Automated package generator for Home Assistant
- **template_thermostat_schedule.yaml**: HA outbound automation template

## 📝 Files Updated

- **README.md**: Added Bidirectional Schedule Sync section (50 lines)
- **USER_MANUAL.md**: Added Schedule Synchronization section (125 lines)
- **DEVELOPMENT_GUIDE.md**: Added MQTT 7-Day Schedule Architecture (200 lines)
- **src/Main-Thermostat.cpp**: Firmware day index conversion fix (critical bug fix)

## ✅ Testing & Verification

- ✓ All 3 firmware variants compile successfully (N8, N16, N32R16V)
- ✓ Bidirectional sync tested in both directions
- ✓ No circular updates or conflicts detected
- ✓ Multi-thermostat operation verified (tested with 3 devices)
- ✓ All 77 automations generate correctly
- ✓ Complete documentation with 3,406+ lines
- ✓ 10+ troubleshooting scenarios documented

## 🚀 Getting Started

### Quick Start (5 minutes)
1. Read [MQTT_SCHEDULE_QUICKSTART.md](MQTT_SCHEDULE_QUICKSTART.md)
2. Follow the 5 numbered setup steps
3. Verify helpers appear in Home Assistant
4. Done!

### Complete Setup (20 minutes)
1. Read [README.md § Home Assistant Integration](https://github.com/jrtaylor71/Smart-Thermostat-Alt-Firmware/blob/main/README.md#-home-assistant-integration)
2. Follow [USER_MANUAL.md § Schedule Synchronization](https://github.com/jrtaylor71/Smart-Thermostat-Alt-Firmware/blob/main/USER_MANUAL.md#schedule-synchronization-)
3. Generate automation package using `generate_schedule_package.sh`
4. Deploy to Home Assistant

### For Developers
- [MQTT_SCHEDULE_SYNC.md](MQTT_SCHEDULE_SYNC.md): Full technical reference
- [DEVELOPMENT_GUIDE.md § MQTT Architecture](https://github.com/jrtaylor71/Smart-Thermostat-Alt-Firmware/blob/main/DEVELOPMENT_GUIDE.md#mqtt-7-day-schedule-architecture-): Code examples and architecture
- [src/Main-Thermostat.cpp](https://github.com/jrtaylor71/Smart-Thermostat-Alt-Firmware/blob/main/src/Main-Thermostat.cpp): Firmware implementation

## 📊 Release Statistics

- **Lines of Code**: 3,406+ lines of new documentation
- **Documentation Files**: 6 markdown files (3 updated, 3 new)
- **API Endpoints**: 7 MQTT state topics + 1 command topic
- **Home Assistant Helpers**: 77 per thermostat
- **Automations**: 77 outbound + 1 inbound (centralized)
- **Time to Setup**: 5 minutes (quick) or 20 minutes (complete)
- **Thermostat Support**: Unlimited

## 🔗 Key Documentation Links

- **Setup Guide**: [MQTT_SCHEDULE_QUICKSTART.md](MQTT_SCHEDULE_QUICKSTART.md)
- **User Manual**: [README.md](https://github.com/jrtaylor71/Smart-Thermostat-Alt-Firmware/blob/main/README.md)
- **Technical Reference**: [MQTT_SCHEDULE_SYNC.md](MQTT_SCHEDULE_SYNC.md)
- **Architecture Guide**: [DEVELOPMENT_GUIDE.md](https://github.com/jrtaylor71/Smart-Thermostat-Alt-Firmware/blob/main/DEVELOPMENT_GUIDE.md)
- **Documentation Index**: [SCHEDULE_SYNC_INDEX.md](SCHEDULE_SYNC_INDEX.md)

## 🛠️ Build & Deploy

### Build Firmware
\`\`\`bash
./build.sh clean all
\`\`\`

### Generate Home Assistant Package
\`\`\`bash
./generate_schedule_package.sh shop_thermostat
./generate_schedule_package.sh studio_thermostat
./generate_schedule_package.sh house_thermostat
\`\`\`

### Deploy to Device
See [README.md § Getting Started](https://github.com/jrtaylor71/Smart-Thermostat-Alt-Firmware/blob/main/README.md#-getting-started) for device flashing instructions.

## ❓ Common Questions

**Q: Will my existing schedules be lost?**  
A: No. Existing schedules are preserved and can be synced to HA using the new bidirectional sync.

**Q: How many thermostats can I manage?**  
A: Unlimited! The system scales horizontally with a single centralized automation.

**Q: What if changes aren't syncing?**  
A: Check [MQTT_SCHEDULE_SYNC.md § Troubleshooting](MQTT_SCHEDULE_SYNC.md#troubleshooting) for 10+ common issues and solutions.

**Q: Do I need to update firmware?**  
A: Yes, this version includes the critical day index bug fix. Update recommended.

## 📋 Known Issues

None at this time. Please report issues on GitHub.

## 🙏 Contributors

- Jonn Taylor (@jrtaylor71)

## 📄 License

GNU General Public License v3.0 - See LICENSE file for details.

---

**Download**: [Get v1.4.001](https://github.com/jrtaylor71/Smart-Thermostat-Alt-Firmware/releases/tag/v1.4.001)
