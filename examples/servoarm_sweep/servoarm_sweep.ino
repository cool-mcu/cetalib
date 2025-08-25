/*
  CETALIB "servoarm" Library Example: "servoarm_sweep.ino"

  This example sweeps the servo arm across it's full range of motion (0-180 degrees) and back.
  Can be used to verify mounting position of the lift arm

  Hardware Configurations Supported:
 
  CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
  (Select Board: "Raspberry Pi Pico W")
  Uses SG92R type servo connected to GP22
  
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select Board: "SparkFun XRP Controller")
  Uses SG92R type servo connected to GP6
 
  Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
  (Select "Board = SparkFun XRP Controller (Beta)")
  Uses SG92R type servo connected to GP16

  created 18 Aug 2025
  by dBm Signal Dynamics Inc.

*/

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// servo angle
int angle;

void setup() {
  myRobot->servoarm->initialize();
}

void loop() {
  for (angle = 0; angle <= 180; angle += 1) {       // goes from 0 degrees to 180 degrees in steps of 1 degree
    myRobot->servoarm->set_angle(angle);            // tell servo to go to position in variable 'angle'
    delay(15);                                      // waits 15ms for the servo to reach the position
  }
  for (angle = 180; angle >= 0; angle -= 1) {       // goes from 180 degrees to 0 degrees
    myRobot->servoarm->set_angle(angle);            // tell servo to go to position in variable 'angle'
    delay(15);                                      // waits 15ms for the servo to reach the position
  }
}