/*
Comm Port 15
 */
#define DCSBIOS_IRQ_SERIAL

#include "DcsBios.h"
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 6
#define DATA_PIN 12
#define BRIGHTNESS 255

// Define the array of leds
CRGB leds[NUM_LEDS];

unsigned int aoaHigh = 0;
unsigned int aoaLow = 0;
unsigned int aoaNormal = 0;
unsigned int lsLock = 0;
unsigned int lsShoot = 0;
unsigned int lsStrobe = 0;


//AOA Indicator
void onAoaIndexerHighChange(unsigned int aoaHigh)
{
 if (aoaHigh == 1)
 {
   FastLED.setBrightness(64);
 leds[0] = CRGB::Red;
  FastLED.show();
 }
 else
 {
   leds[0] = CRGB::Black;
  FastLED.show();
 }
}
DcsBios::IntegerBuffer aoaIndexerHighBuffer(0x7408, 0x0008, 3, onAoaIndexerHighChange);


void onAoaIndexerLowChange(unsigned int aoaLow) 
{
     if (aoaLow == 1)
 {
   FastLED.setBrightness(64);
 //leds[2] = CRGB::Red;
leds[2] =0x00640A;
  FastLED.show();
 }
 else
 {
   leds[2] = CRGB::Black;
  FastLED.show();
  }
}
DcsBios::IntegerBuffer aoaIndexerLowBuffer(0x7408, 0x0020, 5, onAoaIndexerLowChange);


void onAoaIndexerNormalChange(unsigned int aoaNormal) 
{
     if (aoaNormal == 1)
 {
   FastLED.setBrightness(64);
 //leds[1] = CRGB::Red;
  leds[1] =  0x285500; //nice lighter orange
  FastLED.show();
 }
 else
 {
   leds[1] = CRGB::Black;
  FastLED.show();
  }
}
DcsBios::IntegerBuffer aoaIndexerNormalBuffer(0x7408, 0x0010, 4, onAoaIndexerNormalChange);

//Lock and Shoot Indicator
void onLsLockChange(unsigned int lsLock) 
{
       if (lsLock == 1)
 {
   FastLED.setBrightness(64);
 leds[3] = CRGB::Red;
  FastLED.show();
 }
 else
 {
   leds[3] = CRGB::Black;
  FastLED.show();
}
}
DcsBios::IntegerBuffer lsLockBuffer(0x7408, 0x0001, 0, onLsLockChange);

void onLsShootChange(unsigned int lsShoot) 
{
       if (lsShoot == 1)
 {
   FastLED.setBrightness(64);
 leds[4] = CRGB::Red;
  FastLED.show();
 }
 else
 {
   leds[4] = CRGB::Black;
  FastLED.show();
}
}

DcsBios::IntegerBuffer lsShootBuffer(0x7408, 0x0002, 1, onLsShootChange);

void onLsShootStrobeChange(unsigned int lsStrobe)
{
       if (lsStrobe == 1)
 {
   FastLED.setBrightness(255);
 leds[5] = CRGB::White;
  FastLED.show();
 }
 else
 {
   leds[5] = CRGB::Black;
  FastLED.show();
 }
}
DcsBios::IntegerBuffer lsShootStrobeBuffer(0x7408, 0x0004, 2, onLsShootStrobeChange);


//unsigned int RwrPower = 0; //Alr67 Lights lights intensity
//unsigned int RwrLimit = 0; //Alr67 Lights lights intensity

//Hud Control Box
DcsBios::Switch3Pos hudSymRejSw("HUD_SYM_REJ_SW", 7, 30);
DcsBios::Switch2Pos hudSymBrtSelect("HUD_SYM_BRT_SELECT", 11);
DcsBios::Switch3Pos hudAttSw("HUD_ATT_SW", 8, 9);
DcsBios::Switch2Pos hudAltSw("HUD_ALT_SW", 10);
DcsBios::Potentiometer hudAoaIndexer("HUD_AOA_INDEXER", A4);
DcsBios::Potentiometer hudSymBrt("HUD_SYM_BRT", A3);


//AMPCD
DcsBios::Potentiometer ampcdBrtCtl("AMPCD_BRT_CTL", A0);
DcsBios::Switch3Pos leftDdiCrsSw("LEFT_DDI_CRS_SW", 24, 22);
DcsBios::Switch3Pos leftDdiHdgSw("LEFT_DDI_HDG_SW", 28, 26);


//ECM
DcsBios::Switch2Pos auxRelSw("AUX_REL_SW", 29);
DcsBios::Switch3Pos cmsdDispenseSw("CMSD_DISPENSE_SW", 25, 27);
DcsBios::Switch2Pos cmsdJetSelBtn("CMSD_JET_SEL_BTN", 23);
const byte ecmModeSwPins[5] = {39, 37, 35, 33, 31};
DcsBios::SwitchMultiPos ecmModeSw("ECM_MODE_SW", ecmModeSwPins, 5);



//ALR

//Power
DcsBios::Switch2Pos rwrPowerBtn("RWR_POWER_BTN", 49);
DcsBios::LED rwrLowerLt(0x7498, 0x1000, 2);

/*void onRwrLowerLtChange(unsigned int newValue) {
   if (newValue < 32000) {
      RwrPower = map(newValue, 0, 31999, 0 , 120);  //slower kick-in to match sim
    }
    
    else {
      RwrPower = map(newValue, 32000, 65535, 121 , 255); //output capped to lower max brightness
    }
    analogWrite(2, RwrPower);
}
DcsBios::IntegerBuffer rwrLowerLtBuffer(0x7498, 0x1000, 12, onRwrLowerLtChange);
*/
//Display
DcsBios::Switch2Pos rwrDisplayBtn("RWR_DISPLAY_BTN", 47);
DcsBios::LED rwrLimitLt(0x7498, 0x2000, 3);

/*void onRwrLimitLtChange(unsigned int newValue) {
     if (newValue < 32000) {
      RwrLimit = map(newValue, 0, 31999, 0 , 120);  //slower kick-in to match sim
    }
    
    else {
      RwrLimit = map(newValue, 32000, 65535, 121 , 200); //output capped to lower max brightness
    }
    analogWrite(3, RwrLimit);
}
DcsBios::IntegerBuffer rwrLimitLtBuffer(0x7498, 0x2000, 13, onRwrLimitLtChange);
*/

//Special
DcsBios::Switch2Pos rwrSpecialBtn("RWR_SPECIAL_BTN", 45);
//DcsBios::LED rwrSpecialLt(0x749c, 0x0100, 4);

//Offset
DcsBios::Switch2Pos rwrOffsetBtn("RWR_OFFSET_BTN", 43);
DcsBios::LED rwrEnableLt(0x749c, 0x0200, 5);

//BIT
DcsBios::Switch2Pos rwrBitBtn("RWR_BIT_BTN", 41);
//DcsBios::LED rwrBitLt(0x749c, 0x1000, 35);
DcsBios::LED rwrFailLt(0x749c, 0x0800, 6);




DcsBios::Potentiometer rwrAudioCtrl("RWR_AUDIO_CTRL", A1);
DcsBios::Potentiometer rwrDmrCtrl("RWR_DMR_CTRL", A2);

//Display Type
const byte rwrDisTypeSwPins[5] = {32, 34, 36, 38, 40};
DcsBios::SwitchMultiPos rwrDisTypeSw("RWR_DIS_TYPE_SW", rwrDisTypeSwPins, 5);


void setup() {
  DcsBios::setup();
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS); 
}

void loop() {
  DcsBios::loop();
}
