#include <btSerial.h>
#include <LEDStrip.h>
#include <Keypad.h>

#include "lolfunctions.h"
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
byte rowPins[ROWS] = {22, 23, 24, 25}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {26, 27, 28, 29}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
String keypadAnswer = "6175a4d";
String tempKeypadAnswer = "";
int keypadPassIsComplete = 0;

//pins for glowing wire
int glOrange = 32;
int glBlue = 33;
int glPink = 34;
int glRed = 35;
int glGreen = 36;

//pins for cat5
int cat5_1_IN = 37;
int cat5_2_IN = 38;
int cat5_3_IN = 39;
int cat5_4_IN = 40;
int cat5_5_IN = 41;

const int CAT5CABLENUM = 5; 
int cat5Puzzle_pins[CAT5CABLENUM] = {37, 38, 39, 40, 41};
int cat5Puzzle_answers[CAT5CABLENUM] = {230, 70 , 4 , 1025 , 405};
int cat5Puzzle_tempAnswer[CAT5CABLENUM] = {0, 0 , 0, 0, 0};

//random values -- corresponds with PWM pulses 
int cable1_value = 230;
int cable2_value = 70;
int cable3_value = 4;
int cable4_value = 1025;
int cable5_value = 405;

//bluetooth stuff
#ifndef BT_SETUP__
#define BT_SETUP__
const int btCodeLength = 3;
char BTCode[btCodeLength];
#endif

//code vardiables
int glowingWireCodeBase = 21;
int glowingWireCodeEnd = 30;
int headLightBase= 40;
int headLightEnd = 42;
int lolShieldEnd = 44;
int keypadBase = 17;
int keypadEnd = 18;

//init glowing wire manager class
manageGlowingWire glowingWireManager;

void restartKeypad(){
  tempKeypadAnswer = "";
  keypadPassIsComplete = 0;
}
void checkKeypadOutput(){
  if( (tempKeypadAnswer == keypadAnswer) && (keypadPassIsComplete == 0 ) ){
    keypadPassIsComplete = 1;
    Serial1.write('3');
    Serial1.write('1');
    Serial1.write('8');
  }
  if (keypadPassIsComplete != 1){
    char key = keypad.getKey();
    if (key){
      if ( key == keypadAnswer[tempKeypadAnswer.length()] ){
        tempKeypadAnswer.concat(key);
      }
      else{
        restartKeypad();
      }
    }
  }
}

void resetCat5CablePuzzle(){
  for (int i = 0; i < CAT5CABLENUM; i ++){
    cat5Puzzle_tempAnswer[i] = 0;
  }
}
void checkCat5Puzzle(){
  for (int i = 0; i < CAT5CABLENUM; i ++){
    if ( pulseIn(cat5Puzzle_pins[i], HIGH)  == cat5Puzzle_answers[i] ){
      if ( cat5Puzzle_tempAnswer[i] == 0 ){
        cat5Puzzle_tempAnswer[i] = 1;
        
        //code base
        Serial1.write('3');
        Serial1.write('1');
        Serial1.write('1' + i);
      }
    }
  }
}


void setup() {
  Serial.begin(9600);   //testing
  restartKeypad();    //init keypad
 // setupLoLShield();   //init lol  shield
  
  
  //init bluetooth
  #ifndef SERIAL1_SETUP__
  #define SERIAL1_SETUP__
  Serial1.begin(9600);    //bluetooth setup
  #endif
  
  //init led ring

  //init glowing wire
  glowingWireManager.manageGlowingWire::wireSetup();
}

void loop() {
  //check bt for command, if so then handle the command
        //"turn on glow wire" command
        //"do specific led ring light show" command
        //"change lol lights" command
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
              if (codeFunction < 5){
                manageGlowingWire::GeneralFunction wireAction = glowingWireManager.manageGlowingWire::lightWireAction[codeFunction];
                (glowingWireManager.*wireAction)();
               // turnOnGlowingWire(codeNum - glowingWireCodeBase);  
              }
              else{
                //still under construction
              }
            }
            else if (codeNum > headLightBase ){
              if (codeNum < headLightEnd + 1){
                //do something with headlight
                int codeFunction = codeNum - glowingWireCodeEnd;
                if (codeFunction == 1){
                  //turn on headLight
                }
                else if (codeFunction == 2){
                  //turn off headlight
                }
                else{/*not enough room for another function*/}
              }
              else if (codeNum < lolShieldEnd + 1){
                //do something with lol shield
                int codeFunction = codeNum - headLightEnd;
                if (codeFunction == 1){
                  //reset/clear lol shield
                }
                else if (codeFunction == 2){
                  //change/increment the pattern on display
                }
                else{/*not enough room for another function*/}
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
  checkKeypadOutput();
  
  //check all cat5e pins to see if they are all on > if all on then don't check
  checkCat5Puzzle();
  
}