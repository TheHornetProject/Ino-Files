/*
  Tell DCS-BIOS to use a serial connection and use interrupt-driven
  communication. The main program will be interrupted to prioritize
  processing incoming data.
  
  This should work on any Arduino that has an ATMega328 controller
  (Uno, Pro Mini, many others).

  comm port 10
 */
#define DCSBIOS_IRQ_SERIAL

#include "DcsBios.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned int brakePres = 0; //BrakePres

void onHydIndBrakeChange(unsigned int newValue) {

if (newValue < 18000) {
  brakePres = map(newValue, 0, 10000, 0 , 20);
}

else brakePres = map(newValue,  10001 ,65539, 21, 40);

 display.clearDisplay();
  display.setTextSize(2);
 display.setTextColor(SH110X_WHITE);
 display.setCursor(5,1);
 display.print("BRAKE PRES");
 display.setTextSize(1);
 display.setTextColor(SH110X_WHITE);
 display.setCursor(40,55);
 display.print("PSI x 100");
 display.setTextSize(2);
 display.setTextColor(SH110X_WHITE);
 display.setCursor(58,25);
 display.print(brakePres);

 display.display();

}
DcsBios::IntegerBuffer hydIndBrakeBuffer(0x7506, 0xffff, 0, onHydIndBrakeChange);

//ECM Dispenser Button
DcsBios::Switch2Pos cmsdDispenseBtn("CMSD_DISPENSE_BTN", 23);

//Flight Control System Panel
DcsBios::Switch2Pos fcsResetBtn("FCS_RESET_BTN", 25);
DcsBios::Switch2Pos gainSwitch("GAIN_SWITCH", 29);
DcsBios::Switch2Pos gainSwitchCover("GAIN_SWITCH_COVER", 30,true);

DcsBios::Switch2Pos toTrimBtn("TO_TRIM_BTN", 26);
DcsBios::RotaryEncoder rudTrim("RUD_TRIM", "-2000", "+2000", 27, 28);


//Communications Panel
//DcsBios::Potentiometer comAux("COM_AUX", A0);
DcsBios::Potentiometer ifei("IFEI", A0);
DcsBios::Potentiometer comIcs("COM_ICS", A1);
DcsBios::Potentiometer comMidsA("COM_MIDS_A", A2);
DcsBios::Potentiometer comMidsB("COM_MIDS_B", A3);
DcsBios::Potentiometer comRwr("COM_RWR", A4);
DcsBios::Potentiometer comTacan("COM_TACAN", A5);
DcsBios::Potentiometer comVox("COM_VOX", A6);
DcsBios::Potentiometer comWpn("COM_WPN", A7);

/*DcsBios::Switch2Pos comIlsUfcManSw("COM_ILS_UFC_MAN_SW", 38);
DcsBios::RotaryEncoder comIlsChannelSw("COM_ILS_CHANNEL_SW", "DEC", "INC", 48, 49);
DcsBios::Switch3Pos comIffMode4Sw("COM_IFF_MODE4_SW", 39, 40);
DcsBios::Switch2Pos comIffMasterSw("COM_IFF_MASTER_SW", 41);
DcsBios::Switch3Pos comCryptoSw("COM_CRYPTO_SW", 42, 43);
DcsBios::Switch3Pos comCommRelaySw("COM_COMM_RELAY_SW", 44, 45);
DcsBios::Switch3Pos comCommGXmtSw("COM_COMM_G_XMT_SW", 46, 47);*/

//Antenna Select Panel
DcsBios::Switch3Pos comm1AntSelectSw("COMM1_ANT_SELECT_SW", 31, 32);
DcsBios::Switch3Pos iffAntSelectSw("IFF_ANT_SELECT_SW", 33, 35);

//LOX Indicator
DcsBios::Switch2Pos obogsSw("OBOGS_SW", 24);
DcsBios::Potentiometer oxyFlow("OXY_FLOW", A8);

//Mission Computer and Hyd Isolate Pin
DcsBios::Switch2Pos hydIsolateOverrideSw("HYD_ISOLATE_OVERRIDE_SW", 34);
DcsBios::Switch3Pos mcSw("MC_SW", 36, 37);

DcsBios::LED extStrobeLights(0x74d6, 0x2000, 52);

void setup() {
 display.begin(0x3C, true); // Address 0x3C default 
 display.clearDisplay();
 
 DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
