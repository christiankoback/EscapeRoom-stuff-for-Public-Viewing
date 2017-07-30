/*
      LEDstrip_keyDrop.ino

Purpose: provides code for Arduino for LED strip control and key Drop

*/

#include <Servo.h>

int ledStripChange = 0;
int ledRedPin = 6;
int ledBluePin = 7;
int ledGreenPin = 8;
int ledBrightness = 5;

int keyDropGoalSensor = 2;
int keyDropLowRange = 1640; //approx 2 V        // 819.2 per volt
int keyDropHighRange = 4096;  //approx 5V
int keyDropServoPin = 9;

Servo keyDropServo;

void setup() {
  // put your setup code here, to run once:
  //setup led strip to default (all lights on)
  //setup key drop code
  keyDropServo.attach(keyDropServoPin);
  keyDropServo.write( 0 );
}

void loop() {
  // put your main code here, to run repeatedly:

  // change LED strip on programmable trigger ( ? )
  //    if so, do that here
  if (ledStripChange == 1){
    // if 1, change led strip to purely green
    //not done yet

    ledStripChange = 0;
  }
  else if (ledStripChange == 2){
    //if 2, change to purely red
    //not done yet

    ledStripChange = 0;
  }
  else if (ledStripChange == 3){
    // if 3, change to purely blue
    //not done yet

    ledStripChange = 0;
  }
  else()
  
  // check sensor for key drop goal
  //    if goal is reached, rotate the servo motor to drop key
  int keyDropSensorValue = analogRead(keyDropGoalSensor) ;
  if ( (keyDropSensorValue > keyDropLowRange) && ( keyDropSensorValue < keyDropHighRange) ){
      //if goal is reached, then rotate servo and drop the key
      for (pos = 0; pos >= 90; pos += 1){
        keyDropServo.write( pos );
        delay(15);
      }
  }
  
}
