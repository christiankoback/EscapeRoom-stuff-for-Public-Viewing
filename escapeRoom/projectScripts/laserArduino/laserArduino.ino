/* purpose: handles all lasers

*/
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

//create instance of ADCs
Adafruit_ADS1015 ads1015_1(0x48);
Adafruit_ADS1015 ads1015_2(0x49);
Adafruit_ADS1015 ads1015_3(0x4A);
Adafruit_ADS1015 ads1015_4(0x4B);

int servoLaser = 22;
int servoLaserLED = 23;
int panPin = 13;
int tiltPin = 12;
int servoSensorAdc = 0;
int16_t servoSensor;

Servo panServo;
Servo tiltServo;

//the location value must be between 0 and 180
const int PANTILTSERVOLOCATIONS = 3;
int panLocations[PANTILTSERVOLOCATIONS] = {0 , 90 , 45 };
int tiltLocations[PANTILTSERVOLOCATIONS] = {45 , 90 , 0 };
int panTiltIndex = 0;

void startServoLaser(){
  digitalWrite(servoLaser, HIGH);
  digitalWrite(servoLaser, HIGH);
  panServo.write (panLocations[0] );
  tiltServo.write( tiltLocations[0] );
  delay(30);    //wait for servo to reach location
}
void stopServoLaser(){
  digitalWrite(servoLaser, LOW);
  digitalWrite(servoLaser, LOW);
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
int magnetBase = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);

  ads1015_1.begin();
  ads1015_2.begin();
  ads1015_3.begin();
  ads1015_4.begin();

  setupServoLaser();

}

void loop() {
  /*
  servoSensor = ads1015_1.readADC_SingleEnded(0);
  Serial.print("adc value: ");
  Serial.println(servoSensor);
  if( (servoSensor > 800) && (servoSensor < 1024) ){
    Serial.println("     SENSOR IS ON !!  ");
  }
  else{
    Serial.println("     SENSOR IS   NOT    ON !!  ");
  }
  delay(2000);
  stopServoLaser();
  //changeServoLocation();
  //delay(2000);
  */
  // put your main code here, to run repeatedly:
  if  (Serial1.available() ){
      BTCode = getBTCode(btCodeLength);
      if (BTCode[0] != ' '  ){
        int techNum = BTCode[0] -'0';
        int codeNum = ( (BTCode[1] - '0') * 10 ) + (BTCode[2]- '0');

        if (techNum == 3){
          if(  codeNum  == panTiltChangeLocationCommand ){
            panTiltIndex += 1;
            if ( panTiltIndex == PANTILTSERVOLOCATIONS ){
              panTiltIndex = 0;
            }
            //move pan tilt servo to next location, loop when out of new locations
            panServo.write( panLocations[panTiltIndex]);
            tiltServo.write (tiltLocations[panTiltIndex]);
            delay(30);    //wait for servo to reach location
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
