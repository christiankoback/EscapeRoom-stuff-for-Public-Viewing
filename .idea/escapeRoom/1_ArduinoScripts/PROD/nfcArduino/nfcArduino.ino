#include <Adafruit_NeoPixel.h>
#include<stdio.h>

/*
  nfcArduino.ino   ---- Escape Room 1 Season 1
  Purpose:  - manage reading nfc cards and notifying RPi on which card was tapped
            - neopixel ring
          - transmit specific pwm for cat5 puzzles
*/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>  //led ring functionality

// Bluetooth variables/setupbles
String inData;
String token;

/*nfc stuff */

#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

/* end of nfc stuff */

/*init nfc necessary variables  */
//To add cards, add all uid hex into 1 value & add to validCards Array
//implement new functions in NFC_card.h (manageCards class)
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

//card UID Sum --- works if no cards have the same sum
//red bus pass = 650
// cloth/flimsy bus pass = 761
// blue bus pass = 276
uint16_t cardUID;

uint16_t retrieveUID() {
  uint16_t success;
  uint16_t cardUID = 0;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    for (int i = 0; i < uidLength; i++) {
      cardUID += uid[i];
    }
  }
  return cardUID;
}

/*end of nfc stuff */

/* neopixel ring stuff  */
int ledRingBase = 30;
int ledRingEnd = 40;

#define NUM_LEDS 12
#define PIN 5
#define BRIGHTNESS 40
//led ring stuff
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

byte neopix_gamma[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

void turnOffRing() {
  strip.clear();
}

void fadeInColor(char color, int fadeDelay ) {
  for (int j = 0; j < 256 ; j++) {

    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      //strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
      if (color == 'r') {
        strip.setPixelColor(i, strip.Color(neopix_gamma[j], 0, 0, 0 ) );
      }
      else if (color == 'b') {
        strip.setPixelColor(i, strip.Color(0, 0, neopix_gamma[j] , 0 ) );
      }
      else if (color == 'g') {
        strip.setPixelColor(i, strip.Color(0, neopix_gamma[j], 0, 0) );
      }
      else if (color == 'w') {
        strip.setPixelColor(i, strip.Color(0, 0, 0, neopix_gamma[j] ) );
      }
      else {}
      strip.show();
      delay(fadeDelay);
    }
  }
}
void fadeOutColor(char color, int fadeDelay ) {
  for (int j = 255; j >= 0 ; j--) {

    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      if (color == 'r') {
        strip.setPixelColor(i, strip.Color(neopix_gamma[j], 0, 0, 0 ) );
      }
      else if (color == 'b') {
        strip.setPixelColor(i, strip.Color(0, 0, neopix_gamma[j] , 0 ) );
      }
      else if (color == 'g') {
        strip.setPixelColor(i, strip.Color(0, neopix_gamma[j], 0, 0) );
      }
      else if (color == 'w') {
        strip.setPixelColor(i, strip.Color(0, 0, 0, neopix_gamma[j] ) );
      }
      else {}
      strip.show();
      delay(fadeDelay);
    }
  }
}
void rainbowInRotatingWheel(int rainbowLoops, int rotatingDelayMin, int rotatingDelayIncrement) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;
  //int temp = rainbowLoops*wait*10;

  for (int k = 0 ; k <= rainbowLoops ; k ++) {
    for (int j = 256; j >= 0; j--) { // 5 cycles of all colors on wheel

      for (int i = strip.numPixels(); i >= 0; i--) {
        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float((fadeVal * 0.9) / fadeMax);
        greenVal = green(wheelVal) * float( (fadeVal * 0.9) / fadeMax);
        blueVal = blue(wheelVal) * float((fadeVal * 0.9) / fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );
      }

      //First loop, fade in!
      if (k == 0 && fadeVal < fadeMax - 1) {
        fadeVal++;
      }

      //Last loop, fade out!
      else if (k == rainbowLoops - 1 && j > 255 - fadeMax ) {
        fadeVal--;
      }
      strip.show();
      delay(rotatingDelayMin);
      rotatingDelayMin += rotatingDelayIncrement;
      //delay(temp);
      //temp = temp * 0.9 ;
    }
  }
}
void rainbowInRotatingWheel2White(int rainbowLoops, int rotatingDelayMin, int rotatingDelayIncrement) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;
  //int temp = rainbowLoops*wait*10;

  for (int k = 0 ; k <= rainbowLoops ; k ++) {
    for (int j = 256; j >= 0; j--) { // 5 cycles of all colors on wheel

      for (int i = strip.numPixels(); i >= 0; i--) {
        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float((fadeVal * 0.9) / fadeMax);
        greenVal = green(wheelVal) * float( (fadeVal * 0.9) / fadeMax);
        blueVal = blue(wheelVal) * float((fadeVal * 0.9) / fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );
      }

      //First loop, fade in!
      if (k == 0 && fadeVal < fadeMax - 1) {
        fadeVal++;
      }

      //Last loop, fade out!
      else if (k == rainbowLoops - 1 && j > 255 - fadeMax ) {
        fadeVal--;
      }
      strip.show();
      delay(rotatingDelayMin);
      rotatingDelayMin += rotatingDelayIncrement;
      //delay(temp);
      //temp = temp * 0.9 ;
    }
  }
  //int whiteVal = 19;
  for (int i = 0; i < strip.numPixels() ; i++) {
    //weak white to strong white
    //strip.setPixelColor(i, strip.Color(0,0,0,whiteVal ) );
    strip.setPixelColor(i, strip.Color(0, 0, 0, 255 ) );
    strip.show();
    delay(50);
    //whiteVal = whiteVal + 19;
  }
}
void changeLEDColor(int ledNum, int redColor = 0, int blueColor = 0, int greenColor = 0, int whiteColor = 0) {
  strip.setPixelColor( ledNum, strip.Color( redColor, greenColor, blueColor, whiteColor ) );
  updateRing();
}
void changeColor_Uniform(int redColor = 0, int blueColor = 0, int greenColor = 0, int whiteColor = 0) {
  for ( int i = 0; i <= NUM_LEDS; i ++) {
    strip.setPixelColor( i, strip.Color( redColor, greenColor, blueColor, whiteColor ) );
    updateRing();
  }

}
void updateRing() {
  strip.show();
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}
uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}
uint8_t white(uint32_t c) {
  return (c >> 24);
}


/*end of neopixel ring stuff  */


char chestButtonCode[] = "316";
int chestButtonPin = 13;
int chestButtonPrevValue = 0;
int isButtonAlreadyTriggered = 0;
int chestClosedCountResetNum = 10;      //number of 'chest is closed' before chest is closed check is sent (done on reset via Pi)

/* cat5 puzzle -> transmit pwm signal    stuff*/
int cat5e1_pin = 12;
int cat5e2_pin = 11;
int cat5e3_pin = 10;
int cat5e4_pin = 9;
int cat5e5_pin = 8;

//random values -- corresponds with PWM pulses
int cable1_value = 250;
int cable2_value = 120;
int cable3_value = 210;
int cable4_value = 10;
int cable5_value = 50;

void setupCat5Puzzle() {
  //by default, set all cat5 cables to transmit their specific pwm signal
  pinMode(cat5e1_pin, OUTPUT);
  pinMode(cat5e2_pin, OUTPUT);
  pinMode(cat5e3_pin, OUTPUT);
  pinMode(cat5e4_pin, OUTPUT);
  pinMode(cat5e5_pin, OUTPUT);

  analogWrite(cat5e1_pin, cable1_value);
  analogWrite(cat5e2_pin, cable2_value);
  analogWrite(cat5e3_pin, cable3_value);
  analogWrite(cat5e4_pin, cable4_value);
  analogWrite(cat5e5_pin, cable5_value);
}

int cardToSend = 0;
  
void setup() {
  Serial.begin(9600);   //testing
  Serial1.begin(9600);    //bluetooth setup

  nfc.begin();
  nfc.SAMConfig();
  nfc.setPassiveActivationRetries(0x1F);  // make the nfc more like a non-blocking function - try finding card 32 times

  //cardManager.manageCards::cardSetup();
  //setupCat5Puzzle();
  pinMode(chestButtonPin, INPUT_PULLUP);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  for (int i = 0 ; i < 12 ; i++) {    //set complete ring to green color
    strip.setPixelColor(i, strip.Color(0, 255, 0, 0 ) );
  }
  strip.show();

}

void loop() {

  if (Serial1.available() > 0) {
    inData = Serial1.readStringUntil('\n');
    //Serial.println("inData>" + inData + "<");

    while (inData.length() > 2) {
      token = inData.substring(0, 3);
      inData = inData.substring(3, inData.length());
      //Serial.println("token>" + token + "<");
      //Serial.println("inData>" + inData + "<");
      //Serial.println("");

      if (token == "000") {                          // Reply to alive request DO NOT DELETE
        //Serial.println("");
        Serial1.println("!");

      } if (token == "231") {                       //
        turnOffRing();

      } else if (token == "232") {                    //
        rainbowInRotatingWheel2White(5, 10, 10);

      } else if (token == "233") {                    //
        fadeInColor('b', 2);

      } else if (token == "999") {                    // RESET
        chestButtonPrevValue = 0;
        isButtonAlreadyTriggered = 0;
        int chestClosedCount = 0;
        while(chestClosedCount < chestClosedCountResetNum){        
          if ( digitalRead(chestButtonPin) == LOW) {
            chestClosedCount ++;
          }
        }
        if (chestClosedCount == chestClosedCountResetNum){
          Serial1.println("997");
        }
        strip.setBrightness(BRIGHTNESS);
        strip.begin();
        strip.show(); // Initialize all pixels to 'off'
        for (int i = 0 ; i < 12 ; i++) {    //set complete ring to green color
          strip.setPixelColor(i, strip.Color(0, 255, 0, 0 ) );
        }
        strip.show();
      
      } else {
        Serial1.println("!");
      }
    } //end while inData
  } // end if serial available

    //go through all stored nfc cards and check if a valid card is tapped
    cardUID = retrieveUID();
    cardToSend = 0;
    if (cardUID == 650) { // Blue
      cardToSend = 101;
      strip.show(); // Initialize all pixels to 'off'
      for (int i = 0 ; i < 12 ; i++) {    //set complete ring to blue color
        strip.setPixelColor(i, strip.Color(0, 0, 255, 0 ) );
      }
      strip.show();
      Serial.println("Card 1");
    }
    else if (cardUID == 761) { // Red
      cardToSend = 102;
      strip.show(); // Initialize all pixels to 'off'
      for (int i = 0 ; i < 12 ; i++) {    //set complete ring to red color
        strip.setPixelColor(i, strip.Color(255, 0, 0, 0 ) );
      }
      strip.show();
      Serial.println("Card 2");
    }
    else if (cardUID == 276) { // Space - White
       cardToSend = 103; 
       strip.show(); // Initialize all pixels to 'off'
       for (int i = 0 ; i < 12 ; i++) {    //set complete ring to white color
        strip.setPixelColor(i, strip.Color(255, 255, 255, 0 ) );
       }
       strip.show();
       Serial.println("Card 3");
    } else {
    }
    if (cardToSend != 0){
      Serial1.println(cardToSend);
      Serial.println(">" + cardToSend);
      delay(3000);// 1 second = 1000
      strip.show(); // Initialize all pixels to 'off'
      for (int i = 0 ; i < 12 ; i++) {    //set complete ring to green color
        strip.setPixelColor(i, strip.Color(0, 255, 0, 0 ) );
      }
      strip.show();
    }
    cardUID = 0;    //reset the variable to remove chance of reading same card multiple times
    

  //checks if chest in box is open, chest is in "open" state when lock is in a specific position. 
  //"must be open more than 10 times for it to be officially open"
  if(  (digitalRead(chestButtonPin) == HIGH) && ( isButtonAlreadyTriggered == 0) ) {
    chestButtonPrevValue ++;
  }
  if ( (chestButtonPrevValue > 10 ) && (isButtonAlreadyTriggered == 0)){
    //chest is open
    isButtonAlreadyTriggered = 1;
    strip.show(); // Initialize all pixels to 'off'
    for (int i = 0 ; i < 12 ; i++) {    //set complete ring to blue color
      strip.setPixelColor(i, strip.Color(0, 0, 255, 0 ) );
    }
    strip.show();
    Serial1.println("250");
    delay(1000);
    for (int i = 0 ; i < 12 ; i++) {    //set complete ring to blue color
      strip.setPixelColor(i, strip.Color(0, 255, 0, 0 ) );
    }
    strip.show();
  }
  
} // end loop

