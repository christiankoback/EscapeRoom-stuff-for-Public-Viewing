#include <LEDStrip.h>
#include <Keypad.h>
#include <avr/pgmspace.h>  //AVR library for writing to ROM
#include <Charliplexing.h> //Imports the library, which needs to be
                           //Initialized in setup.
#include "glowingWire.h"

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
String keypadAnswer = "1389527526";
String tempKeypadAnswer = "";
int keypadPassIsComplete = 0;
  
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

/*
//pins for glowing wire
int glOrange = 32;
int glBlue = 33;
int glRed = 34;
int glPink = 35;
int glGreen = 36;
*/

//pins for cat5
int cat5_1_IN = A11;
int cat5_2_IN = A12;
int cat5_3_IN = A13;
int cat5_4_IN = A14;
int cat5_5_IN = A15;

//3 black meets 2 white  <= cat5_1 black transmit
const int CAT5CABLENUM = 5; 
int cat5Puzzle_pins[CAT5CABLENUM] = {cat5_1_IN, cat5_2_IN, cat5_3_IN, cat5_4_IN, cat5_5_IN};
int cat5Puzzle_answersLow[CAT5CABLENUM] = { 1988, 954, 1668  ,74, 394};
int cat5Puzzle_answersHigh[CAT5CABLENUM] = { 1994, 955 , 1674, 81,401};
int cat5Puzzle_tempAnswer[CAT5CABLENUM] = {0, 0 , 0, 0, 0};

//random values -- corresponds with PWM pulses 
int cable1_value = 230;
int cable2_value = 70;
int cable3_value = 4;
int cable4_value = 1025;
int cable5_value = 405;

//init glowing wire manager class
manageGlowingWire glowingWireManager;

//init LoL shield manager
const int lolShieldPatternLength = 3;
String lolShieldPatterns[lolShieldPatternLength] = {"whic" , "hplanet" ,"hasoxygen?" };
int lolShieldPatternIndex = 0;


//code vardiables
int glowingWireCodeBase = 21;
int glowingWireCodeEnd = 30;
int lolShieldBase = 40;
int lolShieldEnd = 42;
int keypadBase = 17;
int keypadEnd = 18;





//functions section
//bluetooth stuff
#ifndef BT_SETUP__
#define BT_SETUP__
const int btCodeLength = 3;
byte BTCode[btCodeLength];

void sendBTCode(char * strWord,int strLength){
  Serial1.write( "0" ); //garbage
  for (int i = 0; i < strLength; i++){
    Serial1.write( strWord[i] );
  }
}
char* getBTCode(int strLength){
  char message[strLength];
  if  (Serial1.available() ){
    //char garbage = Serial1.read();
    while(Serial1.available() ){
      for (int i = 0; i < strLength ; i ++){
        message[i] = Serial1.read();
      }
    }
  }
  return message;
}
#endif
void restartKeypad(){
  tempKeypadAnswer = "";
  keypadPassIsComplete = 0;
}
// return 1 if recieved a keypad press
int checkKeypadOutput(){
  int keyPressed = 0;
  if( (tempKeypadAnswer == keypadAnswer) && (keypadPassIsComplete == 0 ) ){
    
  }
  if (keypadPassIsComplete != 1){
    char key = keypad.getKey();
    if (key){
      keyPressed = 1;
      if (key == '*'){
        restartKeypad();
        keyPressed = 3;
      }else if ( key == "#"){
        if ( keypadAnswer == tempKeypadAnswer ){
          keyPressed = 2;
          keypadPassIsComplete = 1;
        }
        else{
          restartKeypad();
          keyPressed = 3;
        }
      }
      else{
        tempKeypadAnswer.concat(key);
      }
    }
  }
  return keypressed;
}
void resetCat5CablePuzzle(){
  for (int i = 0; i < CAT5CABLENUM; i ++){
    cat5Puzzle_tempAnswer[i] = 0;
  }
}
void checkCat5Puzzle(){
  for (int i = 0; i < CAT5CABLENUM; i ++){
    int pulseVal = pulseIn(cat5Puzzle_pins[i], HIGH);
    if( ( pulseVal  >= cat5Puzzle_answersLow[i] ) && ( pulseVal  <= cat5Puzzle_answersHigh[i] ) ){
      if ( cat5Puzzle_tempAnswer[i] == 0 ){
        cat5Puzzle_tempAnswer[i] = 1;
        
        //code base
        int codeLen = 3;
        char message[codeLen] = {'3', '1', (char)('1' + i) };
        sendBTCode(message,codeLen);
      }
    }
  }
}
/*
//delay of msDelay, 4 times
void blink_turnOn(int glowingWirePin, int msDelay){
  for(int i = 0; i < 4; i++){
    digitalWrite(glowingWirePin,HIGH);
    delay(msDelay);
    digitalWrite(glowingWirePin,LOW);
    delay(msDelay);
  }
  digitalWrite(glowingWirePin,HIGH);
}
void blink_turnOff(int glowingWirePin,int msDelay){
  for(int i = 0; i < 4; i++){
    digitalWrite(glowingWirePin,HIGH);
    delay(msDelay);
    digitalWrite(glowingWirePin,LOW);
    delay(msDelay);
  }
  digitalWrite(glowingWirePin,LOW);
}
*/




void setup() {
  Serial.begin(9600);   //testing
  restartKeypad();    //init keypad
  initLoLShield();    //init lol shield
  
  //init bluetooth
  #ifndef SERIAL1_SETUP__
  #define SERIAL1_SETUP__
  Serial1.begin(9600);    //bluetooth setup
  #endif

//glowing wire testing
 /*
  pinMode(glOrange , OUTPUT);
  pinMode(glBlue , OUTPUT);
  pinMode(glPink , OUTPUT);
  pinMode(glRed , OUTPUT);
  pinMode(glGreen , OUTPUT);
*/

  //init led ring

  //init glowing wire
  glowingWireManager.manageGlowingWire::wireSetup();
  
}

void loop() {
  //check bt for command, if so then handle the command
  //"turn on glow wire" command
  //"do specific led ring light show" command
  //"change lol lights" command


  //change/increment the pattern on display
  if (lolShieldPatternIndex == lolShieldPatternLength ){
    lolShieldPatternIndex = 0;
  }
  ClearShield();
  turnOnSentence( lolShieldPatterns[lolShieldPatternIndex] );
  lolShieldPatternIndex++;

  delay(3000);
        
  //  glowing wire testing
  /*
  digitalWrite(glOrange  , HIGH);
  digitalWrite(glBlue  , HIGH);
  digitalWrite(glPink  , HIGH);
  digitalWrite(glRed  , HIGH);
  digitalWrite(glGreen  , HIGH);
  delay(1000);
  digitalWrite(glOrange  , LOW);
  digitalWrite(glBlue  , LOW);
  digitalWrite(glPink  , LOW);
  digitalWrite(glRed  , LOW);
  digitalWrite(glGreen  , LOW);
  delay(1000);
  */
  //cat5 cable reading
  /*
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
  delay(1000);
  */

  /*
    if  (Serial1.available() ){
      int CodeCount = 0;
      while(Serial1.available() ){
        BTCode[CodeCount] = Serial1.read();
        CodeCount ++;
      }
      if (CodeCount >= btCodeLength  ){
        //invalid Bluetooth code
      }
      else{
        int techNum = BTCode[0] -'0';
        int codeNum = ( (BTCode[1] - '0') * 10 ) + (BTCode[2]- '0');

        if (techNum == 2 ){
          if (codeNum >= glowingWireCodeBase ) {
            if( codeNum < glowingWireCodeEnd + 1) {
              //turn on associated glowing wire
              int codeFunction = codeNum - glowingWireCodeBase;
              if (codeFunction == 0){
                //turn off all glowing wire
                glowingWireManager.resetWire();
              }else if (codeFunction <= 5){
                //turn on glowing wire
                manageGlowingWire::GeneralFunction wireAction = glowingWireManager.manageGlowingWire::lightWireAction[codeFunction-1];
                (glowingWireManager.*wireAction)();
              }else if (codeFunction == 6){
                glowingWireManager.turnOff0();
              }
              else{
                //still under construction
              }
            }
            else if (codeNum >= lolShieldBase ){
              if (codeNum <= lolShieldEnd ){
                //do something with lol shield
                int codeFunction = codeNum - lolShieldEnd;
                if (codeFunction == 1){
                  //reset/clear lol shield
                  displayingShield.ClearShield();
                }
                else if (codeFunction == 2){
                  //change/increment the pattern on display
                  if (lolShieldPatternIndex == lolShieldPatternLength ){
                    lolShieldPatternIndex = 0;
                  }
                  displayingShield.ClearShield();
                  displayingShield.turnOnSentence(lolShieldPatterns[lolShieldPatternIndex] );
                  lolShieldPatternIndex++;
                }
                else{
                //not enough room for another function
                }
              }
            }
            else{
              //code not implemented
            }
          }
          else{
            //code contains a number that is too low
          }
        }
        else if (techNum == 3){
          if (codeNum == 17  ){
            //reset keypad puzzle
            restartKeypad();
          }
          else if (codeNum == 19){
            resetCat5CablePuzzle();
          }
          else{
            //not a valid code
          }
        }
        else {
          //not a valid code
        }
      }
   }
   
   
  //check if key on keypad are pressed > if entered password is correct then don't check again
  int keypadResult = checkKeypadOutput();
  if( (keypadResult == 2 ) && (keypadPassIsComplete ) ){
    sendBTCode((char *)"318", 3);
    glowingWireManager.lightWire1();
  }else if (keypadResult == 1){
    glowingWireManager.blinkWire(1, 100);
  }else if (keypadResult == 3){
    glowingWireManager.blinkWire(1, 100);
    glowingWireManager.blinkWire(1, 100);
    glowingWireManager.blinkWire(1, 100);
  }else{}
  
  //check all cat5e pins to see if they are all on > if all on then don't check
  checkCat5Puzzle();
  */
}
