/*
 * Weather.cpp - Weather integration implementation
 */

#include "Weather.h"

// Color definitions (matching main thermostat colors)
#define COLOR_BACKGROUND   0x1082
#define COLOR_TEXT         0xFFFF
#define COLOR_PRIMARY      0x1976

Weather::Weather() {
    _source = WEATHER_DISABLED;
    _updateInterval = 600000; // Default: 10 minutes
    _useFahrenheit = true;
    _data.valid = false;
    _lastUpdateAttempt = 0;
}

void Weather::begin() {
    _data.valid = false;
    _lastError = "";
}

void Weather::setSource(WeatherSource source) {
    _source = source;
}

void Weather::setOpenWeatherMapConfig(String apiKey, String city, String countryCode) {
    _owmApiKey = apiKey;
    _owmCity = city;
    _owmCountryCode = countryCode;
}

void Weather::setHomeAssistantConfig(String haUrl, String haToken, String entityId) {
    _haUrl = haUrl;
    _haToken = haToken;
    _haEntityId = entityId;
}

void Weather::setUpdateInterval(unsigned long intervalMs) {
    _updateInterval = intervalMs;
}

void Weather::setUseFahrenheit(bool useFahrenheit) {
    _useFahrenheit = useFahrenheit;
}

bool Weather::update() {
    unsigned long currentTime = millis();
    
    // Check if it's time to update
    if (currentTime - _lastUpdateAttempt < _updateInterval) {
        return _data.valid;
    }
    
    _lastUpdateAttempt = currentTime;
    
    if (_source == WEATHER_DISABLED) {
        _lastError = "Weather disabled";
        return false;
    }
    
    bool success = false;
    if (_source == WEATHER_OPENWEATHERMAP) {
        success = updateFromOpenWeatherMap();
    } else if (_source == WEATHER_HOMEASSISTANT) {
        success = updateFromHomeAssistant();
    }
    
    if (success) {
        _data.lastUpdate = currentTime;
    }
    
    return success;
}

bool Weather::updateFromOpenWeatherMap() {
    if (_owmApiKey.isEmpty() || _owmCity.isEmpty()) {
        _lastError = "OpenWeatherMap not configured";
        return false;
    }
    
    HTTPClient http;
    String units = _useFahrenheit ? "imperial" : "metric";
    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + _owmCity;
    if (!_owmCountryCode.isEmpty()) {
        url += "," + _owmCountryCode;
    }
    url += "&appid=" + _owmApiKey + "&units=" + units;
    
    Serial.println("Fetching weather from OpenWeatherMap: " + url);
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode != 200) {
        _lastError = "HTTP error: " + String(httpCode);
        http.end();
        return false;
    }
    
    String payload = http.getString();
    http.end();
    
    // Parse JSON response
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        _lastError = "JSON parse error: " + String(error.c_str());
        return false;
    }
    
    // Extract weather data
    _data.temperature = doc["main"]["temp"];
    _data.tempHigh = doc["main"]["temp_max"];
    _data.tempLow = doc["main"]["temp_min"];
    _data.humidity = doc["main"]["humidity"];
    _data.windSpeed = doc["wind"]["speed"];
    _data.condition = doc["weather"][0]["main"].as<String>();
    _data.description = doc["weather"][0]["description"].as<String>();
    
    String icon = doc["weather"][0]["icon"].as<String>();
    if (icon.length() >= 2) {
        _data.iconCode = icon.substring(0, 2).toInt();
    }
    
    _data.valid = true;
    _lastError = "";
    
    Serial.printf("Weather updated: %.1f%s, %s\n", 
                  _data.temperature, 
                  _useFahrenheit ? "F" : "C",
                  _data.condition.c_str());
    
    return true;
}

bool Weather::updateFromHomeAssistant() {
    if (_haUrl.isEmpty() || _haToken.isEmpty() || _haEntityId.isEmpty()) {
        _lastError = "Home Assistant not configured";
        return false;
    }
    
    HTTPClient http;
    String url = _haUrl + "/api/states/" + _haEntityId;
    
    Serial.println("Fetching weather from Home Assistant: " + url);
    
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + _haToken);
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.GET();
    
    if (httpCode != 200) {
        _lastError = "HTTP error: " + String(httpCode);
        http.end();
        return false;
    }
    
    String payload = http.getString();
    http.end();
    
    // Parse JSON response
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        _lastError = "JSON parse error: " + String(error.c_str());
        return false;
    }
    
    // Extract weather data from Home Assistant entity
    _data.temperature = doc["attributes"]["temperature"];
    _data.humidity = doc["attributes"]["humidity"];
    _data.condition = doc["state"].as<String>();
    
    // Optional attributes (may not be present)
    if (doc["attributes"].containsKey("forecast")) {
        JsonArray forecast = doc["attributes"]["forecast"];
        if (forecast.size() > 0) {
            _data.tempHigh = forecast[0]["temperature"];
            _data.tempLow = forecast[0]["templow"];
        }
    }
    
    if (doc["attributes"].containsKey("wind_speed")) {
        _data.windSpeed = doc["attributes"]["wind_speed"];
    }
    
    _data.description = _data.condition;
    _data.valid = true;
    _lastError = "";
    
    Serial.printf("Weather updated from HA: %.1f%s, %s\n", 
                  _data.temperature, 
                  _useFahrenheit ? "F" : "C",
                  _data.condition.c_str());
    
    return true;
}

WeatherData Weather::getData() {
    return _data;
}

bool Weather::isDataValid() {
    return _data.valid;
}

String Weather::getLastError() {
    return _lastError;
}

void Weather::drawWeatherIcon(TFT_eSPI &tft, int x, int y, String condition) {
    // Simple text-based weather icons
    tft.setTextSize(3);
    tft.setTextColor(COLOR_PRIMARY, COLOR_BACKGROUND);
    
    String icon = "?";
    if (condition.equalsIgnoreCase("Clear") || condition.equalsIgnoreCase("sunny")) {
        icon = "☀";  // Sun
    } else if (condition.equalsIgnoreCase("Clouds") || condition.equalsIgnoreCase("cloudy")) {
        icon = "☁";  // Cloud
    } else if (condition.equalsIgnoreCase("Rain") || condition.equalsIgnoreCase("rainy")) {
        icon = "☂";  // Umbrella/Rain
    } else if (condition.equalsIgnoreCase("Snow") || condition.equalsIgnoreCase("snowy")) {
        icon = "❄";  // Snowflake
    } else if (condition.equalsIgnoreCase("Thunderstorm")) {
        icon = "⚡";  // Lightning
    } else if (condition.equalsIgnoreCase("Fog") || condition.equalsIgnoreCase("Mist")) {
        icon = "☃";  // Fog
    }
    
    tft.setCursor(x, y);
    tft.print(icon);
}

void Weather::displayOnTFT(TFT_eSPI &tft, int x, int y, bool useFahrenheit) {
    if (!_data.valid) {
        return;
    }
    
    // Clear the display area (adjust size as needed)
    tft.fillRect(x, y, 100, 40, COLOR_BACKGROUND);
    
    // Draw weather icon
    drawWeatherIcon(tft, x, y, _data.condition);
    
    // Draw temperature
    tft.setTextSize(2);
    tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
    tft.setCursor(x + 30, y + 5);
    char tempStr[8];
    dtostrf(_data.temperature, 4, 1, tempStr);
    tft.print(tempStr);
    tft.print(useFahrenheit ? "F" : "C");
    
    // Draw high/low if available
    if (_data.tempHigh > 0 || _data.tempLow > 0) {
        tft.setTextSize(1);
        tft.setCursor(x + 30, y + 25);
        char hiLoStr[16];
        snprintf(hiLoStr, sizeof(hiLoStr), "H:%.0f L:%.0f", _data.tempHigh, _data.tempLow);
        tft.print(hiLoStr);
    }
}
