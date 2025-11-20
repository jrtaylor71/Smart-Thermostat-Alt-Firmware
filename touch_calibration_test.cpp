// Official TFT_eSPI Touch Calibration - Rotation 1 (Landscape)
// Based on TFT_eSPI Touch_calibrate example
// CRITICAL: Must calibrate at same rotation as application!

#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n=== ESP32-S3 Touch Calibration ===");
  Serial.println("This will calibrate for ROTATION 1 (landscape)");
  Serial.println("Follow the on-screen instructions...\n");

  tft.init();

  // CRITICAL: Set rotation BEFORE calibration
  // This must match your thermostat's rotation!
  tft.setRotation(1);

  // Run calibration
  touch_calibrate();

  // Clear and prepare for testing
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawCentreString("Touch screen to test calibration!", tft.width()/2, tft.height()/2, 2);
  tft.drawCentreString("White dots show where you touched", tft.width()/2, tft.height()/2 + 20, 2);
}

void loop(void) {
  uint16_t x = 0, y = 0;

  // Test the calibrated touch
  bool pressed = tft.getTouch(&x, &y);

  if (pressed) {
    // Draw white dot where touched
    tft.fillCircle(x, y, 3, TFT_WHITE);
    
    // Also print to serial
    Serial.print("Calibrated touch at x=");
    Serial.print(x);
    Serial.print(", y=");
    Serial.println(y);
  }
}

// Official TFT_eSPI calibration function
void touch_calibrate() {
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");
  tft.setTextFont(1);
  tft.println();

  // Calibrate with smaller crosshairs (8 pixels like your thermostat)
  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 8);

  Serial.println(); 
  Serial.println();
  Serial.println("=== CALIBRATION COMPLETE ===");
  Serial.println("// Copy this code into your thermostat setup():");
  Serial.print("uint16_t calData[5] = { ");

  for (uint8_t i = 0; i < 5; i++) {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.println("tft.setTouch(calData);");
  Serial.println("=============================");
  Serial.println();

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");
  tft.println("");
  tft.println("Now testing calibration...");

  delay(3000);
}