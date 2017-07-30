/*
 *    *** escapeRoom1.ino ***
 * 
 * Purpose : setup for arduinos in escape room 1, season 1
 *          - could be basis for all escape rooms
 *          
 
 */
#include "escapeRoom1.h"

int numOfPairedLaserSensor = 15;
int numOfLasers = numOfPairedLaserSensor;
int laserPinStart = 22;

int sensorPinStart = 0;
int numOfSensors = numOfPairedLaserSensor;
int sensorLowRange = 820;
int sensorHighRange = 900;     //820 per volt


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //debugging
  //turn on all lasers
  int maxPin = laserPinStart + numOfLasers;
  for (int i = laserPinStart; i < maxPin; i++){
    pinMode(i , OUTPUT);
    digitalWrite(i , HIGH);
  }

  
}

void loop() {
  // put your main code here, to run repeatedly:
  int maxSensorPin = sensorPinStart + numOfSensors;
  for (int i = sensorPinStart; i < maxSensrPin; i ++){
    int sensorValue = analogRead( i );
    if( (sensorValue > sensorLowRange) && (sensorValue <  sensorHighRange ) ){
      //sensor value is valid
      Serial.print("sensor ");
      Serial.print(i);
      Serial.println(" is on and valid." );
      
    }
  } //end of for loop
  
}
