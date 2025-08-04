#define DCSBIOS_IRQ_SERIAL
#include <DcsBios.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106G.h>

#define i2c_Address 0x3c // I2C address for SH1106 OLED
#define SCREEN_WIDTH 128 // OLED display width
#define SCREEN_HEIGHT 64 // OLED display height
#define OLED_RESET -1    // No reset pin
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned int radarAlt = 5000; // Radar Altitude
unsigned int minHeightPointer = 0; // Minimum Height Pointer
bool dcsBiosConnected = false; // Track DCS-BIOS connection status

// RH Vertical Panel
DcsBios::Switch2Pos fcsBitSw("FCS_BIT_SW", 3);
DcsBios::LED arrestingHookLt(0x74a0, 0x0400, 4);
DcsBios::Switch2Pos hookLever("HOOK_LEVER", 2);
const byte wingFoldRotatePins[3] = {29, 27, 25};
DcsBios::SwitchMultiPos wingFoldRotate("WING_FOLD_ROTATE", wingFoldRotatePins, 3);
DcsBios::Switch2Pos wingFoldPull("WING_FOLD_PULL", 23);
DcsBios::RotaryEncoder radaltHeight("RADALT_HEIGHT", "-2000", "+2000", 33, 35);
DcsBios::Switch2Pos radaltTestSw("RADALT_TEST_SW", 31);
DcsBios::LED lowAltWarnLt(0x749c, 0x8000, 37);
DcsBios::LED radaltGreenLamp(0x74a0, 0x0100, 39);

// KY-58
const byte ky58ModeSelectPins[4] = {28, 30, 32, 34};
DcsBios::SwitchMultiPos ky58ModeSelect("KY58_MODE_SELECT", ky58ModeSelectPins, 4);
const byte ky58PowerSelectPins[3] = {22, 24, 26};
DcsBios::SwitchMultiPos ky58PowerSelect("KY58_POWER_SELECT", ky58PowerSelectPins, 3);
DcsBios::Potentiometer ky58Volume("KY58_VOLUME", A0);

// Function to display STANDBY
void displayStandby() {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 15);
  display.print("STANDBY");
  display.display();
}

// Function to update OLED with radar data
void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 5);
  display.print("RADAR ALT");
  display.setCursor(5, 25);
  display.print("RAlt:");
  display.setCursor(5, 50);
  display.print("Min:");
  
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);
  display.setCursor(70, 25);
  display.print(radarAlt);
  display.setCursor(70, 50);
  display.print(minHeightPointer);
  display.display();
}

// DCS-BIOS connection status callback
void onDcsBiosConnect() {
  dcsBiosConnected = true;
}

// DCS-BIOS disconnection callback
void onDcsBiosDisconnect() {
  dcsBiosConnected = false;
  displayStandby(); // Show STANDBY when disconnected
}

void setup() {
  // Initialize OLED
  if (!display.begin(i2c_Address, true)) {
    // Halt if OLED fails to initialize
    while (1);
  }
  display.clearDisplay();
  displayStandby(); // Show STANDBY immediately
  DcsBios::setup();
  DcsBios::setOnConnectCallback(onDcsBiosConnect);
  DcsBios::setOnDisconnectCallback(onDcsBiosDisconnect);
}

void loop() {
  DcsBios::loop();
  if (!dcsBiosConnected) {
    displayStandby(); // Keep showing STANDBY until connected
  }
}

// Radar Altitude
void onRadaltAltPtrChange(unsigned int newValue) {
  if (newValue <= 29) {
    radarAlt = 5000;
  } else if (newValue <= 2034) {
    radarAlt = map(newValue, 30, 2034, 0, 1);
  } else if (newValue <= 34569) {
    radarAlt = map(newValue, 2035, 34569, 0, 400);
  } else if (newValue <= 52373) {
    radarAlt = map(newValue, 34570, 52373, 401, 1000);
  } else {
    radarAlt = map(newValue, 52374, 65535, 1001, 5000);
  }
  if (dcsBiosConnected) {
    updateDisplay();
  }
}
DcsBios::IntegerBuffer radaltAltPtrBuffer(0x751a, 0xffff, 0, onRadaltAltPtrChange);

// Minimum Height Pointer
void onRadaltMinHeightPtrChange(unsigned int newValue) {
  if (newValue <= 2034) {
    minHeightPointer = map(newValue, 0, 2034, 0, 1);
  } else if (newValue <= 34407) {
    minHeightPointer = map(newValue, 2035, 34407, 0, 400);
  } else if (newValue < 52559) {
    minHeightPointer = map(newValue, 34408, 52559, 401, 1000);
  } else {
    minHeightPointer = map(newValue, 52560, 65535, 1001, 5000);
  }
  if (dcsBiosConnected) {
    updateDisplay();
  }
}
DcsBios::IntegerBuffer radaltMinHeightPtrBuffer(0x7518, 0xffff, 0, onRadaltMinHeightPtrChange);
