// Touch Diagnostic Test - Based on TFT_eSPI Test_Touch_Controller
// This will show RAW touch values to help debug coordinate mapping

#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

void setup(void) {
  Serial.begin(115200);
  Serial.println("\n\n=== ESP32-S3 Touch Diagnostic Test ===");
  Serial.println("Based on TFT_eSPI Test_Touch_Controller");
  Serial.println("Raw ADC values will be shown - NOT pixel coordinates");
  Serial.println("Touch the screen to see x, y, z values...\n");

  tft.init();
  
  // Set the same rotation as your thermostat
  tft.setRotation(1);
  
  // Just clear screen but don't draw anything
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  uint16_t x, y;

  // Get RAW touch values (ADC readings, not pixels)
  tft.getTouchRaw(&x, &y);
  
  Serial.printf("Raw x: %5i     ", x);
  Serial.printf("Raw y: %5i     ", y);
  Serial.printf("Raw z: %5i", tft.getTouchRawZ());
  
  // Also show if touch is detected
  uint16_t px, py;
  bool pressed = tft.getTouch(&px, &py);
  if (pressed) {
    Serial.printf("  ->  Pixel: x=%3i, y=%3i", px, py);
  }
  Serial.println();

  delay(100);  // Faster than original for better debugging
}