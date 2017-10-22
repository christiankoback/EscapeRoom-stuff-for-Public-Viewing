#include <Time.h>           // include time library
#include <TimeLib.h>

#include <Adafruit_ADS1015.h>   //include adafruit adc library

//shape laser & sensor LEDs
int shapeLaser = 22;
int shapeSensor1  =23;
int shapeSensor2 = 24;
int shapeSensor3 = 25;

int shapeSensorOfInterest = 0;
int16_t shapeSensorValue = 0;
int cubeMin = 10;
int triangleMin = 500;
int tunnelMin = 790;
int shapeMax = 1000;
 
int laserMinutes = 0;
int laserSeconds = 0;

int toggleLaserAmount = 0;        //program value
const int LASER_ON_BEFORE_LASER_LOCATION_CHANGE = 2;  

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
Adafruit_ADS1015 ads1015_2(0x49);
Adafruit_ADS1015 ads1015_3(0x4A);
Adafruit_ADS1015 ads1015_4(0x4B);

int NumOfADC_channels = 4;

//variable to keep track of time
time_t t;
int t_seconds;
int t_minutes;

 
void setup() {
  // start ADCs for doing conversions
  ads1015_1.begin();
  ads1015_2.begin();
  ads1015_3.begin();
  ads1015_4.begin();
  t = now();
  t_minutes = minute(t);
  t_seconds = second(t);
  // init laser
}

void loop() {
  time_t currentTime = now();
  currentTime_seconds = second(currentTime);
  currentTime_minutes = minute(currentTime);

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

}
