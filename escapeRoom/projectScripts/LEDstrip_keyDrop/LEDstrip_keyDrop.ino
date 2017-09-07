/*
      LEDstrip_keyDrop.ino

Purpose: provides code for Arduino for LED strip control and key Drop

key drop -> electromagnet
led strip -> 4 pin, +!2V & RGB pins
      -> change brightness via PWM

NOTE: ENSURE LED STRIP PINS ARE PWM CAPABLE !!
*/
int ledStripChange = 0;
int ledGreenPin = 11;
int ledBluePin = 12;
int ledRedPin = 13;
int ledBase = 0;
int ledEnd = 2;

//magnet variable
int magnetPin = 10;
int armedMagnetPin = 9;
int magnetCodeRange = 10;

void turnOnMagnetBox(){
  digitalWrite(magnetPin, HIGH);
  digitalWrite(armedMagnetPin,HIGH);
}
void turnOffMagnetBox(){
  digitalWrite(magnetPin, LOW);
  digitalWrite(armedMagnetPin,LOW);
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

int checkAnalogValue(int value){
  if( (value >= 0 ) && (value <= 255) ){
    return 1;
  }
  else{ 
    return 0;
  }
}

int redMax  = 150;
int blueMax = 125;
void setup() {
	//setup for magnet, magnet  is by default ON
	//Serial.begin(9600);	//testing
	Serial1.begin(9600);	//bluetooth - tx1,rx1
	pinMode(magnetPin, OUTPUT);
  pinMode(armedMagnetPin,OUTPUT);
	turnOnMagnetBox();

	analogWrite(ledBluePin, 0);
	analogWrite(ledRedPin, redMax);
	analogWrite(ledGreenPin, 0);

}

void loop() {
  
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
                analogWrite(ledRedPin, redMax);
                analogWrite(ledGreenPin, 0);
             }
             else if (functionCode == 2){
              //custom call 3, turn led strip blue
				        analogWrite(ledBluePin, blueMax);
                analogWrite(ledRedPin, 0);
                analogWrite(ledGreenPin, 0);
             }
             else{
             //under construction
             }
          }
        }
        else if (techNum == 3){
           if (codeNum == 1){
            turnOnMagnetBox();
           }
           else if (codeNum == 2){
            turnOffMagnetBox();
           }
          else{
          //under construction
          }
        }
        else{
          //not a valid code
          }
      }
  }
  
}
