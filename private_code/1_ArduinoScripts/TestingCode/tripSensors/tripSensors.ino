/*
  SensorReading.ino ---- meant to test values that can be retrieved from solar cell
*/

#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Time.h>           // include time library
#include <TimeLib.h>

//shape laser & sensor LEDs
int shapeLaser = 22;
int cubeShapeSensor = 23;
int triangleShapeSensor = 24;
int tunnelShapeSensor = 25;

uint16_t tempSensor;
Adafruit_ADS1015 ads1015_1(0x48);
Adafruit_ADS1015 ads1015_2(0x4B);
Adafruit_ADS1015 ads1015_3(0x49); 
Adafruit_ADS1015 ads1015_4(0x4A);   

const int TRIP_LASER_AMOUNT = 13;
int adcChannels = 4;
int tripLaserMin[TRIP_LASER_AMOUNT];
int tripLaserMax[TRIP_LASER_AMOUNT];
int tripLaserPercentError = 80;

int firstPin_triggerLaser = 30;
const int AMOUNT_OF_TRIGGER_LASERS = 17;
int checkTripLasers = 0;
int isReset = 0;
int isGameComplete = 0;

void tripSensorIsTripped(){
  //Serial1.println("400");
  Serial.println(" ");
  Serial.println("trip sensors were tripped");
  Serial.println(" ");
}
void tripLaserConfig(int * laserArrayMin, int * laserArrayMax, int index, int count, int tempSensor){
  if (count = 0){
    laserArrayMin[index] = tempSensor;
    laserArrayMax[index] = tempSensor;
    count = 1;
  }
  //check if sensor value is less than current lowest value, if is then update
  if ( tempSensor < laserArrayMin[index]){
    laserArrayMin[index] = tempSensor;
  }
  //check if sensor value is greater than current highest value, if is then update
  if (tempSensor > laserArrayMax[index]){
    laserArrayMax[index] = tempSensor;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   // init computer screen ready to recieve info from Mega
  ads1015_1.begin();
  ads1015_2.begin();
  ads1015_3.begin();
  ads1015_4.begin();
  
  /*
  pinMode(shapeLaser, OUTPUT);
  pinMode(sensor1, OUTPUT);
  pinMode(sensor2, OUTPUT);
  pinMode(sensor3, OUTPUT);
  
  digitalWrite(shapeLaser, HIGH);
  digitalWrite(sensor1, HIGH);
  digitalWrite(sensor2, HIGH);
  digitalWrite(sensor3, HIGH);
*/

 for (int i = firstPin_triggerLaser; i < (firstPin_triggerLaser + AMOUNT_OF_TRIGGER_LASERS); i ++){
     pinMode(i, OUTPUT);
  }

  isReset = 1;              //testing
}

//int tripLaserMinTemp[TRIP_LASER_AMOUNT] = {180 ,120  ,620 , 578, 770 , 880,794, 614 , 800 , 840, 616, 430  };
// int tripLaserMaxTemp[TRIP_LASER_AMOUNT] = { 1050 , 800  ,   ,    ,     ,     ,     ,     ,      ,    , }
int tripLaserMinCheck[TRIP_LASER_AMOUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
void loop() {


  
  if (isReset == 1  ){
    
    //turn on lasers
    if (digitalRead(firstPin_triggerLaser) == LOW){
      //turn off all trigger lasers -- pin 30-46
      for (int i = firstPin_triggerLaser; i < (firstPin_triggerLaser + AMOUNT_OF_TRIGGER_LASERS); i ++){
         digitalWrite(i, HIGH);
      }
    }
    delay(300);
    //Serial.print(" are all lasers on? press any key");
    //byte answer = Serial.read();
    
    // check trip lasers and log lowest & highest values
    time_t currentTempTime = now();
    int tempSeconds = second(currentTempTime);
    int tempMinutes = minute(currentTempTime);
    int timeKeeper = 0;
    
    int tripLaserMinTemp[TRIP_LASER_AMOUNT];
    int tripLaserMaxTemp[TRIP_LASER_AMOUNT];
    int count = 0;
    int adcOfInterest = 0;
    int tempSensor;
    
    int configTimeInSeconds = 5;


    //Serial.println("Timekeeper 1:");
    //clear all garbage values
    /*
    int continueOuterWhileLoop = 1;
    while (continueOuterWhileLoop == 1){
      time_t loopTime = now();
      int currentTime = second(loopTime);
      if ( currentTime < tempSeconds){
        timeKeeper = 60 - tempSeconds - currentTime;
      }
      else{
        timeKeeper = currentTime  - tempSeconds;
      }

      
      if( timeKeeper < configTimeInSeconds){
        //Serial.print("timekeeper : "), Serial.println(timeKeeper);
        //for next 3 adc's, check all 4 channels
        for (int adcCount = 0; adcCount < (adcChannels - 1); adcCount ++){
          for (int i = 0; i < adcChannels; i ++){
            if (adcCount == 0){
              tempSensor = ads1015_2.readADC_SingleEnded(i);
              //tripLaserConfig(tripLaserMinTemp, tripLaserMaxTemp, adcOfInterest, count,tempSensor);
            }
            else if (adcCount ==  1){
              tempSensor = ads1015_3.readADC_SingleEnded(i);
              //tripLaserConfig(tripLaserMinTemp, tripLaserMaxTemp, adcOfInterest, count,tempSensor);
            }
            else if ( adcCount == 2){
              tempSensor = ads1015_4.readADC_SingleEnded(i);
              //tripLaserConfig(tripLaserMinTemp, tripLaserMaxTemp, adcOfInterest, count,tempSensor);
            }else{}
            adcOfInterest++;
          }
        }
      }
      if (timeKeeper >= configTimeInSeconds){
        continueOuterWhileLoop = 0;
      }
    }
    */
    //usable values
    tempSensor = 0;
    int continueOuterWhileLoop = 1;
    //Serial.println("Timekeeper 2:");
    while (continueOuterWhileLoop == 1){
      time_t loopTime = now();
      int currentTime = second(loopTime);
      if ( currentTime < tempSeconds){
        timeKeeper = 60 - tempSeconds - currentTime;
      }
      else{
        timeKeeper = currentTime  - tempSeconds;
      }
      if( timeKeeper < configTimeInSeconds){
        //Serial.print("timekeeper : "), Serial.println(timeKeeper);
        //for next 3 adc's, check all 4 channels
        for (int adcCount = 0; adcCount < (adcChannels - 1); adcCount ++){
          for (int i = 0; i < adcChannels; i ++){
            if (adcCount == 0){
              tempSensor = ads1015_2.readADC_SingleEnded(i);
              tripLaserConfig(tripLaserMinTemp, tripLaserMaxTemp, adcOfInterest, count,tempSensor);
            }
            else if (adcCount ==  1){
              tempSensor = ads1015_3.readADC_SingleEnded(i);
              tripLaserConfig(tripLaserMinTemp, tripLaserMaxTemp, adcOfInterest, count,tempSensor);
            }
            else if ( adcCount == 2){
              tempSensor = ads1015_4.readADC_SingleEnded(i);
              tripLaserConfig(tripLaserMinTemp, tripLaserMaxTemp, adcOfInterest, count,tempSensor);
            }else{}
            adcOfInterest++;
          }
        }
      }
      if (timeKeeper >= configTimeInSeconds){
        continueOuterWhileLoop = 0;
      }
    }

   
    //set values to global variables 
    for (int i = 0; i < TRIP_LASER_AMOUNT; i ++){
      tripLaserMin[i] = tripLaserMinTemp[i] - (int)(tripLaserMaxTemp[i]*tripLaserPercentError/100 );
      tripLaserMax[i] = tripLaserMaxTemp[i] /*+ (int)(tripLaserMaxTemp[i]*tripLaserPercentError/100 )*/;
      Serial.print("min laser channel "), Serial.print(i), Serial.print(" : "), Serial.println(tripLaserMin[i]);
      Serial.print("max laser channel "), Serial.print(i), Serial.print(" : "), Serial.println(tripLaserMax[i]);
    }

    
    //turn off lasers 
    //turn off all shapes
    digitalWrite(shapeLaser, LOW);
    digitalWrite(cubeShapeSensor   , LOW);
    digitalWrite(triangleShapeSensor   , LOW);
    digitalWrite(tunnelShapeSensor   , LOW);

  /*
   //turn off all trigger lasers -- pin 30-46
    for (int i = firstPin_triggerLaser; i < (firstPin_triggerLaser + AMOUNT_OF_TRIGGER_LASERS); i ++){
       digitalWrite(i, LOW);
    }
*/
    //Serial.print(" are all lasers off? press any key");
    //answer = Serial.read();
    
    isReset = 0;
    checkTripLasers = 1;
    isGameComplete = 0;
  }











  
  if ( checkTripLasers == 1){
    int adcOfInterest =0;
    
    //for next 3 adc's, check all 4 channels
    for (int adcCount = 0; adcCount < (adcChannels - 1); adcCount ++){
      for (int i = 0; i < adcChannels; i ++){
        if (adcCount == 0){
          tempSensor = ads1015_2.readADC_SingleEnded(i);
          Serial.print("sensor  "),Serial.print(i ),Serial.print(" : "), Serial.println(tempSensor);
          
          if (tempSensor < tripLaserMin[adcOfInterest]  ){
            if ( tripLaserMinCheck[adcOfInterest] == 0){
              tripLaserMinCheck[adcOfInterest] = 1;
            }else if (tripLaserMinCheck[adcOfInterest] == 1){
              tripSensorIsTripped();
              tripLaserMinCheck[adcOfInterest] = 0;
            }else{}
          }
        }
        else if (adcCount ==  1){
          tempSensor = ads1015_3.readADC_SingleEnded(i);
          Serial.print("sensor  "),Serial.print(i + (adcCount*4)),Serial.print(" : "), Serial.println(tempSensor);
          if (tempSensor < tripLaserMin[adcOfInterest]  ){
            if ( tripLaserMinCheck[adcOfInterest] == 0){
              tripLaserMinCheck[adcOfInterest] = 1;
            }else if (tripLaserMinCheck[adcOfInterest] == 1){
              tripSensorIsTripped();
              tripLaserMinCheck[adcOfInterest] = 0;
            }else{}
          }
        }
        else if ( adcCount == 2){
          Serial.print("sensor  "),Serial.print(i + (adcCount*4)),Serial.print(" : "), Serial.println(tempSensor);
          tempSensor = ads1015_4.readADC_SingleEnded(i);
          if (tempSensor < tripLaserMin[adcOfInterest]  ){
            if ( tripLaserMinCheck[adcOfInterest] == 0){
              tripLaserMinCheck[adcOfInterest] = 1;
            }else if (tripLaserMinCheck[adcOfInterest] == 1){
              tripSensorIsTripped();
              tripLaserMinCheck[adcOfInterest] = 0;
            }else{}
          }
        }else{}
        adcOfInterest++;
      }
     Serial.println(" ");
    }
    //Serial.print("     press any key");
    //byte answer = Serial.read();
  }







  
}
