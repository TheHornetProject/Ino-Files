#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 6
#define DATA_PIN 7
#define BRIGHTNESS 255

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
      
      FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS); // ordering is typical
}

void loop() {

FastLED.setBrightness(64);

  //AOA Lights


// Turn the LED on, then pause

  leds[0] = CRGB::Red;
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(200);

//Turn the LED on, then pause
//  leds[1] = CRGB::Orange;
//  leds[1]= 0x35ab02;
//    leds[1].setRGB(25, 255, 5);
     leds[1] =  0x285500; //nice lighter orange
  
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  leds[1] = CRGB::Black;
  FastLED.show();
  delay(200);

 // Turn the LED on, then pause
  leds[2] =0x00640A;
  FastLED.show();
  delay(500);
   
  // Now turn the LED off, then pause
  leds[2] = CRGB::Black;
  FastLED.show();
  delay(200);
  

//LOCK and Shoot
   
// Turn the LED on, then pause
  leds[3] = CRGB::Red;
  FastLED.show();
  delay(500);

  // Now turn the LED off, then pause
  leds[3] = CRGB::Black;
  FastLED.show();
  delay(200);

 // Turn the LED on, then pause
  leds[4] = CRGB::Red;
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  leds[4] = CRGB::Black;
  FastLED.show();
  delay(200);
  
 // Turn the LED on, then pause
 FastLED.setBrightness(255);
  leds[5] = CRGB::White;
  FastLED.show();
  delay(100);
  // Now turn the LED off, then pause
  leds[5] = CRGB::Black;
  FastLED.show();
  delay(100);
   FastLED.setBrightness(255);
  leds[5] = CRGB::White;
  FastLED.show();
  delay(100);
  // Now turn the LED off, then pause
  leds[5] = CRGB::Black;
  FastLED.show();
  delay(100);
   FastLED.setBrightness(255);
  leds[5] = CRGB::White;
  FastLED.show();
  delay(100);
  // Now turn the LED off, then pause
  leds[5] = CRGB::Black;
  FastLED.show();
  delay(100);
   FastLED.setBrightness(255);
  leds[5] = CRGB::White;
  FastLED.show();
  delay(100);
  // Now turn the LED off, then pause
  leds[5] = CRGB::Black;
  FastLED.show();
  delay(100);
   FastLED.setBrightness(255);
  leds[5] = CRGB::White;
  FastLED.show();
  delay(100);
  // Now turn the LED off, then pause
  leds[5] = CRGB::Black;
  FastLED.show();
  delay(100);


}
