/*
  SensorReading.ino ---- meant to test values that can be retrieved from solar cell
*/

#include <Wire.h>
#include <Adafruit_ADS1015.h>


Adafruit_ADS1015 ads1015;

int laserPin = 22;
int countDown = 10;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   // init computer screen ready to recieve info from Mega
  ads1015.begin();
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, HIGH);
}

void loop() {
  int16_t adc0, adc1;
 
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
  Serial.print("Ain0: "); Serial.println(adc0);
  if( (adc0 > 800) && (adc0 < 1024) ){
    Serial.println("     SENSOR IS ON !!  ");
  }
  //Serial.print("Ain1: "); Serial.println(adc1);

  //countDown -= 1;
  delay(1000);
}
