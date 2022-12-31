
#define DCSBIOS_IRQ_SERIAL

#include <DcsBios.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//comm port 18


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned int eVolts = 16; //E Volts
unsigned int uVolts = 16; //U Volts


//unsigned int onLtdRSw_Coil = 4;    // Magnetic coil CH4

unsigned int ConsolesIntensity = 0; //consoles lights intensity
unsigned int instrIntensity = 0; //consoles lights intensity


//Right Wall
DcsBios::Switch3Pos canopySw("CANOPY_SW", A4, A3);
// DcsBios::Potentiometer defogHandle("DEFOG_HANDLE", A11);
// DcsBios::Switch3Pos wshieldAntiIceSw("WSHIELD_ANTI_ICE_SW", 51, 52);


// ELEC Panel
DcsBios::Switch2Pos lGenSw("L_GEN_SW", 22);
DcsBios::Switch3Pos batterySw("BATTERY_SW", 24, 23);
DcsBios::Switch2Pos rGenSw("R_GEN_SW", 25);

// ENVIRO Panel
const byte bleedAirKnobPins[4] = {26, 27, 28, 29};
DcsBios::SwitchMultiPos bleedAirKnob("BLEED_AIR_KNOB", bleedAirKnobPins, 4);
DcsBios::Switch3Pos cabinPressSw("CABIN_PRESS_SW", 33, 32);
DcsBios::Switch3Pos ecsModeSw("ECS_MODE_SW", 30, 31);
DcsBios::Switch3Pos engAntiiceSw("ENG_ANTIICE_SW", 34, 35);
DcsBios::Switch2Pos pitotHeatSw("PITOT_HEAT_SW", 36);


// Lighting Panel
DcsBios::Potentiometer consolesDimmer("CONSOLES_DIMMER", A0);
DcsBios::Potentiometer instPnlDimmer("INST_PNL_DIMMER", A1);
DcsBios::Switch2Pos lightsTestSw("LIGHTS_TEST_SW", 37);
DcsBios::Potentiometer warnCautionDimmer("WARN_CAUTION_DIMMER", A2);
DcsBios::Switch3Pos cockkpitLightModeSw("COCKKPIT_LIGHT_MODE_SW", 39, 38);

 // Sensor Panel
DcsBios::Switch3Pos flirSw("FLIR_SW", 41, 40);
//DcsBios::Switch2Pos ltdRSw("LTD_R_SW", 5);
DcsBios::ActionButton ltdRSwToggle("LTD_R_SW", "TOGGLE", 5);
DcsBios::LED ltdRSw(0x74c8, 0x4000, 4);

const byte insSwPins[8] = {42, 43, 44, 45, 46, 47, 48, 49};
DcsBios::SwitchMultiPos insSw("INS_SW", insSwPins, 8);
DcsBios::Switch2Pos lstNflrSw("LST_NFLR_SW", 50);
const byte radarSwPins[4] = {8, 9, 10, 11};
DcsBios::SwitchMultiPos radarSw("RADAR_SW", radarSwPins, 4);



// LASER Switch Arm

/*
void onLtdRSwChange(unsigned int newCoilValue) {
   switch (newCoilValue){
      case 0:
      digitalWrite(onLtdRSw_Coil, LOW);
      break;
      case 1:
      digitalWrite(onLtdRSw_Coil, HIGH);
      break;
    }
}
DcsBios::IntegerBuffer ltdRSwBuffer(0x74c8, 0x4000, 14, onLtdRSwChange);

*/

// Console Lighting
void onConsoleIntLtChange(unsigned int newValue) {
        if (newValue < 31199) {
      ConsolesIntensity = map(newValue, 0, 31199, 0 , 20);  //slower kick-in to match sim
    }
    
    else {
      ConsolesIntensity = map(newValue, 32000, 65535, 20 , 180); //output capped to lower max brightness
    }
    analogWrite(2, ConsolesIntensity);
}
DcsBios::IntegerBuffer consoleIntLtBuffer(0x7558, 0xffff, 0, onConsoleIntLtChange);

// Console Lighting
void onInstrIntLtChange(unsigned int newValue) {
         if (newValue < 32000) {
      instrIntensity = map(newValue, 0, 31999, 0 , 20);  //slower kick-in to match sim
    }
    
    else {
      instrIntensity = map(newValue, 32000, 65535, 20 , 180); //output capped to lower max brightness
    }
    analogWrite(3, instrIntensity);
}
DcsBios::IntegerBuffer instrIntLtBuffer(0x7560, 0xffff, 0, onInstrIntLtChange);


//EVolts
void onVoltEChange(unsigned int newValue) {
{
  eVolts = map(newValue, 0, 65535, 16 , 30);
}

 display.setCursor(80,30);
 display.print("E");
 display.setCursor(80,50);
 display.print(eVolts);

 display.display();
}

DcsBios::IntegerBuffer voltEBuffer(0x753e, 0xffff, 0, onVoltEChange);

//uVolts
void onVoltUChange(unsigned int newValue) {
{  
  uVolts = map(newValue, 0, 65539, 16 , 30);
}

 display.setCursor(21,30);
 display.print("U");
 display.setCursor(21,50);
 display.print(uVolts);
 
 display.display();
}

DcsBios::IntegerBuffer voltUBuffer(0x753c, 0xffff, 0, onVoltUChange);

void setup() {
  
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(WHITE,BLACK); // Draw white text
display.setCursor(40,9); display.print("BATT");

  DcsBios::setup();

 
//pinMode (onLtdRSw_Coil, OUTPUT);

}

void loop() {
  DcsBios::loop();
}
