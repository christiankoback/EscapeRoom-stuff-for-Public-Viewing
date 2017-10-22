
/*
  laserArduino.ino   ---- Escape Room 1 Season 1
Purpose:  - manage behaviour of all lasers 
          - manage all sensors via ADC components
          - manage behaviour of all LEDs for sensors
          -manage motors for lasers 


 There will be 3 sensors on the ceiling, which will have to be enabled by mirrors from one laser on pan tilt.
 The room will be filled with lasers, if any are tripped, restart the mirroring puzzle.
 If all 3 boxes are enabled, send code to RPi.

          
*/
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>     //adafruit adcs
#include <Time.h>           // include time library
#include <TimeLib.h>



//shape laser & sensor LEDs
int shapeLaser = 22;
int cubeShapeSensor = 23;
int triangleShapeSensor = 24;
int tunnelShapeSensor = 25;

//keep track of which sensor we care about & its value
int shapeSensorOfInterest = 0;
int16_t shapeSensorValue = 0;

//valid values for shape sensors
int cubeMin = 10;
int triangleMin = 500;
int tunnelMin = 790;
int shapeMax = 1000;

//keeping track between toggling servo laser & sensor LEDs
int laserMinutes = 0;
int laserSeconds = 0;

int toggleLaserAmount = 0;        //program value, number the laser has turned ON
const int LASER_ON_BEFORE_LASER_LOCATION_CHANGE = 2;  //number of laser toggles before location is switched


/* pan-Tilt servo stuff 
    blue = pan   = pin 12
    yellow = tilt = pin 13
 */
int tiltPin = 13;
int panPin = 12;

Servo panServo;
Servo tiltServo;

/*locations/angles for servo laser per shape sensor*/
const int PANTILTSERVOLOCATIONS = 3;
int panTiltIndex = 0;
int panLocations_1[PANTILTSERVOLOCATIONS] = {13 , 30 , 35 };
int tiltLocations_1[PANTILTSERVOLOCATIONS] = {30 , 45 , 25 };



//FILL       THESE   IN       BEFORE      USE
int panLocations_2[PANTILTSERVOLOCATIONS] = {  ,   ,   };
int tiltLocations_2[PANTILTSERVOLOCATIONS] = {  ,  ,  };

int panLocations_3[PANTILTSERVOLOCATIONS] = {  ,   ,   };
int tiltLocations_3[PANTILTSERVOLOCATIONS] = {  ,  ,  };


void setupServo(){
  panServo.attach(panPin);
  tiltServo.attach(tiltPin);
}
void setupShapeLaser_andSensors(){
  pinMode(shapeLaser,OUTPUT);
  pinMode(cubeShapeSensor,OUTPUT);
  pinMode(triangleShapeSensor,OUTPUT);
  pinMode(tunnelShapeSensor,OUTPUT);
}

//manage the toggling of laser & sensor LEDs
function updateTimeLaser(){
  time_t laserTime = now();
  laserMinutes = minute(laserTime);
  laserSeconds = second(laserTime);
}
function toogleLaser(int mainSeconds, int mainMinutes, int laserSeconds, int laserMinutes){
  int currState = digitalRead(shapeLaser);

  //if state is on, wait 2 seconds and turn off
  if (currState == HIGH){
    int elapsedSeconds = mainSeconds - laserSeconds;
    if( elapsedSeconds >= 2 ){
      digitalWrite(shapeLaser, LOW);
      updateTimeLaser();
    }else if (elapsedSeconds == 0){
      int elapsedMinutes = mainMinutes - laserMinutes;
      if (elapsedMinutes >= 1){
        digitalWrite(shapeLaser,LOW);
        updateTimeLaser();
      }
    }
  }
  else if (currState == LOW){
    int elapsedSeconds = mainSeconds - laserSeconds;
    if (elapsedSeconds >= 1 ){
      digitalWrite(shapeLaser, HIGH);
      updateTimeLaser();
    }
    else if (elapsedSeconds == 0){
      int elapsedMinutes = mainMinutes - laserMinutes;
      if (elapsedMinutes >= 1){
        digitalWrite(shapeLaser,HIGH);
        updateTimeLaser();
      }
    }
  }
  else{}
}



// shape laser sensors, THESE are actually taken from Adafruit ADC object (sensor = 0-2, adc 1)
// normal laser sensors   (sensor = 0-3 on adc 2,3,4  & 3 on adc1)
//   - init adafruit adc's
Adafruit_ADS1015 ads1015_1(0x48);
//Adafruit_ADS1015 ads1015_2(0x49);     //not needed
//Adafruit_ADS1015 ads1015_3(0x4A);     //not needed
Adafruit_ADS1015 ads1015_4(0x4B);
int tripLaserMin = 10;
int tripLaserMax = 500;
const int TRIP_LASER_ADC_CHANNEL_AMOUNT = 3;

//variable to keep track of time
time_t t;
int t_seconds;
int t_minutes;


    // Bluetooth variables/setupbles
String inData;
String token;


int firstPin_triggerLaser = 30;
const int AMOUNT_OF_TRIGGER_LASERS = 17;


int isGameIncomplete = 2;
int isLasersOn = 0;

// send code to raspberry pi when a sensor is tripped  --- 400 
//send code to raspberry pi when game is won ---- 402
// when game is won, turn off shapes & continue reading trip sensors



void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  ads1015_1.begin();
  ads1015_2.begin();
  ads1015_3.begin();
  ads1015_4.begin();
  setupServo();
  setupShapeLaser_andSensors();
  t = now();
  t_minutes = minute(t);
  t_seconds = second(t);

}

void loop() {
  if (Serial1.available() > 0){
    inData = Serial1.readStringUntil('\n');
    Serial.println("inData>" + inData + "<");
    
    while (inData.length() > 2) {
      token = inData.substring(0,3);
      inData = inData.substring(3,inData.length());
      //Serial.println("token>" + token + "<");
      //Serial.println("inData>" + inData + "<");
      //Serial.println("");
  
      if (token == "000") {                          // Reply to alive request DO NOT DELETE
        //Serial.println("");
        Serial1.println("!");    
     
      } 
      if (token == "401") {                       // start game
        digitalWrite(shapeLaser,HIGH);
        digitalWrite(cubeShapeSensor,HIGH);
        isGameIncomplete = 0;
      }else if (token == "999"){                  //reset game
        isGameIncomplete = 1;
      }else {
        Serial1.println("!");  
      }

      
      //check if a second has passed, if passed then blink laser
      if(  ( currentTime_minutes - t_minutes) == 0 ){
        if ( currentTime_seconds - t_seconds) >= 1 ){
          //toggle laser
          toogleLaser(currentTime_seconds, currentTime_minutes, laserSeconds, laserMinutes);
          toggleLaserAmount++;
      }
      else if (currentTime_minutes - t_minutes == 1){
        //toggle laser
        toogleLaser(currentTime_seconds, currentTime_minutes, laserSeconds, laserMinutes);
      }

      if (isGameIncomplete == 0){         //start game
        if (isLasersOn == 0){
            //turn on all trigger lasers -- pin 30-46
            for (int i = 0; i < AMOUNT_OF_TRIGGER_LASERS; i++){
              int actualPinForLaser = firstPin_triggerLaser + i;
              pinMode(actualPinForLaser, OUTPUT);
              digitalWrite(actualPinForLaser, HIGH);
            }
          isLasersOn = 1;
        }
       
      
        time_t currentTime = now();
        currentTime_seconds = second(currentTime);
        currentTime_minutes = minute(currentTime);

            
        //change location of the servo laser
        if (toggleLaserAmount >= LASER_ON_BEFORE_LASER_LOCATION_CHANGE){
          //check to see what shape sensor is of interest, change location for specific laser-sensor match
          if (shapeSensorOfInterest == 0){
            panServo.write (panLocations_1[panTiltIndex]);
            tiltServo.write (tiltLocations_1[panTiltIndex]);
          }
          else if(shapeSensorOfInterest == 1){
            panServo.write (panLocations_2[panTiltIndex]);
            tiltServo.write (tiltLocations_2[panTiltIndex]);
          }
          else if(shapeSensorOfInterest == 2){
            panServo.write (panLocations_3[panTiltIndex]);
            tiltServo.write (tiltLocations_3[panTiltIndex]);
          }
          else{}
          panTiltIndex ++;
          if (panTiltIndex > 2 ){
            panTiltIndex = 0;
          }
        }

          //check all sensors
          // check shape laser sensors if active
          if (shapeSensorOfInterest == 0){
            shapeSensorValue = ads1015_1.readADC_SingleEnded(0);
        
            //check if valid sensor value, if valid then change sensor of interest to next sensor
            if ( (shapeSensorValue > cubeMin) && (shapeSensorValue < shapeMax){
               digitalWrite(cubeShapeSensor,LOW);
               digitalWrite(triangleShapeSensor,HIGH);
               shapeSensorOfInterest = 1;
            }
          }
          else if (shapeSensorOfInterest == 1){
            shapeSensorValue = ads1015_1.readADC_SingleEnded(1);
        
            //check if valid sensor value, if valid then change sensor of interest to next sensor
            if ( (shapeSensorValue > triangleMin) && (shapeSensorValue < shapeMax){
              digitalWrite(triangleShapeSensor,LOW); 
              digitalWrite(tunnelShapeSensor,HIGH);
              shapeSensorOfInterest = 2;
            }
          }
          else if (shapeSensorOfInterest == 2){
            shapeSensorValue = ads1015_1.readADC_SingleEnded(2);
        
            //check if valid sensor value, if valid then change sensor of interest to next sensor
            if ( (shapeSensorValue > tunnelMin) && (shapeSensorValue < shapeMax){
              //puzzle is COMPLETE
              Serial1.println("402");
              digitalWrite(tunnelShapeSensor,LOW);
            }
          }
          else{}

         // check all other laser sensors, if any sensors are NOT active then restart the box puzzle
        for (int i = 0; i < TRIP_LASER_ADC_CHANNEL_AMOUNT; i ++){
          int16_t tempSensor = ads1015_4.readADC_SingleEnded(i);
          if (  (tempSensor < tripLaserMin) || (tempSensor > tripLaserMax)  ){
            Serial1.println("400");
            shapeSensorOfInterest = 0;
          }
        }
      }
      else if (isGameIncomplete == 1){    //reset game
        isLasersOn = 0;
        isGameIncomplete = 2;
      }
      else if (isGameIncomplete == 3){
        //read sensors and notify raspberry pi
        // check all other laser sensors, if any sensors are NOT active then restart the box puzzle
        for (int i = 0; i < TRIP_LASER_ADC_CHANNEL_AMOUNT; i ++){
          int16_t tempSensor = ads1015_4.readADC_SingleEnded(i);
          if (  (tempSensor < tripLaserMin) || (tempSensor > tripLaserMax)  ){
            Serial1.println("400");
            shapeSensorOfInterest = 0;
          }
        }
      }
      else{}                              // do nothing
      
    } //end while inData
  } // end if serial available
  
}
