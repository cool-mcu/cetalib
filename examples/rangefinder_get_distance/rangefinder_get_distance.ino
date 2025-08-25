/*
  CETALIB "rangefinder" Library Example: "rangefinder_get_distance.ino"

  This example demonstrates the usage of the "rangefinder->get_distance()" method.

  Hardware Configurations:
 
  CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
  (Select "Board = Raspberry Pi Pico W")
 
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select "Board = SparkFun XRP Controller")

  Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
  (Select "Board = SparkFun XRP Controller (Beta)")

  created 18 Aug 2025
  by dBm Signal Dynamics Inc.

*/

#include <stdio.h>
#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define an output print buffer
char outBuffer[256];

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println();
  myRobot->rangefinder->initialize();
}

// the loop function runs over and over again forever
void loop() {
  sprintf(outBuffer, "distance to target (cm): %3.1f\r\n", myRobot->rangefinder->get_distance());
  Serial.print(outBuffer);
  delay(100);   
}