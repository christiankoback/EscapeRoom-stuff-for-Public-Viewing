/*
SPI: - pin 50 = MISO  <- board's MISO 
       pin 51 = MOSI  <- board's MOSI
       pin 52 = SCK   <-board's SCK
       pin 53 = SS <- not needed since arduino is not a slave, so set to output




LED board:
-active listening when CS = high
 9V battery + 19ohm resistor => board VCC
- 4 frames are sent to LED board


  SPI.transfer();  // transfers data using SPI
  SPI.endTransaction();
*/
//#include <LED_Frame.h>
#include <SPI.h>


// use ->  LED_Frame library for LED management
/*LED_Frame red();
LED_Frame green();
LED_Frame blue();
LED_Frame buttons();
*/
//set pins for LED digital pins
int pin_LEDBoardSelect = 22;
int pin_MISO = 50;
int pin_ArduinoSlave = 53;  //set high so arduino is master
int pin_clock = 52;
int lights = 0;
/*
void setupLEDColor(int led, String color){
// ex color input =  #2366D1
//  divide the color into red, green, and blue
//      ->   string[0-1] = red, string[2-3] = green, string[4-5] = blue
//  place data in correct locations into the data
  
  red.updateLED(led, color.substring(0,1) );
  green.updateLED(led, color.substring(2,3) );
  blue.updateLED(led, color.substring(4,5) );
}
void sendLED_Frame(LED_Frame frame){
  // clock works at 50KHz, then one clock cycle is at 1/50K = 0.00002sec *180 => 0.0036sec = 4 millisec
  digitalWrite(pin_ArduinoSlave, LOW);
  digitalWrite(pin_LEDBoardSelect, HIGH);
  
  SPI.beginTransaction(SPISettings(50000, LSBFIRST, SPI_MODE2));
  
  SPI.transfer( frame.getLEDs0_3() );
  SPI.transfer( frame.getLEDs4_7() );
  SPI.transfer( frame.getLEDs8_11() );
  SPI.transfer( frame.getLEDs12_15() );

  SPI.endTransaction();
  digitalWrite(pin_LEDBoardSelect, LOW);
  digitalWrite(pin_ArduinoSlave, HIGH);
}
void sendTestColor(unint8_t color){
  digitalWrite(pin_ArduinoSlave, LOW);
  digitalWrite(pin_LEDBoardSelect, HIGH);
  SPI.beginTransaction(SPISettings(50000, LSBFIRST, SPI_MODE2));
  
  SPI.transfer( frame.getLEDs0_3() );
  
  SPI.endTransaction();
  digitalWrite(pin_LEDBoardSelect, LOW);
  digitalWrite(pin_ArduinoSlave, HIGH);
}

*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("setup");

  //set SS to high, arduino is a master
  pinMode(pin_ArduinoSlave, OUTPUT);
  digitalWrite(pin_ArduinoSlave, HIGH);
  
  pinMode(pin_LEDBoardSelect, OUTPUT);
  pinMode(pin_MISO, OUTPUT);
  pinMode(pin_clock, OUTPUT);

  digitalWrite(pin_LEDBoardSelect, LOW);  //set CS to low, LED board ignores data on MISO
  delay(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("loop");
  SPI.beginTransaction(SPISettings(50000, LSBFIRST, SPI_MODE2));
  // "The Button Pad Controller SPI will begin listening
  //  for a data set once the CS input has been brought high."
  
  // "(HINT: It's best if the SCK signal is set high before
  //  setting the CS pin high)."
  //digitalWrite(pin_clock, HIGH);
  digitalWrite(pin_LEDBoardSelect, HIGH);
  delayMicroseconds(15);

  // Blink the lights.  
  //lights = !lights;
  
  // Four frames (3 for lights input, 1 for button output)
  // (Here, it is writing to output on the 4th as well, which
  // is wrong, but seems not to cause any issues).
  for(int f = 0; f < 4; f++) {
    
      // Each frame has 16 * 8 = 128 bits of data.
      for(int i = 0; i < 16; i++) {
        
          // "The data on the MISO line should be set while the
          //  clock is low. When the SCK signal goes high the
          //  Button Pad Controller locks in the data currently
          //  on the MISO line."
          digitalWrite(pin_clock, LOW);
          delayMicroseconds(5);
          
          digitalWrite(pin_MISO, lights);
          delayMicroseconds(5);
          
          digitalWrite(pin_clock, HIGH);
          delayMicroseconds(10);
      }
  }
  
  digitalWrite(pin_LEDBoardSelect, LOW);
 
  Serial.println("end");
  
  // "The CS signal should remain high for the duration of the
  //  data set, at which point it should be brought low for a
  //  minimum of 400 Î¼s before sending the next set of data."
  
  //delayMicroseconds(400);
  delay(500);
}
