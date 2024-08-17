/*
  CETALIB "board" Library Example: "board_get_potentiometer.ino"

  This example demonstrates the usage of the "board->get_potentiometer()" method.

  Hardware Configuration:

  CETA IoT Robot (schematic #14-00069A/B) based on RPI-Pico-WH, with USER POT
  connected to pin GP28/ADC2.

  created 11 July 2024
  by dBm Signal Dynamics Inc.

*/

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println();
  myRobot->board->initialize();
}

// the loop function runs over and over again forever
void loop() {
  Serial.print("Potentiometer value: ");
  Serial.println(myRobot->board->get_potentiometer());
  delay(1000);                    
}