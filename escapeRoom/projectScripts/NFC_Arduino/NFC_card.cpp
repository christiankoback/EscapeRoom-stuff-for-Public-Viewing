#include "NFC_card.h"

//testing info
int pinLed = 22;
int pinLed2 = 23;
char cardTech = '1';
int codeLength = 2;


void manageCards::doCard0 ( )
{
  //digitalWrite(pinLed, HIGH);
  //Serial.println("tag 1 was tapped");
  Serial1.write( cardTech );
  char message[codeLength] = "01";
  for (int i = 0; i < codeLength; i++){
    Serial1.write( message[i] );
  }
}
void manageCards::doCard1 ( )
{
  //digitalWrite(pinLed2, HIGH);
  //Serial.println("tag 2 was tapped");
  Serial1.write( cardTech );
  char message[codeLength] = "02";
  for (int i = 0; i < codeLength; i++){
    Serial1.write( message[i] );
  }
}
void manageCards::doCard2 ( )
{
  //digitalWrite(pinLed, HIGH);
  //digitalWrite(pinLed2, HIGH);
  //Serial.println("Mifare Classic 1 card was tapped");
  Serial1.write( cardTech );
  char message[codeLength] = "03";
  for (int i = 0; i < codeLength; i++){
    Serial1.write( message[i] );
  }
}
void manageCards::doCard3 ( )
{
  //digitalWrite(pinLed, HIGH);
  //digitalWrite(pinLed2, HIGH);
  //Serial.println("Mifare Classic 2 card was tapped");
  Serial1.write( cardTech );
  char message[codeLength] = "04";
  for (int i = 0; i < codeLength; i++){
    Serial1.write( message[i] );
  }
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
const manageCards::GeneralFunction manageCards::doCardAction[4] =
  {
    &manageCards::doCard0, 
    &manageCards::doCard1, 
    &manageCards::doCard2, 
    &manageCards::doCard3
  };
