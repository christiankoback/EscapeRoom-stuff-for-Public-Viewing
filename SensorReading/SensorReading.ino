// sensor is plugged into A0
int sensorPin = A0;
int sensorValue = 0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  // <sensor value>:<Actual voltage>, 204.6 adc value : 1 V 
  sensorValue = analogRead(sensorPin);    // value between 0 and 1024 (volts)
  
}
