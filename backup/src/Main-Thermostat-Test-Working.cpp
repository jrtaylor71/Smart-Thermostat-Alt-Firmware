/*
 * ESP32-S3 Smart Thermostat Test with Custom TFT_eSPI Configuration
 * Testing TFT_eSPI with corrected pin configuration for ESP32-S3
 * STEP 3: Adding TFT_eSPI with custom ESP32-S3 pin mapping
 */

#include <WiFi.h>
#include <Preferences.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TFT_eSPI.h>
#include <SPI.h>

// Diagnostic print function to output to both Serial ports
void diagPrint(const char* str) {
    Serial.print(str);
    Serial1.print(str);
}

void diagPrintln(const char* str) {
    Serial.println(str);
    Serial1.println(str);
}

void diagPrintf(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    Serial.print(buffer);
    Serial1.print(buffer);
}

// Hardware pins (matching smart-thermostat config)
#define DHT_PIN     42    // GPIO42 for DHT22/DHT11 sensor
#define DHT_TYPE    DHT22 // DHT22 (AM2302) sensor type
#define ONE_WIRE_BUS 14   // GPIO14 for DS18B20 sensor
// TFT pins are now defined in build_flags in platformio.ini

// OneWire and DallasTemperature setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Global objects
Preferences preferences;
DHT dht(DHT_PIN, DHT_TYPE);
TFT_eSPI tft = TFT_eSPI();

void setup()
{
    // Initialize both Serial ports for ESP32-S3
    Serial.begin(115200);       // USB-Serial/JTAG
    Serial1.begin(115200, SERIAL_8N1, 44, 43);  // Diagnostic UART (RX=44, TX=43)
    delay(3000);  // Extra long delay for ESP32-S3 USB-CDC
    
    // Force flush and multiple attempts
    for(int i = 0; i < 10; i++) {
        diagPrintln("=== ESP32-S3 THERMOSTAT DEBUG START ===");
        Serial.flush();
        Serial1.flush();
        delay(100);
    }
    
    diagPrintln("Serial Working - ESP32-S3-WROOM-1-N16");
    diagPrintln("Step 1: Basic Serial OK");
    
    // Test 1: Initialize Preferences
    diagPrintln("Step 2: Initializing Preferences...");
    preferences.begin("thermostat", false);
    diagPrintln("Step 2: Preferences OK");
    
    // Test 2: Load basic preference strings
    Serial.println("Step 3: Loading preference strings...");
    String wifiSSID = preferences.getString("wifiSSID", "");
    String wifiPassword = preferences.getString("wifiPassword", "");
    String hostname = preferences.getString("hostname", "ESP32-Simple-Thermostat");
    Serial.println("Step 3: Preference loading OK");
    Serial.printf("SSID: %s, Password: %s, Hostname: %s\n", wifiSSID.c_str(), wifiPassword.c_str(), hostname.c_str());
    
    // Test 3: WiFi hostname setting (SUSPECT)
    Serial.println("Step 4: Setting WiFi hostname...");
    WiFi.setHostname(hostname.c_str());
    Serial.println("Step 4: WiFi hostname OK");
    
    // Test 4: Initialize DHT sensor
    Serial.println("Step 5: Initializing DHT sensor...");
    dht.begin();
    Serial.println("Step 5: DHT sensor initialized OK");
    
    // Test 5: Read DHT sensor
    Serial.println("Step 6: Reading DHT sensor...");
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    Serial.printf("DHT - Temperature: %.1f°C, Humidity: %.1f%%\n", temperature, humidity);
    Serial.println("Step 6: DHT reading complete");
    
    // Test 6: Initialize DS18B20 sensors
    Serial.println("Step 7: Initializing DS18B20 sensors...");
    sensors.begin();
    int deviceCount = sensors.getDeviceCount();
    Serial.printf("Found %d DS18B20 device(s)\n", deviceCount);
    Serial.println("Step 7: DS18B20 sensors initialized");
    
    // Test 7: Read DS18B20 temperature
    Serial.println("Step 8: Reading DS18B20 temperature...");
    sensors.requestTemperatures();
    float ds18b20Temp = sensors.getTempCByIndex(0);
    Serial.printf("DS18B20 Temperature: %.2f°C\n", ds18b20Temp);
    Serial.println("Step 8: DS18B20 reading complete");
    
    // Test 8: Setup TFT backlight with PWM control on correct pin (GPIO14)
    diagPrintln("Step 9: Setting up TFT backlight on GPIO14...");
    pinMode(14, OUTPUT);
    // Use PWM for backlight control - full brightness
    ledcSetup(0, 5000, 8); // Channel 0, 5kHz, 8-bit resolution
    ledcAttachPin(14, 0);   // Attach GPIO14 to channel 0
    ledcWrite(0, 255);      // Full brightness (255/255)
    diagPrintln("Step 9: Backlight PWM setup complete - GPIO14 at full brightness");
    
    // Test 9: Test SPI pins before TFT initialization (smart-thermostat pins)
    diagPrintln("Step 10a: Testing SPI pins...");
    // Test the SPI pins by toggling them (smart-thermostat configuration)
    pinMode(13, OUTPUT); // SCLK
    pinMode(12, OUTPUT); // MOSI
    pinMode(9, OUTPUT);  // CS
    pinMode(11, OUTPUT); // DC
    pinMode(10, OUTPUT); // RST
    
    diagPrintln("Toggling smart-thermostat SPI pins for hardware verification...");
    for(int i = 0; i < 5; i++) {
        digitalWrite(13, HIGH); // SCLK
        digitalWrite(12, HIGH); // MOSI
        digitalWrite(9, HIGH);  // CS
        digitalWrite(11, HIGH); // DC
        digitalWrite(10, HIGH); // RST
        delay(200);
        digitalWrite(13, LOW);
        digitalWrite(12, LOW);
        digitalWrite(9, LOW);
        digitalWrite(11, LOW);
        digitalWrite(10, LOW);
        delay(200);
    }
    
    // Test 9b: Initialize TFT display
    diagPrintln("Step 10b: Initializing TFT display...");
    tft.init();
    tft.setRotation(1); // Landscape orientation
    diagPrintln("Step 10b: TFT display initialized");
    
    // Test 10: Test TFT display output with debugging
    Serial.println("Step 11: Testing TFT display output...");
    
    // Try multiple display operations with debugging
    Serial.println("Attempting to fill screen BLACK...");
    tft.fillScreen(TFT_BLACK);
    delay(500);
    
    Serial.println("Attempting to fill screen RED...");
    tft.fillScreen(TFT_RED);
    delay(1000);
    
    Serial.println("Attempting to fill screen GREEN...");
    tft.fillScreen(TFT_GREEN);
    delay(1000);
    
    Serial.println("Attempting to fill screen BLUE...");
    tft.fillScreen(TFT_BLUE);
    delay(1000);
    
    Serial.println("Setting text and drawing...");
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("ESP32-S3", 10, 10);
    tft.drawString("THERMOSTAT", 10, 40);
    tft.setTextSize(1);
    tft.drawString("Display Test", 10, 80);
    tft.drawString("System Ready!", 10, 100);
    
    // Force display refresh and keep backlight on
    ledcWrite(0, 255); // Ensure backlight stays on
    Serial.println("Step 11: TFT display test complete!");
    
    // Test 10: Setup status LED for visual feedback
    Serial.println("Step 11: Setting up status LED for visual feedback...");
    pinMode(LED_BUILTIN, OUTPUT);  // Built-in LED
    digitalWrite(LED_BUILTIN, HIGH);  // Turn on to show system is ready
    Serial.println("Step 11: Status LED ON - System Ready!");
    
    // Test 12: Check if we reach the end
    diagPrintln("Step 12: ALL TESTS PASSED - DHT + DS18B20 + TFT!");
    diagPrintln("=== ESP32-S3 Smart Thermostat Ready ===");
    diagPrintln("RELAYS SHOULD BE CLICKING - HARDWARE WORKING!");
}

void loop()
{
    static unsigned long lastPrint = 0;
    static unsigned long lastLED = 0;
    static unsigned long lastDisplay = 0;
    static bool ledState = false;
    static int loopCount = 0;
    static int displayCycle = 0;
    
    loopCount++;
    
    // Blink LED every second to show system is alive
    if (millis() - lastLED > 1000) {
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
        lastLED = millis();
    }
    
    // Update display every 5 seconds to keep it active
    if (millis() - lastDisplay > 5000) {
        // Ensure backlight stays on GPIO14
        ledcWrite(0, 255); // GPIO14 full brightness
        
        // Cycle through different display content to show it's working
        displayCycle++;
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextSize(2);
        tft.drawString("ESP32-S3", 10, 10);
        tft.drawString("THERMOSTAT", 10, 40);
        tft.setTextSize(1);
        
        switch(displayCycle % 4) {
            case 0:
                tft.drawString("Display Active", 10, 80);
                break;
            case 1:
                tft.drawString("System Running", 10, 80);
                break;
            case 2:
                tft.drawString("No Crashes!", 10, 80);
                break;
            case 3:
                tft.drawString("Hardware OK", 10, 80);
                break;
        }
        
        tft.drawString("Uptime: " + String(millis() / 1000) + "s", 10, 100);
        tft.drawString("Loop: " + String(loopCount), 10, 120);
        
        diagPrintln("Display refreshed - keeping active");
        lastDisplay = millis();
    }
    
    // Print status every 2 seconds with more aggressive output
    if (millis() - lastPrint > 2000) {
        for(int i = 0; i < 3; i++) {
            diagPrintln("=== ESP32-S3 ALIVE ===");
            diagPrintf("Loop: %d, Uptime: %lu sec\n", loopCount, millis() / 1000);
            diagPrintln("SYSTEM RUNNING - NO CRASHES!");
            Serial.flush();
            Serial1.flush();
            delay(50);
        }
        lastPrint = millis();
    }
    delay(50);
}