
void getBT(){
  
}
void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);   //Debug window
Serial1.begin(9600);  //Bluetooth - tx1,rx1
Serial.print("START_ARDUINO>");

}
String inData;
String str;
String text;
int i = 0;

void loop() {
  
 if(Serial1.available() > 0) {
     str = Serial1.readStringUntil('\n');
     Serial.println(">" + str + "<");
     if (str == "11"){
      Serial.println("ELEVEN");
      Serial1.println("ELEVEN");
     } else if (str == "!"){
      Serial.println("REQUEST");
      Serial1.println("REQUEST");
     } else {
      Serial1.println("!");
     }
 }

//delay(5000);








/*
  while (Serial1.available() > 0)
      {
          char recieved = Serial1.read();
          inData += recieved; 
  
          // Process message when new line character is recieved
          if (recieved == '\n')
          {
              Serial.print("Arduino Received: ");
              Serial.print(inData);
  
              inData = ""; // Clear recieved buffer
          }
      }


      */

      
/*
 * working
 
  // listen for the data
  while (Serial1.){
    
  }
  if ( Serial1.available() > 0 ) {
    // read a numbers from serial port
    int count = Serial1.parseInt();
    
     // print out the received number
    if (count > 0) {
        Serial.print("You have input: ");
        Serial.println(String(count));
        Serial1.print (String(count));

    }
  }


*/





  
  /*
  // put your main code here, to run repeatedly:
  //Serial.println("Starting...");
  if  (Serial1.available() > 0){
    Serial.println("Printing bluetooth:");
    //char garbage = Serial1.read();
    while(Serial1.available()){
        char newChar = (char)Serial1.read();
        Serial.print( newChar );
      
  }

  
}
*/
}
