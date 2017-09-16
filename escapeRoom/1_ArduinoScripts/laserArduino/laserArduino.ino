/* purpose: handles all lasers

*/
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>


//servo range is anything above 10 and less than 40
//create instance of ADCs
Adafruit_ADS1015 ads1015_1(0x48);
Adafruit_ADS1015 ads1015_2(0x49);
Adafruit_ADS1015 ads1015_3(0x4A);
Adafruit_ADS1015 ads1015_4(0x4B);
/*
 *blue = pan   = pin 12
 *yellow = tilt = pin 13
 */
int servoLaser = 22;
int servoLaserLED = 23;
int tiltPin = 13;
int panPin = 12;
int servoSensorAdc = 0;
int16_t servoSensor;
int servoSensorMin = 10;
int servoSensorMax = 40;

int toggleLEDVal = 0;
int toggleServoLaserVal = 0;
void toggleServoLED(){
  if (toggleLEDVal == 0){
    digitalWrite(servoLaserLED, HIGH);
    toggleLEDVal = 1;
  }
  else {
    digitalWrite(servoLaserLED, LOW);
    toggleLEDVal = 0;
  }
}
void toggleServoLaser(){
  if (toggleServoLaserVal == 0){
    digitalWrite(servoLaser, HIGH);
    toggleServoLaserVal = 1;
  }
  else {
    digitalWrite(servoLaser, LOW);
    toggleServoLaserVal = 0;
  }
}
Servo panServo;
Servo tiltServo;

//the location value must be between 0 and 180
const int PANTILTSERVOLOCATIONS = 3;
int panLocations[PANTILTSERVOLOCATIONS] = {13 , 30 , 35 };
int tiltLocations[PANTILTSERVOLOCATIONS] = {30 , 45 , 25 };
int panTiltIndex = 0;

void startServoLaser(){
  digitalWrite(servoLaser, HIGH);
  digitalWrite(servoLaserLED, HIGH);
  panServo.write (panLocations[0] );
  tiltServo.write( tiltLocations[0] );
  delay(30);    //wait for servo to reach location
}
void stopServoLaser(){
  digitalWrite(servoLaser, LOW);
  digitalWrite(servoLaserLED, LOW);
}
void changeServoLocation(){
  if (panTiltIndex >= 3){
    panTiltIndex = 0;
  }
  panServo.write (panLocations[panTiltIndex] );
  tiltServo.write( tiltLocations[panTiltIndex] );
  panTiltIndex ++;
}
void setupServoLaser(){
  panServo.attach(panPin);
  tiltServo.attach(tiltPin);
  pinMode(servoLaser, OUTPUT);
  pinMode(servoLaserLED,OUTPUT);
  startServoLaser();
  
}
void startLaser(int laserPin){
  digitalWrite(laserPin, HIGH);
}
void stopLaser(int laserPin){
  digitalWrite(laserPin, LOW);
}
void setupLaser(int laserPin){
  pinMode(laserPin, OUTPUT);
  startLaser(laserPin);
}

//bluetooth stuff
#ifndef BT_SETUP__
#define BT_SETUP__
const int btCodeLength = 3;
byte *BTCode;

void sendBTCode(char * strWord,int strLength){
  Serial1.write( "0" ); //garbage
  for (int i = 0; i < strLength; i++){
    Serial1.write( strWord[i] );
  }
}

byte* getBTCode(int strLength){
  byte message[strLength];
  if  (Serial1.available() ){
    //char garbage = Serial1.read();
    while(Serial1.available() ){
      for (int i = 0; i < strLength ; i ++){
        message[i] = Serial1.read();
      }
    }
  }
  else{
    message[0] = ' ';
  }
  return message;
}
#endif

//code command stuff
int panTiltChangeLocationCommand = 0;
int panTiltReset = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);

  ads1015_1.begin();
  ads1015_2.begin();
  ads1015_3.begin();
  ads1015_4.begin();

  setupServoLaser();
  //startServoLaser();    //testing
}
int codeLen = 3;
int tempLaser = 0;
int panIndex = 0;
void loop() {
  //code base
  /*
  servoSensor = ads1015_1.readADC_SingleEnded(0);
  Serial.print("adc value: ");
  Serial.println(servoSensor);
  toggleServoLED();
  toggleServoLaser();
  //delay(800);

  if (panIndex == 3){
    panIndex = 0;
  }
  panServo.write (panLocations[panIndex]);
  tiltServo.write (tiltLocations[panIndex]);
  panIndex++;
  delay(2000);
  */
  
  servoSensor = ads1015_1.readADC_SingleEnded(0);
  //Serial.print("adc value: ");
  //Serial.println(servoSensor);
  if( (servoSensor > servoSensorMin) && (servoSensor < servoSensorMax) ){
    stopServoLaser();
    char message[codeLen] = {'5', '0', '1' };
    sendBTCode(message,codeLen);
  }
  else{
    //Serial.println("     SENSOR IS   NOT    ON !!  ");
  }
 // delay(2000);

  // put your main code here, to run repeatedly:
  if  (Serial1.available() ){
      BTCode = getBTCode(btCodeLength);
      if (BTCode[0] != ' '  ){
        int techNum = BTCode[0] -'0';
        int codeNum = ( (BTCode[1] - '0') * 10 ) + (BTCode[2]- '0');

        if (techNum == 3){
          if(  codeNum  == panTiltChangeLocationCommand ){
            changeServoLocation();
          }
          else if (codeNum  == panTiltReset){
            panTiltIndex = 0;
            panServo.write (panLocations[panTiltIndex]);
            tiltServo.write (tiltLocations[panTiltIndex]);
            delay(30);    //wait for servo to reach location
          }
          else{
            //under construction
            }
        }
        else if (techNum == 4){
          //manage all laser commands
          if (codeNum == 1){
            startServoLaser();
          }
        }
        else if (techNum == 5){
          //manage all sensor commands
        }
        else{
          //not a valid code
          }
      }
  }
}