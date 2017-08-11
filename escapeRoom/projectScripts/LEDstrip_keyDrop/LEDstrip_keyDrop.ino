/*
      LEDstrip_keyDrop.ino

Purpose: provides code for Arduino for LED strip control and key Drop

key drop -> electromagnet
led strip -> 4 pin, +!2V & RGB pins
      -> change brightness via PWM

NOTE: ENSURE LED STRIP PINS ARE PWM CAPABLE !!
*/
int ledStripChange = 0;
int ledRedPin = 5;
int ledBluePin = 6;
int ledGreenPin = 9;

/*
void startTesting(){
  Serial.begin(9600);
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
}

int checkAnalogValue(int value){
  if( (value >= 0 ) && (value <= 255) ){
    return 1;
  }
  else{ 
    return 0;
  }
}

void LED_allOn(int redValue, int blueValue, int greenValue){
  if(  ( checkAnalogValue(redValue) == 1) && ( checkAnalogValue(blueValue) == 1) && ( checkAnalogValue(greenValue) == 1) ){
    analogWrite(ledRedPin,redValue);
    analogWrite(ledBluePin,blueValue);
    analogWrite(ledGreenPin,greenValue);
  }
  else{
    Serial.println("Values for LEDs must be between 0 and 255");
  }
}

void LED_blueOnONLY( int blueValue){
  if ( checkAnalogValue(blueValue) == 1 ){
    analogWrite(ledRedPin,0);
    analogWrite(ledBluePin,blueValue);
    analogWrite(ledGreenPin,0);
  }
  else{
    Serial.println("Values for blue LEDs must be between 0 and 255");
  }
}
void LED_redOnONLY(int redValue){
  if ( checkAnalogValue(redValue) == 1 ){
    analogWrite(ledRedPin,redValue);
    analogWrite(ledBluePin,0);
    analogWrite(ledGreenPin,0);
  }
  else{
    Serial.println("Values for red LEDs must be between 0 and 255");
  }
}
void LED_greenOnONLY(int greenValue){
  if ( checkAnalogValue(greenValue) == 1 ){
  analogWrite(ledRedPin,0);
  analogWrite(ledBluePin,0);
  analogWrite(ledGreenPin,greenValue);
  }
  else{
    Serial.println("Values for green LEDs must be between 0 and 255");
  }
}
*/
int turnOn = 0 ;  //0 when off,1 if on 
int loopCount = 0;  //testing
int keyDropSensor = 2;

void setup() {
  //setup led strip to default (all lights on)
  //setup magnet to work by default
  startTesting();
  //Serial.println("starting program");   //testing
  pinMode(keyDropSensor, OUTPUT);
  
/*    //the below works for on-off control
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  digitalWrite(ledRedPin, HIGH);
  digitalWrite(ledBluePin, HIGH);
  digitalWrite(ledGreenPin, HIGH);
  */
//  digitalWrite(keyDropSensor, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  LED_allOn(255, 255, 255);
  delay(1000);
  LED_allOn(125, 125, 125);
  delay(1000);
  LED_allOn(0, 0, 0);
  delay(1000);
/*
  LED_allOn(255, 140, 0);     //red, blue, green consecutively
  Serial.println("led 1 is on");
  delay(2000);
  LED_allOn(255, 255, 0);     //red, blue, green consecutively
  Serial.println("led 2 is on");
  delay(2000);
  
  if ( (loopCount % 2) == 0 ){
    turnOn = 0;
  }
  else if ( ( loopCount % 5) == 0){
    turnOn = 1;
  }
  else{}
  */
  //if signal is HIGH, magnet is on  --- NC is XX, if signal is LOW, NC is HIGH when on and NC is LOW when off
  //turn magnet off
 /* if (turnOn == 0){
    digitalWrite(keyDropSensor, LOW);
    Serial.println("magnet off");   //testing
  }
  else if (turnOn == 1){
    //turn the magnet on
    digitalWrite(keyDropSensor, HIGH);
    Serial.println("magnet on");   //testing
  }
  else{}
  */
  /*
  digitalWrite(keyDropSensor, HIGH);
 // Serial.println("magnet on");   //testing
  delay(5000);
  
  
  digitalWrite(keyDropSensor, LOW);
  //Serial.println("magnet off");   //testing
  delay(5000);
*/
 // loopCount += 1;       //testing
 // delay(1000);
/*
  // change LED strip on programmable trigger ( ? )
  //    if so, do that here
  if (ledStripChange == 1){
    // if 1, change led strip to purely green
    //not done yet

    ledStripChange = 0;
  }
  else if (ledStripChange == 2){
    //if 2, change to purely red
    //not done yet

    ledStripChange = 0;
  }
  else if (ledStripChange == 3){
    // if 3, change to purely blue
    //not done yet

    ledStripChange = 0;
  }
  else{}
  
  // check sensor for key drop goal
  //    if goal is reached, release key

*/
}
