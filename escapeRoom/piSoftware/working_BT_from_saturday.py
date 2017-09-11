#Feel free to rename this file
void getBT(){
  
}
void setup() {
  // put your setup code here, to run once:

Serial.begin(9600); //testing
Serial1.begin(9600);  //bluetooth - tx1,rx1
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Starting...");
  if  (Serial1.available() ){
    Serial.println("Printing bluetooth:");
    //char garbage = Serial1.read();
    while(Serial1.available() ){
      for (int i = 0; i < 10 ; i ++){
        char newChar = (char)Serial1.read() ;
        Serial.print( newChar );
      }
  }
}
/*else{
  Serial1.write("hello");
}
*/

 //Serial.print("FINISH");
}