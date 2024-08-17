/*
  CETALIB "board" Library Example: "rangefinder_get_distance.ino"

  This example demonstrates the usage of the "rangefinder->get_distance()" method.

  Hardware Configuration:

  CETA IoT Robot (schematic #14-00069A/B) based on RPI-Pico-WH, with HC-SR04
  Ultrasonic sensor connected to GP17 (TRIG) and GP16 (ECHO).

  created 25 June 2024
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
  sprintf(outBuffer, "distance to target: %3.1f\r\n", myRobot->rangefinder->get_distance());
  Serial.print(outBuffer);
  delay(100);   
}