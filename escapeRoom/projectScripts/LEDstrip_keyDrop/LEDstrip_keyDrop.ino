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
int ledBase = 31;
int ledEnd = 40;

//magnet variable
int magnetPin = 10;
int magnetCodeRange = 10;

//bluetooth stuff
#ifndef BT_SETUP__
#define BT_SETUP__
const int btCodeLength = 3;
byte BTCode[btCodeLength];

void sendBTCode(char * strWord,int strLength){
  Serial1.write( "0" ); //garbage
  for (int i = 0; i < strLength; i++){
    Serial1.write( strWord[i] );
  }
}

char* getBTCode(int strLength){
  char message[strLength];
  if  (Serial1.available() ){
    char garbage = Serial1.read();
    while(Serial1.available() ){
      for (int i = 0; i < strLength ; i ++){
        message[i] = Serial1.read();
      }
    }
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
  pinMode(magnetPin, OUTPUT);
  digitalWrite(magnetPin, HIGH);
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
      while(Serial1.available() ){
        BTCode[CodeCount] = Serial1.read();
        CodeCount ++;
      }
      if (CodeCount >= btCodeLength  ){
        //invalid Bluetooth code
      }
      else{
        int techNum = BTCode[0] -'0';
        int codeNum = ( (BTCode[1] - '0') * 10 ) + (BTCode[2]- '0');

        if (techNum == 2){
          if(  (codeNum >= ledBase) && (codeNum <= ledEnd) ){
             int functionCode = codeNum - ledBase;
             if (functionCode == 0){
                LED_allOff();
             }
             else if (functionCode == 1){
              //custom call 2
             }
             else if (functionCode == 2){
              //custom call 3
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
