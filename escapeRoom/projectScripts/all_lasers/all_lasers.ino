/*
 *  all_lasers.ino
 * 
 * Purpose : hook up and control all lasers with one arduino
 * 
 * laser # : 17
 * Pins : all digital pins on Arduino Mega (Pin 22 + 17)
 * 
 * 
 * NOTE : all pins for lasers must be consecutive starting at the Pin Base
 */


int digitalPinBase = 22;    //first digital pin to use for lasers
int numOfPinsToUse = 17;    //how many lasers to wire
int endOfPinsBorder = digitalPinBase + numOfPinsToUse;

void setup() {
  Serial.begin(9600);
  String laserPinNum = "";

  Serial.print("laser pins ( on ): ");
  // start at the Pin Base and set the next Pins to output, high mode 
  for( int i = digitalPinBase; i < endOfPinsBorder; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
    Serial.print(i);
    Serial.print(", ");
  }
  Serial.println("All lasers are on. ");
}

void loop() {
  // put your main code here, to run repeatedly:

}
