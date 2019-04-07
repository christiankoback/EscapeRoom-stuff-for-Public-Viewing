//meant for main arduino
#include <avr/pgmspace.h>  //AVR library for writing to ROM
#include <Charliplexing.h> //Imports the library, which needs to be
                           //Initialized in setup.
#include <Keypad.h>

//meant for nfc arduino
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Adafruit_NeoPixel.h>    //for LED_Ring12 class
#ifdef __AVR__
  #include <avr/power.h>
#endif


#define NUM_LEDS 12
#define PIN 5
#define BRIGHTNESS 40

//keypad testing info
//global variables
//keypad setup variables
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {29, 28, 27, 26}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {25, 24, 23, 22}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//keypad helper functions
String keypadAnswer = "6175a4d";
String tempKeypadAnswer = "";
int keypadPassIsComplete = 0;
void restartKeypad(){
  tempKeypadAnswer = "";
  keypadPassIsComplete = 0;
}
void checkKeypadOutput(){
  if( (tempKeypadAnswer == keypadAnswer) && (keypadPassIsComplete == 0 ) ){
    keypadPassIsComplete = 1; //correct answer
  }
  if (keypadPassIsComplete != 1){
    char key = keypad.getKey();
    if (key){
      while(key){
        Serial.print("key on keypad:");
        Serial.println(key);
        if ( key == keypadAnswer[tempKeypadAnswer.length()] ){
          tempKeypadAnswer.concat(key);
        }
        else{
          restartKeypad();
        }
        key = keypad.getKey();
      }
    }
  }
}




//lolshield puzzle test
const int lolShieldPatternLength = 3;
String lolShieldPatterns[lolShieldPatternLength] = {"whic" , "hplanet" ,"hasoxygen?" };
int lolShieldPatternIndex = 0;

//lolshield testing info
String charPart0= "mskaigf";
String charPart1 = "qxdo?nh";
String charPart2 = "et1brvw";
String charPart3 = "upjycz4";
String charPart4 = "2519638";
int brightness = 7;
void initLoLShield(){
  LedSign::Init(GRAYSCALE);
}
void ClearShield(){
  LedSign::Clear();
}
void  turnOnLetter(char letter){
  Serial.print("printing letter:");
  Serial.println(letter);
  int index = 0;
  int arrayNum = 0;
  if ( (index = charPart0.indexOf(letter) )== -1){
    if ( (index = charPart1.indexOf(letter) )== -1){
      if ( (index = charPart2.indexOf(letter) )== -1){
        if ( (index = charPart3.indexOf(letter) )== -1){
          // character does not exist
          return;
        }
        arrayNum += 1;
      }
      arrayNum += 1;
    } 
    arrayNum += 1;
  }

  Serial.print("index:");
  Serial.println(index);

  Serial.print("arrayNum:");
  Serial.println(arrayNum);
  int column1;
  int led0,led1;
  if (index == 0){
    led0 = 0;
    led1 = 1;
  }
  if (index == 1){
    led0 = 2;
    led1 = 3;
  }
  if (index == 2){
    led0 = 4;
    led1 = 5;
  }
  if (index == 3){
    led0 = 6;
    led1 = 7;
  }if (index == 4){
    led0 = 8;
    led1 = 9;
  }
  if (index == 5){
    led0 = 10;
    led1 = 11;
  }
  if (index == 6){
    led0 = 12;
    led1 = 13;
  }

  if (arrayNum != 0){
    column1 = 0 + (2 * arrayNum);
  }
  else{
    column1 = 0;
  }
  for (int i = 0; i < 2; i++){
    LedSign::Set(led0 , column1 + i, brightness);
    LedSign::Set(led1 , column1 + i, brightness);
  }
}
void  turnOnSentence(String sentence){
  for(int i = 0; i < sentence.length(); i++){
    turnOnLetter(sentence[i]);
  }
}

//cat5 puzzle  input
int cat5_1_IN = A11;
int cat5_2_IN = A12;
int cat5_3_IN = A13;
int cat5_4_IN = A14;
int cat5_5_IN = A15;

//glowing wire pins
//pins for glowing wire
int glOrange = 32;
int glBlue = 33;
int glRed = 36;
int glPink = 35;
int glGreen = 34;




//led ring testing
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



// cat5 pins - nfc ARDuino
int cat5e1_pin = 12;
int cat5e2_pin = 11;
int cat5e3_pin = 10;
int cat5e4_pin = 9;
int cat5e5_pin = 8;
//cat5 helper functions
void setupCat5Puzzle(){
  pinMode(cat5e1_pin, OUTPUT);
  pinMode(cat5e2_pin, OUTPUT);
  pinMode(cat5e3_pin, OUTPUT);
  pinMode(cat5e4_pin, OUTPUT);
  pinMode(cat5e5_pin, OUTPUT);

  analogWrite(cat5e1_pin, 250);
  analogWrite(cat5e2_pin, 120);
  analogWrite(cat5e3_pin, 210);
  analogWrite(cat5e4_pin, 10);
  analogWrite(cat5e5_pin, 50);
}




//chest closing testing
int chestButtonPin = 13;

//cube headlight/main light
int headLightPin = 6;



//nfc testing
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

//To add cards, add all uid hex into 1 value & add to validCards Array
//implement new functions in NFC_card.h (manageCards class)
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

//works if no cards have the same sum
//card sums
// 236 +215 +172 + 27  -> red bus pass = 650
//4  + 171 + 28 + 210 + 156 + 64 + 128 -> cloth bus pass = 761
// 236  + 35  +  0 +  5   -> blue bus pass = 276

uint16_t validCards[] = { 650 , 761, 276 };
uint16_t cardUID;
uint8_t cardNum = 3;    //4 is hardcoded value in (manageCards class)
//nfc helper functions
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
  Serial.println("tag 1 was tapped");
}
void manageCards::doCard1 ( )
{
  //digitalWrite(pinLed2, HIGH);
  Serial.println("tag 2 was tapped");
}
void manageCards::doCard2 ( )
{
  //digitalWrite(pinLed, HIGH);
  //digitalWrite(pinLed2, HIGH);
  Serial.println("Mifare Classic 1 card was tapped");
}
void manageCards::doCard3 ( )
{
  //digitalWrite(pinLed, HIGH);
  //digitalWrite(pinLed2, HIGH);
  Serial.println("Mifare Classic 2 card was tapped");
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
}
 // array of function pointers
const manageCards::GeneralFunction manageCards::doCardAction[4] =
  {
    &manageCards::doCard0, 
    &manageCards::doCard1, 
    &manageCards::doCard2, 
    &manageCards::doCard3
  };

//init cardManager class
manageCards cardManager;

  

int glowingWireTestingIndex = 0;
int ledRingTestingIndex = 0;
void setup() {
  Serial.begin(9600);   //testing
/*
  //meant for lol shield arduino
  restartKeypad();    //init keypad
  initLoLShield();    //init lol shield
  
  //glowing wire test
  pinMode(glOrange , OUTPUT);
  pinMode(glBlue , OUTPUT);
  pinMode(glPink , OUTPUT);
  pinMode(glRed , OUTPUT);
  pinMode(glGreen , OUTPUT);

*/
  //meant for nfc arduino
  // configure board to read RFID tags
  nfc.begin();
  nfc.SAMConfig();
  nfc.setPassiveActivationRetries(0x1f);
  
  setupCat5Puzzle();
  pinMode(chestButtonPin, INPUT_PULLUP);
  //analogWrite(headLightPin, 0); //testing, obsolete
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  for(int i = 0 ; i < 12 ; i++){
    strip.setPixelColor(i, strip.Color(0,255, 0,0 ) );
  }
  strip.show();

  
}

void loop() {
  /*
  // meant for main cube arduino
  //testing lol shield
  //change/increment the pattern on display
  if (lolShieldPatternIndex == lolShieldPatternLength ){
    lolShieldPatternIndex = 0;
  }
  ClearShield();
  turnOnSentence( lolShieldPatterns[lolShieldPatternIndex] );
  lolShieldPatternIndex++;

  //  glowing wire testing
  if (glowingWireTestingIndex >= 10){
    glowingWireTestingIndex = 0;
  }
  if ( (glowingWireTestingIndex % 2) == 0 ){
    digitalWrite(glOrange  , HIGH);
    digitalWrite(glBlue  , HIGH);
    digitalWrite(glPink  , HIGH);
    digitalWrite(glRed  , HIGH);
    digitalWrite(glGreen  , HIGH);
  }
  else{
    digitalWrite(glOrange  , LOW);
    digitalWrite(glBlue  , LOW);
    digitalWrite(glPink  , LOW);
    digitalWrite(glRed  , LOW);
    digitalWrite(glGreen  , LOW);
  }
  glowingWireTestingIndex++;

   //cat5 cable reading
  int cat51 = pulseIn(cat5_1_IN,HIGH);
  int cat52 = pulseIn(cat5_2_IN,HIGH);
  int cat53 = pulseIn(cat5_3_IN,HIGH);
  int cat54 = pulseIn(cat5_4_IN,HIGH);
  int cat55 = pulseIn(cat5_5_IN,HIGH);
  
  Serial.print("cat5-1 cable value = ");
  Serial.println (cat51);
  Serial.print("cat5-2 cable value = ");
  Serial.println (cat52);
  Serial.print("cat5-3 cable value = ");
  Serial.println (cat53);
  Serial.print("cat5-4 cable value = ");
  Serial.println (cat54);
  Serial.print("cat5-5 cable value = ");
  Serial.println (cat55);
  Serial.println(' ');
  Serial.println(' ');

  //testing keypad 4x4 membrane
  checkKeypadOutput();
  delay(1000);
  
*/

 
  //meant for nfc arduino
  //strip.setBrightness(int brightness);      // 0 <= brightness <= 255
  /*
    
   Serial.println("nfc arduino testing");
  if (ledRingTestingIndex >= 4){
    ledRingTestingIndex = 0;
  }
  
  if (ledRingTestingIndex == 0 ){
    rainbowInRotatingWheel(3, 1, 1);
    strip.clear();
    strip.show();
  }
  else if (ledRingTestingIndex == 1 ){
  
    fadeInColor('b', 2);
    fadeOutColor('b', 2);
    strip.clear();
    strip.show();
  }
  else if (ledRingTestingIndex == 2 ){
    rainbowInRotatingWheel2White(5, 3, 3);
    strip.clear();
    strip.show();
  }
  else if (ledRingTestingIndex == 3 ){
    //int redColor = 0, int blueColor = 0, int greenColor= 0, int whiteColor=0
    changeColor_Uniform(125, 125, 125,0);
    strip.clear();
    strip.show();
  }else{}

  ledRingTestingIndex++;
  */
  
  
  //testing if chest is closed
  if (digitalRead(chestButtonPin) == LOW){
    Serial.println("Chest is closed");
  }else if (digitalRead(chestButtonPin) == HIGH){
    Serial.println("Chest is open");
  }

  //testing nfc reader
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
  }

}
