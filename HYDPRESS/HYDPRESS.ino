#define DCSBIOS_IRQ_SERIAL

#include <DcsBios.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned int lHydo = 0; //Left Hydo
unsigned int rHydo = 0; //Right Hydo

//Left Hydo
void onHydIndLeftChange(unsigned int newValue) {
{
  lHydo = map(newValue, 0, 65535, 00 , 60);
}

 display.setCursor(27,29);
 display.print("L");
 display.setCursor(20,49);
 display.print(lHydo);

 display.display();
}

DcsBios::IntegerBuffer hydIndLeftBuffer(0x751e, 0xffff, 0, onHydIndLeftChange);

//Right Hydo
void onHydIndRightChange(unsigned int newValue) {
{  
  rHydo = map(newValue, 0, 65539, 00 , 60);
}

 display.setCursor(87,29);
 display.print("R");
 display.setCursor(80,49);
 display.print(rHydo);
 
 display.display();
}

DcsBios::IntegerBuffer hydIndRightBuffer(0x7520, 0xffff, 0, onHydIndRightChange);

void setup() {
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
 display.clearDisplay();
 display.setTextSize(2);
 display.setTextColor(WHITE,BLACK); // Draw white text
 display.setCursor(10,7);
 display.print("HYD PRESS");

 DcsBios::setup();
}

void loop() {
 DcsBios::loop();
}
