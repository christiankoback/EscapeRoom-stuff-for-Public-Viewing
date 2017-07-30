



/*
    lol_board.ino

Purpose :  to use the LoL board (9 x 14 LED board)

Pins : Mega pins - uses from RESET pin to A1-A5 pin, 
                    uses digital pins 13 to 0

*/

#include <Charliplexing.h>


byte pins[3] = {9, 10, 11};
Charlieplex charlie(pins, sizeof(pins));

void setup() {
  charlie.setHoldTime(1000000);
  charlie.setLed(0, true);    // Turn on LED1
  charlie.setLed(3, true);    // Turn on LED4
  charlie.setPwmLed(5, 64);   // Set LED6 to one-quarter on
  
}

void loop() {
  charlie.loop();
  
}
