#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Adafruit_NeoPixel.h>  //led ring functionality

#include "NFC_card.h"

#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

//To add cards, add all uid hex into 1 value & add to validCards Array
//implement new functions in NFC_card.h (manageCards class)
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

//works if no cards have the same sum
uint16_t validCards[] = {529, 615 , 650 , 305 };
uint16_t cardUID;
uint8_t cardNum = 4;    //4 is hardcoded value in (manageCards class)

int cat5e1_pin = 13;
int cat5e2_pin = 12;
int cat5e3_pin = 11;
int cat5e4_pin = 10;
int cat5e5_pin = 9;

int headLightPin = 6;
int headLight_start = 43;
int headLight_end = 53;

int ledRingBase = 30;
int ledRingEnd = 40;

//init cardManager class
manageCards cardManager;

//bluetooth stuff
#ifndef BT_SETUP__
#define BT_SETUP__
const int btCodeLength = 3;
byte BTCode[btCodeLength];
#endif


#define NUM_LEDS 12
#define PIN 9

//led ring stuff
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGBW + NEO_KHZ800);

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
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

  
char chestButtonCode[] = "316";
int chestButtonPin = 7;
int chestButtonPrevValue = 0;

    
void nfcSetup(){
  // configure board to read RFID tags
  nfc.SAMConfig();
}
uint16_t retrieveUID(){
  uint16_t success;
  uint16_t cardUID = 0;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    for(int i = 0; i < uidLength; i++){
      cardUID += uid[i];
    }
  }
  return cardUID;
}
void setupCat5Puzzle(){
  pinMode(cat5e1_pin, OUTPUT);
  pinMode(cat5e2_pin, OUTPUT);
  pinMode(cat5e3_pin, OUTPUT);
  pinMode(cat5e4_pin, OUTPUT);
  pinMode(cat5e5_pin, OUTPUT);

  analogWrite(cat5e1_pin, 250);
  analogWrite(cat5e1_pin, 120);
  analogWrite(cat5e1_pin, 210);
  analogWrite(cat5e1_pin, 10);
  analogWrite(cat5e1_pin, 50);
}
void turnOffRing(){
  strip.clear();
}

void fadeInColor(char color, int fadeDelay ){
  for(int j = 0; j < 256 ; j++){
    
    for(uint16_t i=0; i < strip.numPixels(); i++) {
      //strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
      if (color =='r'){
        strip.setPixelColor(i, strip.Color(neopix_gamma[j],0, 0, 0 ) );
      }
      else if (color =='b'){
        strip.setPixelColor(i, strip.Color(0,0,neopix_gamma[j] , 0 ) );
      }
      else if (color =='g'){
        strip.setPixelColor(i, strip.Color(0,neopix_gamma[j], 0,0) );
      }
      else if (color =='w'){
        strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
      }
      else{}
      strip.show();
      delay(fadeDelay);
    }
  }
}
void fadeOutColor(char color, int fadeDelay ){
  for(int j = 255; j >= 0 ; j--){

    for(uint16_t i=0; i < strip.numPixels(); i++) {
       if (color =='r'){
        strip.setPixelColor(i, strip.Color(neopix_gamma[j],0, 0, 0 ) );
      }
      else if (color =='b'){
        strip.setPixelColor(i, strip.Color(0,0,neopix_gamma[j] , 0 ) );
      }
      else if (color =='g'){
        strip.setPixelColor(i, strip.Color(0,neopix_gamma[j], 0,0) );
      }
      else if (color =='w'){
        strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
      }
      else{}
      strip.show();
      delay(fadeDelay);
    }
  }
}
void rainbowInRotatingWheel(int rainbowLoops, int rotatingDelayMin, int rotatingDelayIncrement){
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;
  //int temp = rainbowLoops*wait*10;

  for(int k = 0 ; k <= rainbowLoops ; k ++){
    for(int j=256; j >= 0; j--) { // 5 cycles of all colors on wheel

      for(int i=strip.numPixels(); i >= 0; i--) {
        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float((fadeVal*0.9)/fadeMax);
        greenVal = green(wheelVal) * float( (fadeVal*0.9) /fadeMax);
        blueVal = blue(wheelVal) * float((fadeVal*0.9)/fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );
      }

      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) {
        fadeVal++;
      }

      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
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

void rainbowInRotatingWheel2White(int rainbowLoops, int rotatingDelayMin, int rotatingDelayIncrement){
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;
  //int temp = rainbowLoops*wait*10;

  for(int k = 0 ; k <= rainbowLoops ; k ++){
    for(int j=256; j >= 0; j--) { // 5 cycles of all colors on wheel

      for(int i=strip.numPixels(); i >= 0; i--) {
        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float((fadeVal*0.9)/fadeMax);
        greenVal = green(wheelVal) * float( (fadeVal*0.9) /fadeMax);
        blueVal = blue(wheelVal) * float((fadeVal*0.9)/fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );
      }

      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) {
        fadeVal++;
      }

      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
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
  for(int i = 0; i < strip.numPixels() ; i++) {
    //weak white to strong white
    //strip.setPixelColor(i, strip.Color(0,0,0,whiteVal ) );    
    strip.setPixelColor(i, strip.Color(0,0,0,255 ) );
    strip.show();
    delay(50);
    //whiteVal = whiteVal + 19;
  }
}
void changeLEDColor(int ledNum, int redColor = 0, int blueColor = 0, int greenColor= 0, int whiteColor=0){
  strip.setPixelColor( ledNum, strip.Color( redColor, greenColor, blueColor, whiteColor ) );
  updateRing();
}
void changeColor_Uniform(int redColor = 0, int blueColor = 0, int greenColor= 0, int whiteColor=0){
  for ( int i = 0; i <= NUM_LEDS; i ++){
    strip.setPixelColor( i, strip.Color( redColor, greenColor, blueColor, whiteColor ) );
    updateRing();
  }
  
}
void updateRing(){
  strip.show();
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
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
uint8_t white(uint32_t c){
  return (c >> 24);
}



void setup() {
  Serial.begin(9600);
  //Serial.println("Hello!");   //testing
  nfc.begin();
  nfcSetup();
  
  cardManager.manageCards::cardSetup();
  setupCat5Puzzle();
  pinMode(chestButtonPin, INPUT_PULLUP);

  #ifndef SERIAL1_SETUP__
  #define SERIAL1_SETUP__
  Serial1.begin(9600);    //bluetooth setup
  #endif

}

void loop() {
/*    //testing headlight
  analogWrite(headLightPin, 255);
  delay(1000);
  analogWrite(headLightPin, 10);
  delay(1000);
  */
  
  //if bluetooth is used, get all data
  if  (Serial1.available() ){
      int CodeCount = 0;
      while(Serial1.available() ){
        BTCode[CodeCount] = Serial1.read();
        CodeCount ++;
      }
      if (CodeCount >= btCodeLength  ){
        //invalid Bluetooth code
      }
      
      int completeCode = 0;
      for (int i = 0; i < btCodeLength; i ++){
        completeCode = ( BTCode[i] - '0')  * pow(10, btCodeLength - (i+1) );
      }
      if (completeCode == 320){
        //reset for chest button
        chestButtonPrevValue = 0;
      }
      else{
        int techNum = BTCode[0] -'0';
        int codeNum = ( (BTCode[1] - '0') * 10 ) + (BTCode[2]- '0');

        if (techNum == 2 ){
          if ( (codeNum > ledRingBase ) && (codeNum <= ledRingEnd ) ){
              //do associated ledRing function
              int patternToDo = codeNum - ledRingBase;
              if (patternToDo == 0){
                //do pattern 1, turn off led ring
                turnOffRing();
              }
              else if (patternToDo == 1){
                //do pattern 2
                rainbowInRotatingWheel2White(5,10,10);
              }
              else if (patternToDo == 2){
                //do pattern 3
                fadeInColor('b',2);
              }
              else{
                //still under construction
              }
            }
         else if ( (codeNum >= headLight_start ) && (codeNum <= headLight_end ) ){
            int functionCode = codeNum - headLight_start;
            if (functionCode == 0){
              //turn off headlight
              analogWrite(headLightPin, 0);
            }
            else if (functionCode == 1){
              analogWrite(headLightPin, 255);
            }
            else if (functionCode == 2){
              analogWrite(headLightPin, 127);
            }
            else{
              //under construction
            }
         }
         else{}
        }
      }
    }
    
  uint16_t cardUID = retrieveUID();
  if ( cardUID != 0 ){
    //go through all stored nfc cards and check if a valid card is tapped
    for (int i = 0; i < cardNum ; i++){
      if (cardUID == validCards[i]){
        manageCards::GeneralFunction cardAction = cardManager.manageCards::doCardAction[i];
        (cardManager.*cardAction)();
        break;
      }
    }
    //reset all actions
    cardManager.manageCards::resetCards();
  }
  if( ( chestButtonPin == LOW ) && (chestButtonPrevValue == 0)  ){
    chestButtonPrevValue = 1;
    for(int i = 0; i < btCodeLength; i++){
      Serial1.write( chestButtonCode[i] );
    }  
  }
  delay(500);  //half a second delay between loops
}
