/*********************************************************************
  This is an example for our Monochrome OLEDs based on SH110X drivers

  This example is for a 128x64 size display using I2C to communicate
  3 pins are required to interface (2 I2C and one reset)

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution

  i2c SH1106 modified by Rupert Hirst  12/09/21
*********************************************************************/



#define DCSBIOS_IRQ_SERIAL

#include <DcsBios.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
//Comm Port 17
/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


unsigned int minHeightPointer = 0; //Set Height
unsigned int radarAlt = 5000; //Radar Altitude
//unsigned int offFlag = 999;

//RH Vertical Panel
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

//KY58
const byte ky58ModeSelectPins[4] = {28, 30, 32, 34};
DcsBios::SwitchMultiPos ky58ModeSelect("KY58_MODE_SELECT", ky58ModeSelectPins, 4);
const byte ky58PowerSelectPins[3] = {22, 24, 26};
DcsBios::SwitchMultiPos ky58PowerSelect("KY58_POWER_SELECT", ky58PowerSelectPins, 3);
DcsBios::Potentiometer ky58Volume("KY58_VOLUME", A0);

void setup() {



  display.begin(0x3C, true); // Address 0x3C default 
  display.clearDisplay();
  display.setTextSize(3);
 display.setTextColor(SH110X_WHITE);
 display.setCursor(10,15);
 display.print("STANDBY");
 
 DcsBios::setup();
}

void loop() {
 DcsBios::loop();
}

/*void onRadaltOffFlagChange(unsigned int newValue) {
   offFlag=map(newValue, 0, 64355, 0 , 1);
}
DcsBios::IntegerBuffer radaltOffFlagBuffer(0x750c, 0xffff, 0, onRadaltOffFlagChange);
*/


//Min Height Pointer
void onRadaltMinHeightPtrChange(unsigned int newValue) {

 if (newValue <=2034) {
  minHeightPointer = map(newValue, 0, 2034, 0, 1);
}

else if ((newValue >2034) && (newValue <=34407)) {
  minHeightPointer = map(newValue, 2035 , 34407, 0, 400);
}

else if ((newValue >34407) && (newValue <52559)) {
 minHeightPointer = map(newValue, 34408, 52559, 401, 1000);
}

else {
  minHeightPointer = map(newValue,52560, 65535, 1001, 5000);
}

  display.clearDisplay();
  display.clearDisplay();
    display.setTextSize(2);
 display.setTextColor(SH110X_WHITE);
 display.setCursor(10,5);
 display.print("RADAR ALT");
 display.setCursor(5,25);
 display.print("RAlt:");
 display.setCursor(5,50);
 display.print("Min:");
 
// display.setTextSize(1);
// display.setCursor(120,50);
// display.print(offFlag);
 
  display.setTextSize(2);
 display.setTextColor(SH110X_WHITE,SH110X_BLACK); // Draw white text on Black Background
 display.setCursor(70,25);
 display.print(radarAlt);
 display.setCursor(70,50);
 display.print(minHeightPointer);
 display.display();
 
}
DcsBios::IntegerBuffer radaltMinHeightPtrBuffer(0x7518, 0xffff, 0, onRadaltMinHeightPtrChange);

// Radar Altitude
void onRadaltAltPtrChange(unsigned int newValue) {

 if (newValue <=29) {
  radarAlt = map(newValue, 0, 29, 5000, 5000);
}

 else if  ((newValue >30) && (newValue <=2034)) {
  radarAlt = map(newValue, 0, 2034, 0, 1);
}
else if ((newValue >2034) && (newValue <=34569)) {
  radarAlt = map(newValue, 2035 , 34569, 0, 400);
}

else if ((newValue >34570) && (newValue <52373)) {
 radarAlt = map(newValue, 34570, 52373, 401, 1000);
}

else {
  radarAlt = map(newValue,52374, 65535, 1001, 5000);
}


  display.clearDisplay();
  display.clearDisplay();
    display.setTextSize(2);
 display.setTextColor(SH110X_WHITE);
 display.setCursor(10,5);
 display.print("RADAR ALT");
 display.setCursor(5,25);
 display.print("RAlt:");
 display.setCursor(5,50);
 display.print("Min:");
 
 // display.setTextSize(1);
// display.setCursor(120,50);
 //display.print(offFlag);
 
  display.setTextSize(2);
 display.setTextColor(SH110X_WHITE,SH110X_BLACK); // Draw white text on Black Background
 display.setCursor(70,25);
 display.print(radarAlt);
 display.setCursor(70,50);
 display.print(minHeightPointer);
 display.display();
}
DcsBios::IntegerBuffer radaltAltPtrBuffer(0x751a, 0xffff, 0, onRadaltAltPtrChange);
