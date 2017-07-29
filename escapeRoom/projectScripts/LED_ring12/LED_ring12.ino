

#include <Adafruit_NeoPixel.h>

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      12

#define PIN            9

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGBW + NEO_KHZ800);
int delayval = 500; // delay for half a second

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
  
  pixels.setPixelColor(4, pixels.Color(0,255,0)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.

  delay(2000);
  
  pixels.setPixelColor(5, pixels.Color(255,0,0)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.

  delay(2000);
  
  pixels.setPixelColor(6, pixels.Color(0,150,150)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  
}

void loop() {
  // put your main code here, to run repeatedly:



}
