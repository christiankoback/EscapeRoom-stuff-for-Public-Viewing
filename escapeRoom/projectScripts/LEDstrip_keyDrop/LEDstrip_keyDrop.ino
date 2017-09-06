/*
      LEDstrip_keyDrop.ino

Purpose: provides code for Arduino for LED strip control and key Drop

key drop -> electromagnet
led strip -> 4 pin, +!2V & RGB pins
      -> change brightness via PWM

NOTE: ENSURE LED STRIP PINS ARE PWM CAPABLE !!
*/
int ledStripChange = 0;
int ledBluePin = 11;
int ledRedPin = 12;
int ledGreenPin = 13;
int ledBase = 0;
int ledEnd = 2;

//magnet variable
int magnetPin = 10;
int magnetCodeRange = 10;

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

int checkAnalogValue(int value){
  if( (value >= 0 ) && (value <= 255) ){
    return 1;
  }
  else{ 
    return 0;
  }
}
void setup() {
	//setup for magnet, magnet  is by default ON
	Serial.begin(9600);	//testing
	Serial1.begin(9600);	//bluetooth - tx1,rx1
	pinMode(magnetPin, OUTPUT);
	digitalWrite(magnetPin, HIGH);

	analogWrite(ledBluePin, 0);
	analogWrite(ledRedPin, 105);
	analogWrite(ledGreenPin, 0);

}

void loop() {
  // put your main code here, to run repeatedly:
 /*     //testing
  LED_allOn(255, 255, 255);
  delay(1000);
  LED_allOn(125, 125, 125);
  delay(1000);
  LED_allOn(0, 0, 0);
  delay(1000);
  */
  if  (Serial1.available() ){
      int CodeCount = 0;
      BTCode = getBTCode(CodeCount);
      if (BTCode[0] == ' '  ){
        //invalid Bluetooth code
      }
      else{
        int techNum = BTCode[0] -'0';
        int codeNum = ( (BTCode[1] - '0') * 10 ) + (BTCode[2]- '0');

        if (techNum == 2){
          if(  (codeNum >= ledBase) && (codeNum <= ledEnd) ){
             int functionCode = codeNum - ledBase;
             if (functionCode == 0){
                analogWrite(ledBluePin, 0);
                analogWrite(ledRedPin, 0);
                analogWrite(ledGreenPin, 0);
             }
             else if (functionCode == 1){
                //custom call 2, turn led strip red
                analogWrite(ledBluePin, 0);
                analogWrite(ledRedPin, 105);
                analogWrite(ledGreenPin, 0);
             }
             else if (functionCode == 2){
              //custom call 3, turn led strip blue
				analogWrite(ledBluePin, 125);
                analogWrite(ledRedPin, 0);
                analogWrite(ledGreenPin, 0);
             }
             else{/*under construction*/}
          }
        }
        else if (techNum == 3){
           if (codeNum == 1){
            digitalWrite(magnetPin, HIGH);
           }
           else if (codeNum == 2){
            digitalWrite(magnetPin, LOW);
           }
          else{/*under construction*/}
        }
        else{/*not a valid code*/}
      }
  }
}
