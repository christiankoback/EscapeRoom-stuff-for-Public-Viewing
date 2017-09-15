#include "glowingWire.h"


//pins for glowing wire
int glOrange = 32;
int glBlue = 33;
int glRed = 34;
int glPink = 35;
int glGreen = 36;

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
void blinkGlowingWire(int glowingWirePin,int msDelay){
    digitalWrite(glowingWirePin,HIGH);
    delay(msDelay);
    digitalWrite(glowingWirePin,LOW);
    delay(msDelay);
}

void manageGlowingWire::blinkWire(int wire, int msDelay){
  if (wire == 0){
    blinkGlowingWire(glOrange, 100);
  }else if (wire ==1){
    blinkGlowingWire(glBlue, 100);
  }
  else if (wire == 2){
    blinkGlowingWire(glGreen, 100);
  }else if (wire == 3){
    blinkGlowingWire(glRed, 100);
  }else if (wire ==4){
    blinkGlowingWire(glPink, 100);
  }else{}
}
void manageGlowingWire::lightWire0 ( )
{
  blink_turnOn(glOrange, 100);
}
void manageGlowingWire::lightWire1 ( )
{
  blink_turnOn(glBlue, 100);
}
void manageGlowingWire::lightWire2 ( )
{
  blink_turnOn(glGreen, 100);
}
void manageGlowingWire::lightWire3 ( )
{
  blink_turnOn(glRed, 100);
}
void manageGlowingWire::lightWire4 ( )
{
  blink_turnOn(glPink, 100);
}
void manageGlowingWire::turnOff0 ( )
{
  blink_turnOff(glOrange,100);
}
void manageGlowingWire::turnOff1 ( )
{
  blink_turnOff(glBlue,100);
}
void manageGlowingWire::turnOff2 ( )
{
  blink_turnOff(glGreen,100);
}
void manageGlowingWire::turnOff3 ( )
{
  blink_turnOff(glRed, 100);
}
void manageGlowingWire::turnOff4 ( )
{
  blink_turnOff(glPink, 100);
}
void manageGlowingWire::resetWire(){
	turnOff0();
	turnOff1();
	turnOff2();
	turnOff3();
	turnOff4();
}
void manageGlowingWire::wireSetup(){
  pinMode(glOrange, OUTPUT);
  pinMode(glBlue, OUTPUT);
  pinMode(glGreen, OUTPUT);
  pinMode(glRed, OUTPUT);
  pinMode(glPink, OUTPUT);
}
 // array of function pointers
const manageGlowingWire::GeneralFunction manageGlowingWire::lightWireAction[5] =
  {
    &manageGlowingWire::lightWire0, 
    &manageGlowingWire::lightWire1, 
    &manageGlowingWire::lightWire2, 
    &manageGlowingWire::lightWire3,
	&manageGlowingWire::lightWire4
  };
   // array of function pointers
const manageGlowingWire::GeneralFunction manageGlowingWire::turnOffAction[5] =
  {
    &manageGlowingWire::turnOff0, 
    &manageGlowingWire::turnOff1, 
    &manageGlowingWire::turnOff2, 
    &manageGlowingWire::turnOff3,
	&manageGlowingWire::turnOff4
  };
  
