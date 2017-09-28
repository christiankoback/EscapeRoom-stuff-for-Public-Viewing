/*
  nfcArduino.ino   ---- Escape Room 1 Season 1
Purpose:  - manage reading nfc cards and notifying RPi on which card was tapped
          - transmit specific pwm for cat5 puzzle

Main Programmer: Chris Koback
                 Oles Shnurovskyy
*/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Adafruit_NeoPixel.h>  //led ring functionality

//bluetooth stuff
#ifndef BT_SETUP__
#define BT_SETUP__
const int btCodeLength = 3;
byte *BTCode;

void sendBTCode(char * strWord,int strLength){
  Serial1.write( "0" ); //garbage
  for (int i = 0; i < strLength; i++){
    Serial1.write( strWord[i] );
  }
  delay(30);  
}

byte* getBTCode(int strLength){
  byte message[strLength];
  if  (Serial1.available() ){
    //char garbage = Serial1.read();
    while(Serial1.available() ){
      for (int i = 0; i < strLength ; i ++){
        message[i] = Serial1.read();
      }
    }
  }
  else{
    message[0] = ' ';
  }
  return message;
}
#endif

/*nfc stuff */
class manageCards {
  public:
    void doCard0 ();
    void doCard1 ();
    void doCard2 ();
    void doCard3 (); 
    void resetCards();
    void cardSetup();
   // typedef for class function
   typedef void (manageCards::*GeneralFunction) ();
   static const GeneralFunction doCardAction [4];
}; 

//testing info
int pinLed = 22;
int pinLed2 = 23;
char cardTech = '1';
int codeLength = 3;

void manageCards::doCard0 ( )
{
  //digitalWrite(pinLed, HIGH);
  //Serial.println("tag 1 was tapped");
  char message[codeLength] = {cardTech, '0','1'};
  sendBTCode(message,codeLength);
}
void manageCards::doCard1 ( )
{
  //digitalWrite(pinLed2, HIGH);
  //Serial.println("tag 2 was tapped");
  char message[codeLength] = {cardTech, '0','2'};
  sendBTCode(message,codeLength);
}
void manageCards::doCard2 ( )
{
  //digitalWrite(pinLed, HIGH);
  //digitalWrite(pinLed2, HIGH);
  //Serial.println("Mifare Classic 1 card was tapped");
  char message[codeLength] = {cardTech, '0','3'};
  sendBTCode(message,codeLength);
}
void manageCards::resetCards(){
  //digitalWrite(pinLed, LOW);
  //digitalWrite(pinLed2, LOW);
}
void manageCards::cardSetup(){
  //pinMode(pinLed, OUTPUT);
  //pinMode(pinLed2, OUTPUT);  
  //digitalWrite(pinLed, LOW);
  //digitalWrite(pinLed2, LOW);  

  #ifndef SERIAL1_SETUP__
  #define SERIAL1_SETUP__
  Serial1.begin(9600);    //bluetooth setup
  #endif
}
 // array of function pointers
const manageCards::GeneralFunction manageCards::doCardAction[3] =
  {
    &manageCards::doCard0, 
    &manageCards::doCard1, 
    &manageCards::doCard2
  };

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

uint16_t validCards[] = { 650 , 761, 276 };
uint16_t cardUID;
uint8_t cardNum = 3;


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

//init cardManager class
manageCards cardManager;

/*end of nfc init */
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
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

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


 /*end of neopixel ring stuff  */

 
char chestButtonCode[] = "316";
int chestButtonPin = 13;
int chestButtonPrevValue = 0;

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

void setupCat5Puzzle(){
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

/* end of cat5 puzzle stuff */



void setup() {
  Serial.begin(9600);   //testing
  
  nfc.begin();
  nfc.SAMConfig();
  nfc.setPassiveActivationRetries(0x1F);  // make the nfc more like a non-blocking function - try finding card 32 times

  cardManager.manageCards::cardSetup();
  setupCat5Puzzle();
  pinMode(chestButtonPin, INPUT_PULLUP);

  #ifndef SERIAL1_SETUP__
  #define SERIAL1_SETUP__
  Serial1.begin(9600);    //bluetooth setup
  #endif
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  for(int i = 0 ; i < 12 ; i++){      //set complete ring to green color
    strip.setPixelColor(i, strip.Color(0,255, 0,0 ) );
  }
  strip.show();
}

void loop() {
  if  (Serial1.available() ){
    BTCode = getBTCode(btCodeLength);
    if (BTCode[0] != ' '  ){
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
              int patternToDo = codeNum - (ledRingBase+1);
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
         else{}
        }
      }
    }
  }
  if ( cardUID != 0 ){
    //go through all stored nfc cards and check if a valid card is tapped
    for (int i = 0; i < cardNum ; i++){
      if (cardUID == validCards[i]){
        manageCards::GeneralFunction cardAction = cardManager.manageCards::doCardAction[i];
        (cardManager.*cardAction)();
        break;
      }
    }
  }
  
  if( ( chestButtonPin == LOW ) && (chestButtonPrevValue == 0)  ){
    chestButtonPrevValue = 1;
    sendBTCode(chestButtonCode, 3);
  }
  delay(50);
}
