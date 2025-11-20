#ifndef WEBPAGES_H
#define WEBPAGES_H

#include "WebInterface.h"

// Generate modern status page HTML
String generateStatusPage(float currentTemp, float currentHumidity, float hydronicTemp, 
                         String thermostatMode, String fanMode, String version_info, 
                         String hostname, bool useFahrenheit, bool hydronicHeatingEnabled,
                         int heatRelay1Pin, int heatRelay2Pin, int coolRelay1Pin, 
                         int coolRelay2Pin, int fanRelayPin) {
    
    String html = "<!DOCTYPE html><html lang='en'><head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Smart Thermostat - Status</title>";
    html += CSS_STYLES;
    html += "</head><body>";
    
    html += "<div class='container'>";
    
    // Header
    html += "<div class='header'>";
    html += "<h1>Smart Thermostat Alt Firmware</h1>";
    html += "<div class='version'>Version " + version_info + " • " + hostname + "</div>";
    html += "</div>";
    
    // Navigation tabs
    html += "<div class='nav-tabs'>";
    html += "<button class='nav-tab active' onclick='showTab(\"status\")'>Status</button>";
    html += "<button class='nav-tab' onclick='showTab(\"settings\")'>Settings</button>";
    html += "<button class='nav-tab' onclick='showTab(\"system\")'>System</button>";
    html += "</div>";
    
    // Status tab content
    html += "<div id='status-content' class='tab-content content'>";
    
    // Main temperature display
    html += "<div class='status-card' style='text-align: center; margin-bottom: 24px;'>";
    html += "<div class='card-header'>";
    html += ICON_TEMPERATURE;
    html += "<h2 class='card-title'>Current Temperature</h2>";
    html += "</div>";
    html += "<div class='temp-display'>" + String(currentTemp, 1) + "<span class='temp-unit'>&deg;" + String(useFahrenheit ? "F" : "C") + "</span></div>";
    html += "</div>";
    
    // Status grid
    html += "<div class='status-grid'>";
    
    // Humidity card
    html += "<div class='status-card'>";
    html += "<div class='card-header'>";
    html += ICON_HUMIDITY;
    html += "<h3 class='card-title'>Humidity</h3>";
    html += "</div>";
    html += "<div style='text-align: center; font-size: 2rem; color: var(--secondary-color);'>";
    html += String(currentHumidity, 1) + "<span style='font-size: 1rem; opacity: 0.7;'>%</span></div>";
    html += "</div>";
    
    // Thermostat mode card
    html += "<div class='status-card'>";
    html += "<div class='card-header'>";
    html += ICON_THERMOSTAT;
    html += "<h3 class='card-title'>Thermostat Mode</h3>";
    html += "</div>";
    html += "<div style='text-align: center; margin: 16px 0;'>";
    String modeClass = "status-";
    if (thermostatMode == "off") modeClass += "off";
    else if (thermostatMode == "auto") modeClass += "auto";
    else modeClass += "on";
    html += "<span class='status-indicator " + modeClass + "'>" + thermostatMode + "</span>";
    html += "</div>";
    html += "<div style='text-align: center; font-size: 0.9rem; opacity: 0.7;'>Fan: " + fanMode + "</div>";
    html += "</div>";
    
    // Hydronic temperature (if enabled)
    if (hydronicHeatingEnabled) {
        html += "<div class='status-card'>";
        html += "<div class='card-header'>";
        html += ICON_TEMPERATURE;
        html += "<h3 class='card-title'>Hydronic Temperature</h3>";
        html += "</div>";
        html += "<div style='text-align: center; font-size: 2rem; color: var(--warning);'>";
        html += String(hydronicTemp, 1) + "<span style='font-size: 1rem; opacity: 0.7;'>&deg;F</span></div>";
        html += "</div>";
    }
    
    html += "</div>"; // End status-grid
    
    // System status section
    html += "<div class='status-card'>";
    html += "<div class='card-header'>";
    html += ICON_RELAY;
    html += "<h3 class='card-title'>System Status</h3>";
    html += "</div>";
    html += "<div class='system-status'>";
    
    // Relay statuses
    bool heat1Status = digitalRead(heatRelay1Pin);
    bool heat2Status = digitalRead(heatRelay2Pin);
    bool cool1Status = digitalRead(coolRelay1Pin);
    bool cool2Status = digitalRead(coolRelay2Pin);
    bool fanStatus = digitalRead(fanRelayPin);
    
    html += "<div class='relay-status" + String(heat1Status ? " active" : "") + "'>";
    html += "<span>Heat Stage 1</span><span class='status-indicator " + String(heat1Status ? "status-on" : "status-off") + "'>" + String(heat1Status ? "ON" : "OFF") + "</span>";
    html += "</div>";
    
    html += "<div class='relay-status" + String(heat2Status ? " active" : "") + "'>";
    html += "<span>Heat Stage 2</span><span class='status-indicator " + String(heat2Status ? "status-on" : "status-off") + "'>" + String(heat2Status ? "ON" : "OFF") + "</span>";
    html += "</div>";
    
    html += "<div class='relay-status" + String(cool1Status ? " active" : "") + "'>";
    html += "<span>Cool Stage 1</span><span class='status-indicator " + String(cool1Status ? "status-on" : "status-off") + "'>" + String(cool1Status ? "ON" : "OFF") + "</span>";
    html += "</div>";
    
    html += "<div class='relay-status" + String(cool2Status ? " active" : "") + "'>";
    html += "<span>Cool Stage 2</span><span class='status-indicator " + String(cool2Status ? "status-on" : "status-off") + "'>" + String(cool2Status ? "ON" : "OFF") + "</span>";
    html += "</div>";
    
    html += "<div class='relay-status" + String(fanStatus ? " active" : "") + "'>";
    html += "<span>Fan</span><span class='status-indicator " + String(fanStatus ? "status-on" : "status-off") + "'>" + String(fanStatus ? "ON" : "OFF") + "</span>";
    html += "</div>";
    
    html += "</div>"; // End system-status
    html += "</div>"; // End status-card
    
    html += "</div>"; // End status-content tab
    
    // Settings tab content (placeholder)
    html += "<div id='settings-content' class='tab-content content' style='display: none;'>";
    html += "<div class='status-card'>";
    html += "<h3>Settings</h3>";
    html += "<p>Settings interface will be loaded here...</p>";
    html += "<div class='button-group'>";
    html += "<a href='/settings' class='btn btn-primary'>Open Settings</a>";
    html += "</div>";
    html += "</div>";
    html += "</div>";
    
    // System tab content
    html += "<div id='system-content' class='tab-content content' style='display: none;'>";
    html += "<div class='status-card'>";
    html += "<div class='card-header'>";
    html += ICON_SETTINGS;
    html += "<h3 class='card-title'>System Management</h3>";
    html += "</div>";
    html += "<p>Manage system functions and maintenance tasks.</p>";
    html += "<div class='button-group'>";
    html += "<a href='/update' class='btn btn-primary'>OTA Update</a>";
    html += "<button onclick='confirmAction(\"reboot the system\", \"/reboot\")' class='btn btn-warning'>Reboot</button>";
    html += "<a href='/confirm_restore' class='btn btn-danger'>Factory Reset</a>";
    html += "</div>";
    html += "</div>";
    html += "</div>";
    
    html += "</div>"; // End container
    html += JAVASCRIPT_CODE;
    html += "</body></html>";
    
    return html;
}

// Generate modern settings page HTML
String generateSettingsPage(String thermostatMode, String fanMode, float setTempHeat, 
                           float setTempCool, float setTempAuto, float tempSwing, 
                           float autoTempSwing, bool fanRelayNeeded, bool useFahrenheit,
                           bool mqttEnabled, int stage1MinRuntime, float stage2TempDelta,
                           bool stage2HeatingEnabled, bool stage2CoolingEnabled,
                           bool hydronicHeatingEnabled, float hydronicTempLow, 
                           float hydronicTempHigh, int fanMinutesPerHour,
                           String mqttServer, int mqttPort, String mqttUsername,
                           String mqttPassword, String wifiSSID, String wifiPassword,
                           String hostname, bool use24HourClock, String timeZone,
                           float tempOffset, float humidityOffset, bool displaySleepEnabled,
                           unsigned long displaySleepTimeout) {
    
    String html = "<!DOCTYPE html><html lang='en'><head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Smart Thermostat - Settings</title>";
    html += CSS_STYLES;
    html += "</head><body>";
    
    html += "<div class='container'>";
    
    // Header
    html += "<div class='header'>";
    html += "<h1>Thermostat Settings</h1>";
    html += "<div class='version'>Configure your smart thermostat</div>";
    html += "</div>";
    
    html += "<div class='content'>";
    
    // Settings form
    html += "<form action='/set' method='POST'>";
    
    // Basic Settings Section
    html += "<div class='settings-section'>";
    html += "<h3>Basic Settings</h3>";
    
    html += "<div class='form-group'>";
    html += "<label class='form-label'>Thermostat Mode</label>";
    html += "<select name='thermostatMode' class='form-select'>";
    html += "<option value='off'" + String(thermostatMode == "off" ? " selected" : "") + ">Off</option>";
    html += "<option value='heat'" + String(thermostatMode == "heat" ? " selected" : "") + ">Heat</option>";
    html += "<option value='cool'" + String(thermostatMode == "cool" ? " selected" : "") + ">Cool</option>";
    html += "<option value='auto'" + String(thermostatMode == "auto" ? " selected" : "") + ">Auto</option>";
    html += "</select>";
    html += "</div>";
    
    html += "<div class='form-group'>";
    html += "<label class='form-label'>Fan Mode</label>";
    html += "<select name='fanMode' class='form-select'>";
    html += "<option value='auto'" + String(fanMode == "auto" ? " selected" : "") + ">Auto</option>";
    html += "<option value='on'" + String(fanMode == "on" ? " selected" : "") + ">On</option>";
    html += "<option value='cycle'" + String(fanMode == "cycle" ? " selected" : "") + ">Cycle</option>";
    html += "</select>";
    html += "</div>";
    
    html += "<div style='display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 16px;'>";
    
    html += "<div class='form-group'>";
    html += "<label class='form-label'>Heat Setpoint</label>";
    html += "<input type='number' name='setTempHeat' value='" + String(setTempHeat, 1) + "' step='0.5' class='form-input'>";
    html += "</div>";
    
    html += "<div class='form-group'>";
    html += "<label class='form-label'>Cool Setpoint</label>";
    html += "<input type='number' name='setTempCool' value='" + String(setTempCool, 1) + "' step='0.5' class='form-input'>";
    html += "</div>";
    
    html += "<div class='form-group'>";
    html += "<label class='form-label'>Auto Setpoint</label>";
    html += "<input type='number' name='setTempAuto' value='" + String(setTempAuto, 1) + "' step='0.5' class='form-input'>";
    html += "</div>";
    
    html += "</div>"; // End grid
    
    html += "<div style='display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 16px;'>";
    
    html += "<div class='form-group'>";
    html += "<label class='form-label'>Temperature Swing</label>";
    html += "<input type='number' name='tempSwing' value='" + String(tempSwing, 1) + "' step='0.1' class='form-input'>";
    html += "</div>";
    
    html += "<div class='form-group'>";
    html += "<label class='form-label'>Auto Temp Swing</label>";
    html += "<input type='number' name='autoTempSwing' value='" + String(autoTempSwing, 1) + "' step='0.1' class='form-input'>";
    html += "</div>";
    
    html += "</div>"; // End grid
    
    html += "<div class='form-checkbox'>";
    html += "<input type='checkbox' name='fanRelayNeeded' " + String(fanRelayNeeded ? "checked" : "") + ">";
    html += "<label class='form-label'>Fan Relay Required</label>";
    html += "</div>";
    
    html += "<div class='form-checkbox'>";
    html += "<input type='checkbox' name='useFahrenheit' " + String(useFahrenheit ? "checked" : "") + ">";
    html += "<label class='form-label'>Use Fahrenheit</label>";
    html += "</div>";
    
    html += "</div>"; // End basic settings section
    
    // HVAC Advanced Settings
    html += "<div class='settings-section'>";
    html += "<h3>HVAC Advanced Settings</h3>";
    
    html += "<div style='display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 16px;'>";
    
    html += "<div class='form-group'>";
    html += "<label class='form-label'>Stage 1 Min Runtime (seconds)</label>";
    html += "<input type='number' name='stage1MinRuntime' value='" + String(stage1MinRuntime) + "' class='form-input'>";
    html += "</div>";
    
    html += "<div class='form-group'>";
    html += "<label class='form-label'>Stage 2 Temp Delta</label>";
    html += "<input type='number' name='stage2TempDelta' value='" + String(stage2TempDelta, 1) + "' step='0.1' class='form-input'>";
    html += "</div>";
    
    html += "<div class='form-group'>";
    html += "<label class='form-label'>Fan Minutes Per Hour</label>";
    html += "<input type='number' name='fanMinutesPerHour' value='" + String(fanMinutesPerHour) + "' class='form-input'>";
    html += "</div>";
    
    html += "</div>"; // End grid
    
    html += "<div class='form-checkbox'>";
    html += "<input type='checkbox' name='stage2HeatingEnabled' " + String(stage2HeatingEnabled ? "checked" : "") + ">";
    html += "<label class='form-label'>Enable 2nd Stage Heating</label>";
    html += "</div>";
    
    html += "<div class='form-checkbox'>";
    html += "<input type='checkbox' name='stage2CoolingEnabled' " + String(stage2CoolingEnabled ? "checked" : "") + ">";
    html += "<label class='form-label'>Enable 2nd Stage Cooling</label>";
    html += "</div>";
    
    html += "<div class='form-checkbox'>";
    html += "<input type='checkbox' name='hydronicHeatingEnabled' " + String(hydronicHeatingEnabled ? "checked" : "") + ">";
    html += "<label class='form-label'>Hydronic Heating Enabled</label>";
    html += "</div>";
    
    if (hydronicHeatingEnabled) {
        html += "<div style='display: grid; grid-template-columns: 1fr 1fr; gap: 16px;'>";
        
        html += "<div class='form-group'>";
        html += "<label class='form-label'>Hydronic Temp Low</label>";
        html += "<input type='number' name='hydronicTempLow' value='" + String(hydronicTempLow, 1) + "' step='0.5' class='form-input'>";
        html += "</div>";
        
        html += "<div class='form-group'>";
        html += "<label class='form-label'>Hydronic Temp High</label>";
        html += "<input type='number' name='hydronicTempHigh' value='" + String(hydronicTempHigh, 1) + "' step='0.5' class='form-input'>";
        html += "</div>";
        
        html += "</div>"; // End grid
    }
    
    html += "</div>"; // End HVAC settings section
    
    // The settings page continues but is getting long, so I'll add a "Load More" concept
    // or split into multiple pages. For now, let's add save/navigation buttons
    
    html += "<div class='button-group'>";
    html += "<input type='submit' value='Save Settings' class='btn btn-primary'>";
    html += "<a href='/' class='btn btn-secondary'>Back to Status</a>";
    html += "<a href='/settings_advanced' class='btn btn-secondary'>Advanced Settings</a>";
    html += "</div>";
    
    html += "</form>";
    html += "</div>"; // End content
    html += "</div>"; // End container
    
    html += JAVASCRIPT_CODE;
    html += "</body></html>";
    
    return html;
}

// Generate OTA update page
String generateOTAPage() {
    String html = "<!DOCTYPE html><html lang='en'><head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Smart Thermostat - OTA Update</title>";
    html += CSS_STYLES;
    html += "</head><body>";
    
    html += "<div class='container'>";
    
    html += "<div class='header'>";
    html += "<h1>Over-The-Air Update</h1>";
    html += "<div class='version'>Upload new firmware</div>";
    html += "</div>";
    
    html += "<div class='content'>";
    
    html += "<div class='alert alert-warning'>";
    html += "<strong>Warning:</strong> Do not power off the device during the update process. ";
    html += "The update may take several minutes to complete.";
    html += "</div>";
    
    html += "<div class='settings-section'>";
    html += "<h3>Firmware Upload</h3>";
    html += "<form method='POST' action='/update' enctype='multipart/form-data'>";
    html += "<div class='form-group'>";
    html += "<label class='form-label'>Select Firmware File (.bin)</label>";
    html += "<input type='file' name='update' accept='.bin' class='form-input' style='padding: 8px;'>";
    html += "</div>";
    html += "<div class='button-group'>";
    html += "<input type='submit' value='Upload & Install' class='btn btn-primary' onclick='showUploadProgress()'>";
    html += "<a href='/' class='btn btn-secondary'>Cancel</a>";
    html += "</div>";
    html += "</form>";
    html += "</div>";
    
    html += "<div id='upload-progress' style='display: none;'>";
    html += "<div class='settings-section'>";
    html += "<h3>Uploading...</h3>";
    html += "<div class='progress-bar'>";
    html += "<div class='progress-fill' style='width: 0%;'></div>";
    html += "</div>";
    html += "<p>Please wait while the firmware is being uploaded and installed.</p>";
    html += "</div>";
    html += "</div>";
    
    html += "</div>"; // End content
    html += "</div>"; // End container
    
    html += "<script>";
    html += "function showUploadProgress() {";
    html += "  document.getElementById('upload-progress').style.display = 'block';";
    html += "  // Simulate progress (in real implementation, this would be actual progress)";
    html += "  let progress = 0;";
    html += "  const progressBar = document.querySelector('.progress-fill');";
    html += "  const interval = setInterval(() => {";
    html += "    progress += 2;";
    html += "    progressBar.style.width = progress + '%';";
    html += "    if (progress >= 100) clearInterval(interval);";
    html += "  }, 100);";
    html += "}";
    html += "</script>";
    
    html += "</body></html>";
    
    return html;
}

// Generate factory reset confirmation page
String generateFactoryResetPage() {
    String html = "<!DOCTYPE html><html lang='en'><head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Smart Thermostat - Factory Reset</title>";
    html += CSS_STYLES;
    html += "</head><body>";
    
    html += "<div class='container'>";
    
    html += "<div class='header'>";
    html += "<h1>Factory Reset</h1>";
    html += "<div class='version'>Restore default settings</div>";
    html += "</div>";
    
    html += "<div class='content'>";
    
    html += "<div class='alert alert-error'>";
    html += "<strong>Warning:</strong> This action will permanently delete all your settings ";
    html += "and restore the thermostat to factory defaults. This cannot be undone.";
    html += "</div>";
    
    html += "<div class='settings-section'>";
    html += "<h3>Confirm Factory Reset</h3>";
    html += "<p>The following settings will be reset to defaults:</p>";
    html += "<ul style='margin: 16px 0; padding-left: 24px;'>";
    html += "<li>Temperature setpoints and swing settings</li>";
    html += "<li>HVAC staging configuration</li>";
    html += "<li>WiFi credentials</li>";
    html += "<li>MQTT server settings</li>";
    html += "<li>Display and calibration settings</li>";
    html += "<li>All custom preferences</li>";
    html += "</ul>";
    
    html += "<div class='button-group'>";
    html += "<form action='/restore_defaults' method='POST' style='display: inline;'>";
    html += "<button type='submit' class='btn btn-danger' onclick='return confirm(\"Are you absolutely sure? This cannot be undone!\")'>Yes, Reset Everything</button>";
    html += "</form>";
    html += "<a href='/' class='btn btn-secondary'>Cancel</a>";
    html += "</div>";
    html += "</div>";
    
    html += "</div>"; // End content
    html += "</div>"; // End container
    
    html += "</body></html>";
    
    return html;
}

#endif // WEBPAGES_H