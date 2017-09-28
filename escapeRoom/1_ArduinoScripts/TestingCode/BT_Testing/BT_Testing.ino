
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
      Serial.println("ELEVEN from cube");
      Serial1.println("ELEVEN from cub");
     } else if (str == "!"){
      Serial.println("REQUEST from cub");
      Serial1.println("REQUEST from cub");
     } else {
      Serial1.println("from cube" + str);
     }
 }
}
