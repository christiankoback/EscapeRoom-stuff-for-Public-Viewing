/*
  magnetArduino.ino   ---- Escape Room 1 Season 1
  Purpose:  - manage all functionality of:
                  LED light strip
                  magnet release
*/

            //led strip variables/setup
int ledStripChange = 0;
int ledGreenPin = 11;
int ledBluePin = 12;
int ledRedPin = 13;
int ledBase = 0;
int ledEnd = 2;
int redMax  = 150; //values used to write to strip must be between 0 and 255
int blueMax = 125; //values used to write to strip must be between 0 and 255
            //end of led strip variables/setup
            
            //magnet variables/setup
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
}            //end of magnet variables/setup
            
            // Bluetooth variables/setupbles
String inData;
String token;

void setup() {
	//setup for magnet, magnet  is by default ON
	Serial.begin(9600);	    // Debug Window
	Serial1.begin(9600);	  // Bluetooth Communication - tx1,rx1
	pinMode(magnetPin, OUTPUT);
  pinMode(armedMagnetPin,OUTPUT);
	turnOnMagnetBox();
  
	analogWrite(ledBluePin, 0);
	analogWrite(ledRedPin, redMax);
	analogWrite(ledGreenPin, 0);
}

void loop() {
  
  if (Serial1.available() > 0){
    inData = Serial1.readStringUntil('\n');
    //Serial.println("inData>" + inData + "<");
    while (inData.length() > 2) {
      token = inData.substring(0,3);
      inData = inData.substring(3,inData.length());
      //Serial.println("token>" + token + "<");
      //Serial.println("inData>" + inData + "<");
      //Serial.println("");
  
        if (token == "000") {                          // Reply to alive request DO NOT DELETE
          //Serial.println("");
          Serial1.println("!");    
     
       } if (token == "200") {                          // RED 
        //Serial.println("LED-RED");
        analogWrite(ledBluePin, 0);
        analogWrite(ledRedPin, redMax);
        analogWrite(ledGreenPin, 0);
        //Serial1.println("LED-RED");    
        
      } else if (token == "201"){                   // TURN ALL LIGHTS OFF
        //Serial.println("LED-OFF");
        analogWrite(ledBluePin, 0);
        analogWrite(ledRedPin, 0);
        analogWrite(ledGreenPin, 0);
        //Serial1.println("LED-OFF");
        
      }  else if (token == "202") {                 // BLUE
        //Serial.println("LED-BLUE");
        analogWrite(ledBluePin, 255);
        analogWrite(ledRedPin, 0);
        analogWrite(ledGreenPin, 0);
        //Serial1.println("LED-BLUE");     
        
      } else if (token == "203"){                    // GREEN
        //Serial.println("LED-GREEN");
        analogWrite(ledBluePin, 0);
        analogWrite(ledRedPin, 0);
        analogWrite(ledGreenPin, 255);
        //Serial1.println("LED-GREEN");   

      } else if (token == "301"){                    // Magnet on
        //Serial.println("MAGNET-ON");
        turnOnMagnetBox();
        //Serial1.println("MAGNET-ON");     
      
      } else if (token == "302"){                    // Magnet off
        //Serial.println("MAGNET-OFF");
        turnOffMagnetBox();
        //Serial1.println("MAGNET-OFF");    
      
       } else {
          //Serial.println("");
          Serial1.println("!");  
       }
    }
  }
  }
