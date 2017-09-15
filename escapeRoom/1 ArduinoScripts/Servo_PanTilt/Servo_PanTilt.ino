/*        Program runs the Pan-Tilt servo setup
 * 
 *    -take the script, plug and play
 *
 *    Connections :  red/middle wires -- connect to 5V pins
 *                    brown/outside wires --- connect to ground
 *                   tan/outside wires --- connect to appropriate PWM pin 
 *                      - top servo is connected to pin 8 (PWM pin)
 *                      - bottom servo is connected to pin 9(PWM pin)
 */
#include <Servo.h>



int topServoPin = 8;      //PWM pin top servo is connected to
int bottomServoPin = 9;   //PWM pin bottom servo is connected to
Servo topServo;
Servo bottomServo;

int pos = 0;   //corresponds to degrees of motion
void setup() {
  // put your setup code here, to run once:
  topServo.attach(topServoPin);
  bottomServo.attach(bottomServoPin);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    topServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    topServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(1000);
/*
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    bottomServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    bottomServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(1000);
  */
}
