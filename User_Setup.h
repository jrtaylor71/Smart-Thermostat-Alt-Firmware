// ESP32-S3 User Setup for TFT_eSPI library
// Pin assignments matching smart-thermostat hardware v0.04

#define ILI9341_DRIVER       // Define the display driver

#define TFT_MISO 21    // SPI MISO (smart-thermostat hardware)
#define TFT_MOSI 12    // SPI MOSI (smart-thermostat hardware)  
#define TFT_SCLK 13    // SPI Clock (smart-thermostat hardware)
#define TFT_CS   9     // Chip Select (smart-thermostat hardware)
#define TFT_DC   11    // Data/Command (smart-thermostat hardware)
#define TFT_RST  10    // Reset (smart-thermostat hardware)

#define TOUCH_CS 47    // Touch Chip Select (smart-thermostat hardware)

// Load glcd font
#define LOAD_GLCD   
#define LOAD_FONT2  
#define LOAD_FONT4  
#define LOAD_FONT6  
#define LOAD_FONT7  
#define LOAD_FONT8  
#define LOAD_GFXFF  

// Define the SPI frequency
#define SPI_FREQUENCY  20000000   // 20MHz for good performance
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000