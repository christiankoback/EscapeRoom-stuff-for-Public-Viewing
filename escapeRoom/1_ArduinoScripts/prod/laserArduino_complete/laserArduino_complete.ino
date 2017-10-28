
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

/* ADC objects  */
Adafruit_ADS1015 ads1015_1(0x48);
Adafruit_ADS1015 ads1015_2(0x4B);
Adafruit_ADS1015 ads1015_3(0x49); 
Adafruit_ADS1015 ads1015_4(0x4A);   
int adcChannels = 4;   //amount of channels available on ADCs 
int tripSensorAdcAmount = 3;      //amount of adc's with trip sensors on them

/*shape laser & sensor LED    pins    */
int shapeLaser = 22;
int cubeShapeSensor = 23;
int triangleShapeSensor = 24;
int tunnelShapeSensor = 25;
const int LASER_ON_BEFORE_LASER_LOCATION_CHANGE = 6;  //number of laser on/off cycles toggles before laser location is changed

/* trip laser & associated ADC variables     */
const int TRIP_LASER_AMOUNT = 13;
int firstPin_triggerLaser = 30;
const int AMOUNT_OF_TRIGGER_LASERS = 17;
int laserTripVal = 500;

/*servo motor pins & objects
    pan-Tilt servo stuff 
    blue = pan   = pin 12
    yellow = tilt = pin 13
 */
int tiltPin = 13;
int panPin = 12;
Servo panServo;
Servo tiltServo;

/* locations/angles for servo laser per shape sensor */
const int PANTILTSERVOLOCATIONS = 3;
int panTiltIndex = 0;
int panLocations_1[PANTILTSERVOLOCATIONS] = {130  ,  170 ,100   };
int tiltLocations_1[PANTILTSERVOLOCATIONS] = {30  , 50 , 25 };

int panLocations_2[PANTILTSERVOLOCATIONS] = {13 , 60 , 35 };
int tiltLocations_2[PANTILTSERVOLOCATIONS] = {30 , 40 , 25 };

int panLocations_3[PANTILTSERVOLOCATIONS] = {80  , 70  , 100  };
int tiltLocations_3[PANTILTSERVOLOCATIONS] = {45  , 25 , 37 };

/* static sensor values
  valid values for shape sensors*/
int cubeMin = 20;
int cubeMax = 100;
int triangleMin = 500;
int triangleMax = 600;
int tunnelMin = 780;
int tunnelMax = 1000;

/* tracking variables (keep track of in-program values)
    keep track of which sensor we care about & its value  */
int shapeSensorOfInterest = 0;
int16_t shapeSensorValue = 0;

//keeping track between toggling servo laser & sensor LEDs
int laserMinutes = 0;
int laserSeconds = 0;

//keep track of how many correct values the sensors retrieve in a row before shape sensor is "activated"
int correctValueCube = 0;
int correctValueTriangle = 0;
int toggleLaserAmount = 0;        //program value, number the laser has turned ON

//variable to keep track of time
time_t t;
int t_seconds;
int t_minutes;


int isGameComplete = 2;       // 0 = start laser puzzle
int isLasersOn = 0;           //1 = turn on all lasers when new game begins
int checkTripLasers = 0;      //1 = trip sensors are checked       
int isReset = 0;              //1 = resets all variables for lazer puzzle 
int isTripSensorTripped = 0;

    // Bluetooth variables/setupbles
String inData;
String token;
String trippedLaser = "400";
String gameIsComplete = "402";

/*      function    section    */
void setupShapeLaser_andSensors(){
  pinMode(shapeLaser,OUTPUT);
  pinMode(cubeShapeSensor,OUTPUT);
  pinMode(triangleShapeSensor,OUTPUT);
  pinMode(tunnelShapeSensor,OUTPUT);
}

void setupServo(){
  panServo.attach(panPin);
  tiltServo.attach(tiltPin);
}


//manage the toggling of laser & sensor LEDs
void updateTimeLaser(){
  time_t laserTime = now();
  laserMinutes = minute(laserTime);
  laserSeconds = second(laserTime);
  toggleLaserAmount++;
}

//turn on all necessary objects for laser puzzle
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
//turn off all necessary objects for laser puzzle
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

//manage the blinking sequencing for laser and glowing shapes
void toggleLight(int light, int mainSeconds, int mainMinutes, int laserSeconds, int laserMinutes){
  int currState = digitalRead(light);
  
  //if state is on, wait 3 seconds and turn off
  int elapsedSeconds =  mainSeconds - laserSeconds;
  int elapsedMinutes = mainMinutes - laserMinutes;
  
  if (currState == HIGH){
    if (elapsedMinutes >= 1){
      toggleEverythingOff();
      updateTimeLaser();
    }else if (elapsedSeconds >= 3){
      toggleEverythingOff();
      updateTimeLaser();
    }
    else{}
  }
  else if (currState == LOW){
    if (elapsedMinutes >= 1){
      toggleEverythingOn();
      updateTimeLaser();
    }else if (elapsedSeconds >= 1){
      toggleEverythingOn();
      updateTimeLaser();
    } else{}
  }
}

int tripSensorTripped_BlinkingAmount = 3; // amount of blinking laser on cycles
int delayBetweenBlinks_TripSensorIsTripped_Milliseconds = 100;
//actions when a trip sensor is tripped.  Reset all necessary values
void tripSensorIsTripped(){
  if (isTripSensorTripped == 0){
    isTripSensorTripped = 1;
    if( shapeSensorOfInterest != 0 ){
      if ( isGameComplete == 0 ) {
        Serial1.println(trippedLaser);
        shapeSensorOfInterest = 0;
        correctValueCube = 0;
        correctValueTriangle = 0;
        isGameComplete = 0;

        for(int i = 0 ; i < (tripSensorTripped_BlinkingAmount * 2); i ++){
          // blink all lasers several times
          //assuming all lasers are on when sensor is tripped
          for (int i = firstPin_triggerLaser; i < (firstPin_triggerLaser + AMOUNT_OF_TRIGGER_LASERS); i ++){
             digitalWrite( i, !digitalRead(i));
          }
          delay(delayBetweenBlinks_TripSensorIsTripped_Milliseconds);
        }
        digitalWrite(cubeShapeSensor, HIGH);
        digitalWrite(triangleShapeSensor, LOW);
        digitalWrite(tunnelShapeSensor, LOW);
      }else if (isGameComplete == 1){
        Serial1.println(trippedLaser);
        isGameComplete = 2;       //failed game
        digitalWrite(cubeShapeSensor, LOW);
        digitalWrite(triangleShapeSensor, LOW);
        digitalWrite(tunnelShapeSensor, LOW);
      }else{}
    }else{}//dont do anything if shape of interest is shape 0
  }else{}// dont do anything if sensor is tripped more than 1 time
}


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

  //setup all lasers 
  for (int i = firstPin_triggerLaser; i < (firstPin_triggerLaser + AMOUNT_OF_TRIGGER_LASERS); i ++){
     pinMode(i, OUTPUT);
  }
  //isReset = 1;              //testing
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
        Serial1.println("!");          } 
      if (token == "401") {                       // start game
        isGameComplete = 0;
        isLasersOn = 0;
        checkTripLasers = 1;
        isTripSensorTripped = 0;
        shapeSensorOfInterest = 0;
      }else if (token == "999"){                  //reset game
        isGameComplete = 2;
        isLasersOn = 0;
        checkTripLasers = 0;
        isTripSensorTripped = 0;
        isReset = 1; 
      }else {
        Serial1.println("!");  
      }
      
    } //end while inData
  } // end if serial available
  
  if (isReset == 1  ){
    //turn off lasers 
    //turn off all shapes
    digitalWrite(shapeLaser, LOW);
    digitalWrite(cubeShapeSensor   , LOW);
    digitalWrite(triangleShapeSensor   , LOW);
    digitalWrite(tunnelShapeSensor   , LOW);
  
    //turn off all trigger lasers -- pin 30-46
    if (digitalRead(firstPin_triggerLaser) == HIGH){
      for (int i = firstPin_triggerLaser; i < (firstPin_triggerLaser + AMOUNT_OF_TRIGGER_LASERS); i ++){
         digitalWrite(i, LOW);
      }
    }
    isReset = 0;
    //isGameComplete = 0;       //testing
    //checkTripLasers = 1;      //testing
  }


  if ( checkTripLasers == 1){
    Serial.println("checking trip sensors");
    int tempSensor;
    
    //for next 3 adc's, check all 4 channels
    for (int adcCount = 0; adcCount < tripSensorAdcAmount; adcCount ++){
      for (int i = 0; i < adcChannels; i ++){
        if (adcCount == 0){
          tempSensor = ads1015_2.readADC_SingleEnded(i);
          Serial.print("sensor  "),Serial.print(i),Serial.print(" : "), Serial.println(tempSensor);
          if (tempSensor < laserTripVal ){
            tripSensorIsTripped();
          }
        }
        else if (adcCount ==  1){
          tempSensor = ads1015_3.readADC_SingleEnded(i);
          if (tempSensor < laserTripVal  ){
            tripSensorIsTripped();
          }
        }
        else if ( adcCount == 2){
          tempSensor = ads1015_4.readADC_SingleEnded(i);
          if (tempSensor < laserTripVal  ){
            tripSensorIsTripped();
          }
        }else{}
      }
    }
    Serial.println("done checking all trip sensors");
  }


    if (isGameComplete == 0){         //start game
      if (isLasersOn == 0){
        Serial.print(" laser is off");
        isLasersOn = 1;
        digitalWrite(shapeLaser,HIGH);
        digitalWrite(cubeShapeSensor,HIGH);
        panServo.write(panLocations_1[panTiltIndex] );
        tiltServo.write(tiltLocations_1[panTiltIndex]);
  
        //turn on all lasers
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
      Serial.println(" "),Serial.print("sensor of interest: ");
      Serial.println(shapeSensorOfInterest),Serial.println( " ");
      
      //check all sensors
      // check shape laser sensors if active
      if (shapeSensorOfInterest == 0){
        shapeSensorValue = ads1015_1.readADC_SingleEnded(0);
        Serial.println("cube sensor: "),Serial.println(shapeSensorValue);
        //check if valid sensor value, if valid then change sensor of interest to next sensor
        if (correctValueCube == 0){
          if ( (shapeSensorValue > cubeMin) && (shapeSensorValue < cubeMax) ) {
            correctValueCube = 1;
          }
        }
        else if ( correctValueCube == 1){
          if ( (shapeSensorValue > cubeMin) && (shapeSensorValue < cubeMax) ) {
            digitalWrite(cubeShapeSensor,LOW);
            digitalWrite(triangleShapeSensor,HIGH);
            shapeSensorOfInterest = 1;
            isTripSensorTripped = 0;    //reset trip sensor value
          }
          else{
            correctValueCube = 0;
          }
        }else{}        
      }
      else if (shapeSensorOfInterest == 1){
        shapeSensorValue = ads1015_1.readADC_SingleEnded(1);
        Serial.println("triangle sensor: "),Serial.println(shapeSensorValue);
        if (correctValueTriangle == 0){
          if ( (shapeSensorValue > triangleMin) && (shapeSensorValue < triangleMax) ) {
            correctValueTriangle = 1;
          }
        }
        else if ( correctValueTriangle == 1){
          if ( (shapeSensorValue > triangleMin) && (shapeSensorValue < triangleMax) ) {
            digitalWrite(triangleShapeSensor,LOW); 
            digitalWrite(tunnelShapeSensor,HIGH);
            shapeSensorOfInterest = 2;
          }
          else{
            correctValueTriangle = 0;
          }
        }else{}
      }
      else if (shapeSensorOfInterest == 2){
        shapeSensorValue = ads1015_1.readADC_SingleEnded(2);
        Serial.println("tunnel sensor: "),Serial.println(shapeSensorValue);
        //check if valid sensor value, if valid then change sensor of interest to next sensor
        if ( (shapeSensorValue > tunnelMin) && (shapeSensorValue < tunnelMax) ){
          //puzzle is COMPLETE
          isGameComplete = 1;
          Serial1.println(gameIsComplete);
          digitalWrite(shapeLaser, LOW);
          digitalWrite(tunnelShapeSensor,LOW);
        }
    }else{}                              // do nothing
    Serial.println("at end of game play");
  }



  Serial.println("done loop");
}
