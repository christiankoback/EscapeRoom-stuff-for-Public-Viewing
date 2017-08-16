#include "glowingWire.h"



int wire1Pin = 1;
int wire2Pin = 2;
int wire3Pin = 3;
int wire4Pin = 4;
int wire5Pin = 5;


void manageGlowingWire::lightWire0 ( )
{
  digitalWrite(wire1Pin, HIGH);
}
void manageGlowingWire::lightWire1 ( )
{
  digitalWrite(wire2Pin, HIGH);
}
void manageGlowingWire::lightWire2 ( )
{
  digitalWrite(wire3Pin, HIGH);
}
void manageGlowingWire::lightWire3 ( )
{
  digitalWrite(wire4Pin, HIGH);
}
void manageGlowingWire::lightWire4 ( )
{
  digitalWrite(wire5Pin, HIGH);
}
void manageGlowingWire::turnOff0 ( )
{
  digitalWrite(wire1Pin, LOW);
}
void manageGlowingWire::turnOff1 ( )
{
  digitalWrite(wire2Pin, LOW);
}
void manageGlowingWire::turnOff2 ( )
{
  digitalWrite(wire3Pin, LOW);
}
void manageGlowingWire::turnOff3 ( )
{
  digitalWrite(wire4Pin, LOW);
}void manageGlowingWire::turnOff4 ( )
{
  digitalWrite(wire5Pin, LOW);
}
void manageGlowingWire::resetWire(){
	turnOff0();
	turnOff1();
	turnOff2();
	turnOff3();
	turnOff4();
}
void manageGlowingWire::wireSetup(){
  pinMode(wire1Pin, OUTPUT);
  pinMode(wire2Pin, OUTPUT);
  pinMode(wire3Pin, OUTPUT);
  pinMode(wire4Pin, OUTPUT);
  pinMode(wire5Pin, OUTPUT);
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
  
