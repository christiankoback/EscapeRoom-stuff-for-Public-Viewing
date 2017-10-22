/*
  SensorReading.ino ---- meant to test values that can be retrieved from solar cell
*/

#include <Wire.h>
#include <Adafruit_ADS1015.h>


Adafruit_ADS1015 ads1015(0x48);
Adafruit_ADS1015 ads1015_2(0x4B);

int shapeLaser = 22;
int sensor1  =23;
int sensor2 = 24;
int sensor3 = 25;


/*cube active = 25, off = 4
 * triangle active = 550   , off = 468
 * tunnnel active = 750+     , off = 719
 * 
 */
 int cubeMin = 10;
 int triangleMin = 570;
 int tunnelMin = 790;
 int shapeMax = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   // init computer screen ready to recieve info from Mega
  ads1015.begin();
  ads1015_2.begin();
  pinMode(shapeLaser, OUTPUT);
  pinMode(sensor1, OUTPUT);
  pinMode(sensor2, OUTPUT);
  pinMode(sensor3, OUTPUT);
  
  digitalWrite(shapeLaser, HIGH);
  digitalWrite(sensor1, HIGH);
  digitalWrite(sensor2, HIGH);
  digitalWrite(sensor3, HIGH);

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
  int16_t adc0, adc1, adc2;
 
  /*if (countDown == 0 ){
    countDown = 10;
  }
  if ( (countDown % 2 ) == 0){
    digitalWrite(laserPin, HIGH);
  }
  else{
    digitalWrite(laserPin, LOW);
  }
  */
  adc0 = ads1015.readADC_SingleEnded(0);
  adc1 = ads1015.readADC_SingleEnded(1);
  adc2 = ads1015.readADC_SingleEnded(2);


  int16_t sensorSeries = ads1015_2.readADC_SingleEnded(0);
  
 if (  (adc0 > cubeMin)  &&   (adc0 < shapeMax)){
    Serial.print("cube sensor: "); Serial.println(adc0);
  }
  if (  (adc1 > triangleMin)  &&   (adc1 < shapeMax)){
    Serial.print("triangle : "); Serial.println(adc1);
  }
  if (  (adc2 > tunnelMin)  &&   (adc2 < shapeMax)){
    Serial.print("tunnel: "); Serial.println(adc2);
  }
 
  int tripLaserMin = 10;
  int tripLaserMax = 500;
  int adcChannelAmount = 3;
  for (int i = 0; i < adcChannelAmount; i ++){
    int16_t tempSensor = ads1015_2.readADC_SingleEnded(i);
    if (  (tempSensor < tripLaserMin) || (tempSensor > tripLaserMax)  ){
      Serial1.println("400");
      shapeSensorOfInterest = 0;
    }
  }
 Serial.println(" ");
  delay(500);
}
