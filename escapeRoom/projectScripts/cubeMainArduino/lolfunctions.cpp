

#include "lolfunctions.h"

String charPart0= "mskaigf";
String charPart1 = "qxdo?nh";
String charPart2 = "et1brvw";
String charPart3 = "upjycz4";
String charPart4 = "2519638";
int LoLBrightness = 7;


void setupLoLShield(){
  LedSign::Init(GRAYSCALE);
}
void ClearShield(){
  LedSign::Clear();
}
void turnOnLetter(char letter){
  
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
    LedSign::Set(led0 , column1 + i, LoLBrightness);
    LedSign::Set(led1 , column1 + i, LoLBrightness);
  }
}
void turnOnSentence(String sentence){
  for(int i = 0; i < sentence.length(); i++){
    turnOnLetter(sentence[i]);
  }
}
