#include <Time.h>           // include time library
#include <TimeLib.h>
#include <Servo.h>

#include <Adafruit_ADS1015.h>   //include adafruit adc library

//shape laser & sensor LEDs
int shapeLaser = 22;
int cubeShapeSensor = 23;
int triangleShapeSensor = 24;
int tunnelShapeSensor = 25;

int shapeSensorOfInterest = 0;
int16_t shapeSensorValue = 0;
int cubeMin = 10;
int triangleMin = 500;
int tunnelMin = 790;
int shapeMax = 1000;
 
int laserMinutes = 0;
int laserSeconds = 0;

int toggleLaserAmount = 0;        //program value
const int LASER_ON_BEFORE_LASER_LOCATION_CHANGE = 6;  //amount of off and on cycles before location changes


int tiltPin = 13;
int panPin = 12;
int isDefaultLocation = 0;

Servo panServo;
Servo tiltServo;

/*locations/angles for servo laser per shape sensor*/
const int PANTILTSERVOLOCATIONS = 3;
int panTiltIndex = 0;
int panLocations_1[PANTILTSERVOLOCATIONS] = {13 , 60 , 35 };
int tiltLocations_1[PANTILTSERVOLOCATIONS] = {30 , 40 , 25 };



//FILL       THESE   IN       BEFORE      USE
int panLocations_2[PANTILTSERVOLOCATIONS] = {130  ,  170 ,100   };
int tiltLocations_2[PANTILTSERVOLOCATIONS] = {30  , 50 , 25 };

int panLocations_3[PANTILTSERVOLOCATIONS] = {80  , 70  , 100  };
int tiltLocations_3[PANTILTSERVOLOCATIONS] = {45  , 25 , 37 };

void updateTimeLaser(){
  time_t laserTime = now();
  laserMinutes = minute(laserTime);
  laserSeconds = second(laserTime);
  toggleLaserAmount++;
}
void toggleLaser(int mainSeconds, int mainMinutes, int laserSeconds, int laserMinutes){
  int currState = digitalRead(shapeLaser);
/*
  //if state is on, wait 2 seconds and turn off
  if (currState == HIGH){
    int elapsedSeconds = mainSeconds - laserSeconds;
    if( elapsedSeconds >= 3 ){
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
  else{}*/

 
  //if state is on, wait 3 seconds and turn off
  int elapsedSeconds =  mainSeconds - laserSeconds;
  int elapsedMinutes = mainMinutes - laserMinutes;
  
  if (currState == HIGH){
    if (elapsedSeconds >= 3){
      digitalWrite(shapeLaser,LOW);
      updateTimeLaser();
    }
    else if (elapsedMinutes >= 1){
      digitalWrite(shapeLaser,LOW);
      updateTimeLaser();
    }
    else{}
  }
  else if (currState == LOW){
    if (elapsedSeconds >= 1){
      digitalWrite(shapeLaser,HIGH);
      updateTimeLaser();
    }
    else if (elapsedMinutes >= 1){
      digitalWrite(shapeLaser,HIGH);
      updateTimeLaser();
    }
    else{}
  }
  else{}
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
    if (elapsedSeconds >= 3){
      toggleEverythingOff();
      updateTimeLaser();
    }
    else if (elapsedMinutes >= 1){
      toggleEverythingOff();
      updateTimeLaser();
    }
    else{}
  }
  else if (currState == LOW){
    if (elapsedSeconds >= 1){
      toggleEverythingOn();
      updateTimeLaser();
    }
    else if (elapsedMinutes >= 1){
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
Adafruit_ADS1015 ads1015_2(0x49);
Adafruit_ADS1015 ads1015_3(0x4A);
Adafruit_ADS1015 ads1015_4(0x4B);

int NumOfADC_channels = 4;

//variable to keep track of time
time_t t;
int t_seconds;
int t_minutes;

 
void setup() {
  //testing 
  Serial.begin(9600);
  // start ADCs for doing conversions
  ads1015_1.begin();
  ads1015_2.begin();
  ads1015_3.begin();
  ads1015_4.begin();
  t = now();
  t_minutes = minute(t);
  t_seconds = second(t);


  panServo.attach(panPin);
  tiltServo.attach(tiltPin);

  
  // init laser
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(24,OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(40, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(43, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);


  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(28, HIGH);
  digitalWrite(29, HIGH);
  digitalWrite(30, HIGH);
  digitalWrite(31, HIGH);
  digitalWrite(32, HIGH);
  digitalWrite(33, HIGH);
  digitalWrite(34, HIGH);
  digitalWrite(35, HIGH);
  digitalWrite(36, HIGH);
  digitalWrite(37, HIGH);
  digitalWrite(38, HIGH);
  digitalWrite(39, HIGH);
  digitalWrite(40, HIGH);
  digitalWrite(41, HIGH);
  digitalWrite(42, HIGH);
  digitalWrite(43, HIGH);
  digitalWrite(44, HIGH);
  digitalWrite(45, HIGH);
  digitalWrite(46, HIGH);
}

void loop() {
  time_t currentTime = now();
  int currentTime_seconds = second(currentTime);
  int currentTime_minutes = minute(currentTime);

  //check if a second has passed, if passed then blink laser
 /* if(  ( currentTime_minutes - t_minutes) == 0 ) {
    if ( ( currentTime_seconds - t_seconds) >= 1 ){
      //toggle laser
      toogleLaser(currentTime_seconds, currentTime_minutes, laserSeconds, laserMinutes);
      
   }
  }
  else if (currentTime_minutes - t_minutes >= 1){
    //toggle laser
    toogleLaser(currentTime_seconds, currentTime_minutes, laserSeconds, laserMinutes);
  }*/
  int cube = 23;
  int triangle = 24;
  int tunnel = 25;
  
  toggleLight(shapeLaser, currentTime_seconds, currentTime_minutes, laserSeconds, laserMinutes);
  //toggleLaser(currentTime_seconds, currentTime_minutes, laserSeconds, laserMinutes);
  
  Serial.print("toggle laser amount : ");
  Serial.println(toggleLaserAmount);
  Serial.print("panTiltIndex : ");
  Serial.println(panTiltIndex);
  Serial.println(' ');

  
  //change location of the servo laser
  if (toggleLaserAmount >= LASER_ON_BEFORE_LASER_LOCATION_CHANGE){
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
    else{
      if (isDefaultLocation == 0){
        isDefaultLocation = 1;
        panServo.write(panLocations_1[panTiltIndex] );
        tiltServo.write(tiltLocations_1[panTiltIndex]);
      }
    }
    delay(500);
  }

/*
  //check all sensors
  // check shape laser sensors if active
  if (shapeSensorOfInterest == 0){
    shapeSensorValue = ads1015_1.readADC_SingleEnded(0);

    //check if valid sensor value, if valid then change sensor of interest to next sensor
    if ( (shapeSensorValue > cubeMin) && (shapeSensorValue < shapeMax){
      shapeSensorOfInterest = 1;
    }
  }
  else if (shapeSensorOfInterest == 1){
    shapeSensorValue = ads1015_1.readADC_SingleEnded(1);

    //check if valid sensor value, if valid then change sensor of interest to next sensor
    if ( (shapeSensorValue > triangleMin) && (shapeSensorValue < shapeMax){
      shapeSensorOfInterest = 2;
    }
  }
  else if (shapeSensorOfInterest == 2){
    shapeSensorValue = ads1015_1.readADC_SingleEnded(2);

    //check if valid sensor value, if valid then change sensor of interest to next sensor
    if ( (shapeSensorValue > tunnelMin) && (shapeSensorValue < shapeMax){
      //shapeSensorOfInterest = 2;
      //puzzle is COMPLETE
    }
  }



  // check all other laser sensors, if any sensors are NOT active then restart the box puzzle
  int tempSensor = ads1015_1.readADC_SingleEnded(3);
  
  //check if valid sensor value, if valid then change sensor of interest to next sensor
    if ( (tempSensor < shapeSensorMin) || (tempSensor > shapeSensorMax){
      shapeSensorOfInterest = 0;
    }

    for (int i = 0; i < NumOfADC_channels; i++){
      int tempSensor = ads1015_2.readADC_SingleEnded(i);
  
      //check if valid sensor value, if valid then change sensor of interest to next sensor
      if ( (tempSensor < shapeSensorMin) || (tempSensor > shapeSensorMax){
        shapeSensorOfInterest = 0;
      }
    }
    for (int i = 0; i < NumOfADC_channels; i++){
      int tempSensor = ads1015_3.readADC_SingleEnded(i);
  
      //check if valid sensor value, if valid then change sensor of interest to next sensor
      if ( (tempSensor < shapeSensorMin) || (tempSensor > shapeSensorMax){
        shapeSensorOfInterest = 0;
      }
    }
    for (int i = 0; i < NumOfADC_channels; i++){
      int tempSensor = ads1015_4.readADC_SingleEnded(i);
  
      //check if valid sensor value, if valid then change sensor of interest to next sensor
      if ( (tempSensor < shapeSensorMin) || (tempSensor > shapeSensorMax){
        shapeSensorOfInterest = 0;
      }
    }
*/
  
}
