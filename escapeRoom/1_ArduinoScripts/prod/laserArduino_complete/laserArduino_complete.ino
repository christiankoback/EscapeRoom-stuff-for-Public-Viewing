
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
const int LASER_ON_BEFORE_LASER_LOCATION_CHANGE = 5;  //number of laser on/off cycles toggles before laser location is changed


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

//FILL       THESE   IN       BEFORE      USE
int panLocations_1[PANTILTSERVOLOCATIONS] = {130  ,  170 ,100   };
int tiltLocations_1[PANTILTSERVOLOCATIONS] = {30  , 50 , 25 };

int panLocations_2[PANTILTSERVOLOCATIONS] = {13 , 60 , 35 };
int tiltLocations_2[PANTILTSERVOLOCATIONS] = {30 , 40 , 25 };

int panLocations_3[PANTILTSERVOLOCATIONS] = {80  , 70  , 100  };
int tiltLocations_3[PANTILTSERVOLOCATIONS] = {45  , 25 , 37 };


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
void updateTimeLaser(){
  time_t laserTime = now();
  laserMinutes = minute(laserTime);
  laserSeconds = second(laserTime);
  toggleLaserAmount++;
}
void toggleEverythingOn(){
  digitalWrite(shapeLaser,HIGH);
  if ( shapeSensorOfInterest == 0){
    digitalWrite(cubeShapeSensor, HIGH);
  }else if ( shapeSensorOfInterest == 1){
    digitalWrite(triangleShapeSensor, HIGH);
  }
  else if ( shapeSensorOfInterest == 2){
    digitalWrite(tunnelShapeSensor, HIGH);
  }
  else{}
}
void toggleEverythingOff(){
  digitalWrite(shapeLaser,LOW);
  if ( shapeSensorOfInterest == 0){
    digitalWrite(cubeShapeSensor, LOW);
  }else if ( shapeSensorOfInterest == 1){
    digitalWrite(triangleShapeSensor, LOW);
  }
  else if ( shapeSensorOfInterest == 2){
    digitalWrite(tunnelShapeSensor, LOW);
  }
  else{}
}

void toggleLight(int light, int mainSeconds, int mainMinutes, int laserSeconds, int laserMinutes){
  int currState = digitalRead(light);
  
  //if state is on, wait 3 seconds and turn off
  int elapsedSeconds =  mainSeconds - laserSeconds;
  int elapsedMinutes = mainMinutes - laserMinutes;
  
  if (currState == HIGH){
    if (elapsedMinutes >= 1){
      toggleEverythingOff();
      updateTimeLaser();
    }
    else if (elapsedSeconds >= 3){
      toggleEverythingOff();
      updateTimeLaser();
    }
    else{}
  }
  else if (currState == LOW){
    if (elapsedMinutes >= 1){
      toggleEverythingOn();
      updateTimeLaser();
    }
    else if (elapsedSeconds >= 1){
      toggleEverythingOn();
      updateTimeLaser();
    }
    else{}
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

const int TRIP_LASER_ADC_CHANNEL_AMOUNT = 3;
int tripLaserMin[TRIP_LASER_ADC_CHANNEL_AMOUNT];
int tripLaserMax[TRIP_LASER_ADC_CHANNEL_AMOUNT];
int tripLaserPercentError = 20;

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
int checkTripLasers = 0;
int isReset = 0;
// send code to raspberry pi when a sensor is tripped  --- 400 
//send code to raspberry pi when game is won ---- 402
// when game is won, turn off shapes & continue reading trip sensors



void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  ads1015_1.begin();
  ads1015_4.begin();
  setupServo();
  setupShapeLaser_andSensors();
  t = now();
  t_minutes = minute(t);
  t_seconds = second(t);

  for (int i = firstPin_triggerLaser; i < (firstPin_triggerLaser + AMOUNT_OF_TRIGGER_LASERS); i ++){
     pinMode(i, OUTPUT);
  }
}

void loop() {
  if (Serial1.available() > 0){
    inData = Serial1.readStringUntil('\n');
    //Serial.println("inData>" + inData + "<");
    
    while (inData.length() > 2) {
      token = inData.substring(0,3);
      inData = inData.substring(3,inData.length());
      Serial.println("token>" + token + "<");
      Serial.println("inData>" + inData + "<");
      Serial.println("");
  
      if (token == "000") {                          // Reply to alive request DO NOT DELETE
        //Serial.println("");
        Serial1.println("!");    
     
      } 
      if (token == "401") {                       // start game
        isGameIncomplete = 0;
        isLasersOn = 0;
        checkTripLasers = 1;
        shapeSensorOfInterest = 0;
      }else if (token == "999"){                  //reset game
        isGameIncomplete = 1;
        isLasersOn = 0;
        checkTripLasers = 0;
        isReset = 1;
      }else {
        Serial1.println("!");  
      }
      
    } //end while inData
  } // end if serial available
  if (isGameIncomplete == 0){         //start game
    if (isLasersOn == 0){
      Serial.print(" laser is on");
        isLasersOn = 1;
        digitalWrite(shapeLaser,HIGH);
        digitalWrite(cubeShapeSensor,HIGH);
        panServo.write(panLocations_1[panTiltIndex] );
        tiltServo.write(tiltLocations_1[panTiltIndex]);

        for (int i = firstPin_triggerLaser; i < (firstPin_triggerLaser + AMOUNT_OF_TRIGGER_LASERS); i ++){
           digitalWrite(i, HIGH);
        }
    }
       
    //get current time -- used to figure out non-blocking blinking of stuff
    time_t currentTime = now();
    int currentTime_seconds = second(currentTime);
    int currentTime_minutes = minute(currentTime);

    Serial.println("before blinking laser");
    
    //check if a second has passed, if passed then blink laser
    toggleLight(shapeLaser, currentTime_seconds, currentTime_minutes, laserSeconds, laserMinutes);

    Serial.println("before changing servo laser location");

    //change location of the servo laser
    if( (toggleLaserAmount >= LASER_ON_BEFORE_LASER_LOCATION_CHANGE) && (digitalRead(shapeLaser) == LOW) ){
      toggleLaserAmount = 0;
      panTiltIndex++;
      if (panTiltIndex >= PANTILTSERVOLOCATIONS){
        panTiltIndex = 0;
      }
      //check to see what shape sensor is of interest, change location for specific laser-sensor match
      if (shapeSensorOfInterest == 0){
        panServo.write(panLocations_1[panTiltIndex] );
        tiltServo.write(tiltLocations_1[panTiltIndex]);
      }
      else if(shapeSensorOfInterest == 1){
        panServo.write(panLocations_2[panTiltIndex]);
        tiltServo.write(tiltLocations_2[panTiltIndex]);
      }
      else if(shapeSensorOfInterest == 2){
        panServo.write(panLocations_3[panTiltIndex]);
        tiltServo.write(tiltLocations_3[panTiltIndex]);
      }
      else{}
    }
    Serial.println(" ");
    Serial.print("sensor of interest: ");
    Serial.println(shapeSensorOfInterest);
    Serial.println( " ");
    
    //check all sensors
    // check shape laser sensors if active
    if (shapeSensorOfInterest == 0){
      shapeSensorValue = ads1015_1.readADC_SingleEnded(0);
      Serial.println("after checking cube shape sensor");
      //check if valid sensor value, if valid then change sensor of interest to next sensor
      if ( (shapeSensorValue > cubeMin) && (shapeSensorValue < shapeMax) ) {
         digitalWrite(cubeShapeSensor,LOW);
         digitalWrite(triangleShapeSensor,HIGH);
         Serial.println(" ");
        Serial.println(" ");
        Serial.println("THIS IS NOT TO BE ON " );
        Serial.println(" ");
        Serial.println(" ");
        
         shapeSensorOfInterest = 1;
      }
    }
    else if (shapeSensorOfInterest == 1){
      shapeSensorValue = ads1015_1.readADC_SingleEnded(1);
  
      //check if valid sensor value, if valid then change sensor of interest to next sensor
      if ( (shapeSensorValue > triangleMin) && (shapeSensorValue < shapeMax) ){
        digitalWrite(triangleShapeSensor,LOW); 
        digitalWrite(tunnelShapeSensor,HIGH);
        shapeSensorOfInterest = 2;
      }
    }
    else if (shapeSensorOfInterest == 2){
      shapeSensorValue = ads1015_1.readADC_SingleEnded(2);
  
      //check if valid sensor value, if valid then change sensor of interest to next sensor
      if ( (shapeSensorValue > tunnelMin) && (shapeSensorValue < shapeMax) ){
        //puzzle is COMPLETE
        Serial1.println("402");
        digitalWrite(tunnelShapeSensor,LOW);
      }
    }
    else{}
  }
  else{}                              // do nothing
  Serial.println("at end of game play");

  if (isReset == 1  ){
    //turn on lasers
    if (digitalRead(firstPin_triggerLaser) == LOW){
      //turn off all trigger lasers -- pin 30-46
      for (int i = firstPin_triggerLaser; i < (firstPin_triggerLaser + AMOUNT_OF_TRIGGER_LASERS); i ++){
         digitalWrite(i, HIGH);
      }
    }
    // check trip lasers and log lowest & highest values
    time_t currentTempTime = now();
    int tempMinutes = minute(currentTempTime);
    time_t loopTime = now();
    int tripLaserMinTemp[TRIP_LASER_ADC_CHANNEL_AMOUNT] ={4096, 4096, 4096};
    int tripLaserMaxTemp[TRIP_LASER_ADC_CHANNEL_AMOUNT] ={0, 0, 0};
    if (tempMinutes == 59){   //ensure while loop is 2 minute long, boundry cases -- 59 & 60 minutes of an hour
      while( minute(loopTime) != 1){
        for (int i = 0; i < TRIP_LASER_ADC_CHANNEL_AMOUNT; i ++){
          //get sensor value
          int16_t tempSensor = ads1015_4.readADC_SingleEnded(i);
          //check if sensor value is less than current lowest value, if is then update
          if (tempSensor < tripLaserMinTemp[i]){
            tripLaserMinTemp[i] = tempSensor;
          }
          //check if sensor value is greater than current highest value, if is then update
          else if (tempSensor > tripLaserMinTemp[i]){
            tripLaserMaxTemp[i] = tempSensor;
          }
          else{}
        }
        loopTime = now();
      }
    }else if (tempMinutes == 60){
      while( minute(loopTime) != 2){
        for (int i = 0; i < TRIP_LASER_ADC_CHANNEL_AMOUNT; i ++){
          //get sensor value
          int16_t tempSensor = ads1015_4.readADC_SingleEnded(i);
          //check if sensor value is less than current lowest value, if is then update
          if (tempSensor < tripLaserMinTemp[i]){
            tripLaserMinTemp[i] = tempSensor;
          }
          //check if sensor value is greater than current highest value, if is then update
          else if (tempSensor > tripLaserMinTemp[i]){
            tripLaserMaxTemp[i] = tempSensor;
          }
          else{}
        }
        loopTime = now();
      }
    }else{
      while( ( minute(loopTime) - tempMinutes )  <= 2   ){
        for (int i = 0; i < TRIP_LASER_ADC_CHANNEL_AMOUNT; i ++){
          //get sensor value
          int16_t tempSensor = ads1015_4.readADC_SingleEnded(i);
          //check if sensor value is less than current lowest value, if is then update
          if (tempSensor < tripLaserMinTemp[i]){
            tripLaserMinTemp[i] = tempSensor;
          }
          //check if sensor value is greater than current highest value, if is then update
          else if (tempSensor > tripLaserMinTemp[i]){
            tripLaserMaxTemp[i] = tempSensor;
          }
          else{}
        }
      }
      loopTime = now();
    }
    
    //set values to global variables after 2 seconds
    for (int i = 0; i < TRIP_LASER_ADC_CHANNEL_AMOUNT; i ++){
      tripLaserMin[i] = tripLaserMinTemp[i] - (int)(tripLaserMinTemp[i]*tripLaserPercentError/100 );
      tripLaserMax[i] = tripLaserMaxTemp[i] + (int)(tripLaserMaxTemp[i]*tripLaserPercentError/100 );
    }
    
    //turn off lasers 
    //turn off all shapes
    digitalWrite(shapeLaser, LOW);
    digitalWrite(cubeShapeSensor   , LOW);
    digitalWrite(triangleShapeSensor   , LOW);
    digitalWrite(tunnelShapeSensor   , LOW);
  
   //turn off all trigger lasers -- pin 30-46
    for (int i = firstPin_triggerLaser; i < (firstPin_triggerLaser + AMOUNT_OF_TRIGGER_LASERS); i ++){
       digitalWrite(i, LOW);
    }
    isReset = 0;
  }
  
  if ( checkTripLasers == 1){
    Serial.println("checking trip sensors");
    // check all other laser sensors, if any sensors are NOT active then restart the box puzzle
    for (int i = 0; i < TRIP_LASER_ADC_CHANNEL_AMOUNT; i ++){
      int16_t tempSensor = ads1015_4.readADC_SingleEnded(i);
      if (  (tempSensor < tripLaserMin[i]) || (tempSensor > tripLaserMax[i])  ){
        Serial1.println("400");
        shapeSensorOfInterest = 0;
        digitalWrite(triangleShapeSensor   , LOW);
        digitalWrite(tunnelShapeSensor   , LOW);
      }
    }
    Serial.println("done checking all trip sensors");
  }
  Serial.println("done loop");
}
