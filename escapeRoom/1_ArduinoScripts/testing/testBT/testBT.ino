
void getBT(){
  
}
void setup() {
  // put your setup code here, to run once:

Serial.begin(9600); //testing
Serial1.begin(9600);  //bluetooth - tx1,rx1
Serial1.print ("START_ARDUINO>");
Serial.print ("START_ARDUINO>");
Serial.println(Serial1.read());
}
String inData;
String str;
void loop() {
  
 if(Serial1.available() > 0) {
     str = Serial1.readStringUntil('\n');
     Serial.println(">" + str + "<");
     Serial1.println(str+"PI");
 }
 
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
