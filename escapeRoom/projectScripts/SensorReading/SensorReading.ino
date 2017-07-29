/*
  SensorReading.ino ---- meant to test values that can be retrieved from solar cell
*/
// sensor is plugged into A1
int sensorPin = A1;
int sensorValue = 0;
int sensorValues[10];
int index = 0;


// <sensor value>:<Actual voltage>, 204.6 adc value : 1 V 
//int sensorResolution = 205;
int maxVoltPerStep = 1;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   // init computer screen ready to recieve info from Mega
  analogReference(DEFAULT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int whileBoolean = 0;
  maxVoltPerStep = 0;
  sensorValue = 0;
  sensorValue = analogRead(sensorPin);    // value between 0 and 1024 (volts)
  double voltage = (sensorValue / 1024.0) * 5;
  if ( (voltage > 0) && (voltage < 5) ){
    while( whileBoolean == 0){      //locks up the program until the range is found.
       
      if (voltage < maxVoltPerStep  ){
        //sensor is less than current maxVoltPerStep but greater than previous maxVoltPerStep
        whileBoolean = 1;
        Serial.print("Sensor value is between ");
        Serial.print(maxVoltPerStep-1);
        Serial.print(" and ");
        Serial.print(maxVoltPerStep);
        Serial.print(" \r\n ");


        Serial.print("Sensor Voltage is :");
        Serial.print(voltage);
        Serial.print(" \r\n ");

        Serial.print("Sensor Value is :");
        Serial.print(sensorValue);
        Serial.print(" \r\n ");

        if (index == 10){
          index = 0;
          double sum = 0;
          for (int i = 0; i < 10; i++){
            sum += sensorValues[i];
          }
          Serial.print("Sensor voltages averaged is :");
          Serial.print(sum/10);
          Serial.print(" \r\n ");
        }
        sensorValues[index] = voltage;
        index += 1;
      }

      maxVoltPerStep += 1;
    }     //end of while loop
  }
  else{
    Serial.println("Did not correctly recieve a value.");
  }
  delay(2000); //delay for 2 seconds between readings
}
