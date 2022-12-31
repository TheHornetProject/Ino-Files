//Commport 16

#define DCSBIOS_IRQ_SERIAL

#include "DcsBios.h"
#include <AccelStepper.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 3
#define DATA_PIN 30
#define BRIGHTNESS 255

// Define the array of leds
CRGB leds[NUM_LEDS];

unsigned int lsLock = 0;
unsigned int lsShoot = 0;
unsigned int lsStrobe = 0;

//Lock and Shoot Indicator
void onLsLockChange(unsigned int lsLock) 
{
       if (lsLock == 1)
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
DcsBios::IntegerBuffer lsLockBuffer(0x7408, 0x0001, 0, onLsLockChange);

void onLsShootChange(unsigned int lsShoot) 
{
       if (lsShoot == 1)
 {
   FastLED.setBrightness(64);
 leds[1] = CRGB::Red;
  FastLED.show();
 }
 else
 {
   leds[1] = CRGB::Black;
  FastLED.show();
}
}

DcsBios::IntegerBuffer lsShootBuffer(0x7408, 0x0002, 1, onLsShootChange);

void onLsShootStrobeChange(unsigned int lsStrobe)
{
       if (lsStrobe == 1)
 {
   FastLED.setBrightness(255);
 leds[2] = CRGB::White;
  FastLED.show();
 }
 else
 {
   leds[2] = CRGB::Black;
  FastLED.show();
 }
}
DcsBios::IntegerBuffer lsShootStrobeBuffer(0x7408, 0x0004, 2, onLsShootStrobeChange);


//RDDI Master

//RDDI Controls
const byte rightDdiBrtSelectPins[3] = {22, 24, 26};
DcsBios::SwitchMultiPos rightDdiBrtSelect("RIGHT_DDI_BRT_SELECT", rightDdiBrtSelectPins, 3);
DcsBios::Potentiometer rightDdiBrtCtl("RIGHT_DDI_BRT_CTL", A2);
DcsBios::Potentiometer rightDdiContCtl("RIGHT_DDI_CONT_CTL", A1);

//RH Advisory Panel
DcsBios::LED rhAdvAaa(0x740a, 0x0800, 49);
DcsBios::LED rhAdvAi(0x740a, 0x0400, 51);
DcsBios::LED rhAdvCw(0x740a, 0x1000, 53);
DcsBios::LED rhAdvDisp(0x740a, 0x0100, 43);
DcsBios::LED rhAdvRcdrOn(0x740a, 0x0080, 45);
DcsBios::LED rhAdvSam(0x740a, 0x0200, 47);

DcsBios::Switch2Pos apuFireBtn("APU_FIRE_BTN", 46);
DcsBios::LED fireApuLt(0x740c, 0x0004, 48);

DcsBios::Switch2Pos rightFireBtn("RIGHT_FIRE_BTN", 50);
DcsBios::LED fireRightLt(0x740c, 0x0010, 52);

//SPIN
DcsBios::Potentiometer hmdOffBrt("HMD_OFF_BRT", A3);
DcsBios::Switch3Pos irCoolSw("IR_COOL_SW", 2, 3);
//DcsBios::Switch2Pos spinRecoveryCover("SPIN_RECOVERY_COVER", 4);
DcsBios::ActionButton spinRecoveryCoverToggle("SPIN_RECOVERY_COVER", "TOGGLE", 4);
//DcsBios::Switch2Pos spinRecoverySw("SPIN_RECOVERY_SW", 5);
DcsBios::LED spinLt(0x742a, 0x0800, 6);

DcsBios::Potentiometer rwrRwrIntesity("RWR_RWR_INTESITY", A0);
DcsBios::Switch2Pos saiCage("SAI_CAGE", 31);
DcsBios::RotaryEncoder saiSet("SAI_SET", "-3200", "+3200", 27, 29);
DcsBios::RotaryEncoder stbyPressAlt("STBY_PRESS_ALT", "-3200", "+3200", 25, 23);
DcsBios::LED emergInstrIntLt(0x74d4, 0x2000, 28);

// Compass

unsigned long nextupdate = 0;
bool outputstate;
int flashinterval = 1000;
#define LedMonitorPin 7

struct StepperConfig {
  unsigned int maxSteps;
  unsigned int acceleration;
  unsigned int maxSpeed;
};

const long zeroTimeout = 3000;

class Vid60Stepper : public DcsBios::Int16Buffer {
  private:
    AccelStepper& stepper;
    StepperConfig& stepperConfig;
    inline bool zeroDetected() {
      return digitalRead(irDetectorPin) == 0;
    }
    unsigned int (*map_function)(unsigned int);
    unsigned char initState;
    long currentStepperPosition;
    long lastAccelStepperPosition;
    unsigned char irDetectorPin;
    long zeroOffset;
    bool movingForward;
    bool lastZeroDetectState;

    long zeroPosSearchStartTime = 0;

    long normalizeStepperPosition(long pos) {
      if (pos < 0) return pos + stepperConfig.maxSteps;
      if (pos >= stepperConfig.maxSteps) return pos - stepperConfig.maxSteps;
      return pos;
    }

    void updateCurrentStepperPosition() {
      // adjust currentStepperPosition to include the distance our stepper motor
      // was moved since we last updated it
      long movementSinceLastUpdate = stepper.currentPosition() - lastAccelStepperPosition;
      currentStepperPosition = normalizeStepperPosition(currentStepperPosition + movementSinceLastUpdate);
      lastAccelStepperPosition = stepper.currentPosition();
    }
  public:
    Vid60Stepper(unsigned int address, AccelStepper& stepper, StepperConfig& stepperConfig, unsigned char irDetectorPin, long zeroOffset, unsigned int (*map_function)(unsigned int))
      : Int16Buffer(address), stepper(stepper), stepperConfig(stepperConfig), irDetectorPin(irDetectorPin), zeroOffset(zeroOffset), map_function(map_function), initState(0), currentStepperPosition(0), lastAccelStepperPosition(0) {
    }

    virtual void loop() {
      if (initState == 0) { // not initialized yet
        pinMode(irDetectorPin, INPUT);
        stepper.setMaxSpeed(stepperConfig.maxSpeed);
        stepper.setSpeed(400);

        initState = 1;
        zeroPosSearchStartTime = millis();
      }

      if (initState == 1) {
        // move off zero if already there so we always get movement on reset
        // (to verify that the stepper is working)
        if (zeroDetected()) {
          stepper.runSpeed();
        } else {
          initState = 2;
        }
      }

      if (initState == 2) { // zeroing
        if (!zeroDetected()) {
          // Currently this safety check isn't working
          // Add Ethernet card for more troubleshooting
          // Need to check IP addresses of PC secondary nic
          if (millis() >= (zeroTimeout + zeroPosSearchStartTime)) {
            stepper.disableOutputs();
            initState == 99;
          }
          else
            stepper.runSpeed();



        } else {
          stepper.setAcceleration(stepperConfig.acceleration);
          stepper.runToNewPosition(stepper.currentPosition() + zeroOffset);
          // tell the AccelStepper library that we are at position zero
          stepper.setCurrentPosition(0);
          lastAccelStepperPosition = 0;
          // set stepper acceleration in steps per second per second
          // (default is zero)
          stepper.setAcceleration(stepperConfig.acceleration);

          lastZeroDetectState = true;
          initState = 3;
        }
      }
      if (initState == 3) { // running normally

        // recalibrate when passing through zero position
        bool currentZeroDetectState = zeroDetected();
        if (!lastZeroDetectState && currentZeroDetectState && movingForward) {
          // we have moved from left to right into the 'zero detect window'
          // and are now at position 0
          lastAccelStepperPosition = stepper.currentPosition();
          currentStepperPosition = normalizeStepperPosition(zeroOffset);
        } else if (lastZeroDetectState && !currentZeroDetectState && !movingForward) {
          // we have moved from right to left out of the 'zero detect window'
          // and are now at position (maxSteps-1)
          lastAccelStepperPosition = stepper.currentPosition();
          currentStepperPosition = normalizeStepperPosition(stepperConfig.maxSteps + zeroOffset);
        }
        lastZeroDetectState = currentZeroDetectState;


        if (hasUpdatedData()) {
          // convert data from DCS to a target position expressed as a number of steps
          long targetPosition = (long)map_function(getData());

          updateCurrentStepperPosition();

          long delta = targetPosition - currentStepperPosition;

          // if we would move more than 180 degree counterclockwise, move clockwise instead
          if (delta < -((long)(stepperConfig.maxSteps / 2))) delta += stepperConfig.maxSteps;
          // if we would move more than 180 degree clockwise, move counterclockwise instead
          if (delta > (stepperConfig.maxSteps / 2)) delta -= (long)stepperConfig.maxSteps;

          movingForward = (delta >= 0);

          // tell AccelStepper to move relative to the current position
          stepper.move(delta);

        }
        stepper.run();
      }

      if (initState == 99) { // Timed out looking for zero do nothing
        stepper.disableOutputs();
      }
    }
};

/* modify below this line */

/* define stepper parameters
   multiple Vid60Stepper instances can share the same StepperConfig object */
struct StepperConfig stepperConfig = {
  720,  // maxSteps
  200, // maxSpeed
  50 // acceleration
};


// define AccelStepper instance
AccelStepper stepper(AccelStepper::FULL4WIRE, 11, 9, 10, 8);

// define Vid60Stepper class that uses the AccelStepper instance defined in the line above
//           v-- arbitrary name
// Vid60Stepper alt100ftPointer(0x107e,          // address of stepper data
Vid60Stepper standbyCompass(0x0436,          // address of stepper data
                            stepper,         // name of AccelStepper instance
                            stepperConfig,   // StepperConfig struct instance
                            12,              // IR Detector Pin (must be LOW in zero position)
                            0,               // zero offset
[](unsigned int newValue) -> unsigned int {
  /* this function needs to map newValue to the correct number of steps */

  // For most guages this map will do
  //return map(newValue, 0, 65535, 0, stepperConfig.maxSteps - 1);

  // For the compass we only has 360 degrees and need to exclude upper part
  // of 16 bit value
  //Output Type: integer Address: 0x0436 Mask: 0x01ff Shift By: 0 Max. Value: 360 Description: Heading (Degrees)
  // so instead of 0 to 65000 its 0 to 360. Need to exclude upper part of 16 bit value
  newValue = newValue & 0x01ff;
  return map(newValue, 0, 360, 0, stepperConfig.maxSteps - 1);
});


void setup() {
  DcsBios::setup();
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  pinMode(13, OUTPUT);
  pinMode(LedMonitorPin, OUTPUT);
  outputstate = true;
  digitalWrite(LedMonitorPin,outputstate);

  nextupdate = millis() + flashinterval;
  
}

void loop() {
  //  PORTB |= (1 << 5);
  //  PORTB &= ~(1 << 5);
  DcsBios::loop();

 if (millis() >= nextupdate) {
    outputstate = !outputstate;
    digitalWrite(LedMonitorPin, outputstate);
    nextupdate = millis() + flashinterval;
 }

}
