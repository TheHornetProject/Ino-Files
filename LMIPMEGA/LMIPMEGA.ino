/*
  Tell DCS-BIOS to use a serial connection and use interrupt-driven
  communication. The main program will be interrupted to prioritize
  processing incoming data.
  
  This should work on any Arduino that has an ATMega328 controller
  (Uno, Pro Mini, many others).
 */
#define DCSBIOS_IRQ_SERIAL

#include "DcsBios.h"

//Comm Port 12

//LDDI*

 // Left Annunciator
DcsBios::LED lhAdvLBarGreen(0x740a, 0x0002, 32);
DcsBios::LED lhAdvLBarRed(0x7408, 0x8000, 33);
DcsBios::LED lhAdvSpdBrk(0x7408, 0x2000, 34);
DcsBios::LED lhAdvLBleed(0x7408, 0x0800, 35);
DcsBios::LED lhAdvGo(0x740a, 0x0010, 36);
DcsBios::LED lhAdvAspjOh(0x740a, 0x0008, 37);
DcsBios::LED lhAdvXmit(0x740a, 0x0004, 38);
DcsBios::LED lhAdvRec(0x740a, 0x0001, 39);
DcsBios::LED lhAdvStby(0x7408, 0x4000, 40);
DcsBios::LED lhAdvRBleed(0x7408, 0x1000, 41);
DcsBios::LED lhAdvNoGo(0x740a, 0x0020, 42);

//Master Caution
DcsBios::Switch2Pos masterCautionResetSw("MASTER_CAUTION_RESET_SW", 8);
DcsBios::LED masterCautionLt(0x7408, 0x0200, 9);
DcsBios::LED fireLeftLt(0x7408, 0x0040, 11);
DcsBios::Switch2Pos leftFireBtn("LEFT_FIRE_BTN", 10);

//Master Arm Panel
DcsBios::LED mcReady(0x740c, 0x8000, 31);

DcsBios::Switch2Pos masterArmSw("MASTER_ARM_SW", 25);
DcsBios::Switch2Pos masterModeAa("MASTER_MODE_AA", 27);
DcsBios::LED masterModeAaLt(0x740c, 0x0200, 26);
DcsBios::Switch2Pos masterModeAg("MASTER_MODE_AG", 29);
DcsBios::LED masterModeAgLt(0x740c, 0x0400, 28);
DcsBios::Switch2Pos emerJettBtn("EMER_JETT_BTN", 12);

//LDDI
const byte leftDdiBrtSelectPins[3] = {24, 23, 22};
DcsBios::SwitchMultiPos leftDdiBrtSelect("LEFT_DDI_BRT_SELECT", leftDdiBrtSelectPins, 3);
DcsBios::Potentiometer leftDdiBrtCtl("LEFT_DDI_BRT_CTL", A1);
DcsBios::Potentiometer leftDdiContCtl("LEFT_DDI_CONT_CTL", A2);

/*JETT SELECT STATION*/
DcsBios::LED flpLgFlapsLt(0x7466, 0x0001, 2);
DcsBios::LED flpLgFullFlapsLt(0x7430, 0x8000, 3);
DcsBios::LED flpLgHalfFlapsLt(0x7430, 0x4000, 4);
DcsBios::LED flpLgLeftGearLt(0x7430, 0x1000, 5);
DcsBios::LED flpLgRightGearLt(0x7430, 0x2000, 6);
DcsBios::LED flpLgNoseGearLt(0x7430, 0x0800, 7);
DcsBios::LED engInstrIntLt(0x74d4, 0x4000, A3);




DcsBios::LED sjCtrLt(0x742e, 0x4000, 44);
DcsBios::LED sjLiLt(0x742e, 0x8000, 46);
DcsBios::LED sjRiLt(0x7430, 0x0200, 48);
DcsBios::LED sjLoLt(0x7430, 0x0100, 50);
DcsBios::LED sjRoLt(0x7430, 0x0400, 52);

DcsBios::Switch2Pos sjCtr("SJ_CTR", 45);
DcsBios::Switch2Pos sjLi("SJ_LI", 47);
DcsBios::Switch2Pos sjRi("SJ_RI", 49);
DcsBios::Switch2Pos sjLo("SJ_LO", 51);
DcsBios::Switch2Pos sjRo("SJ_RO", 53);


void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
