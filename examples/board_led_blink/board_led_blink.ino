/*
  CETALIB "board" Library Example: "board_led_blink.ino"

  This example demonstrates the usage of the "board->led_blink()" method.

  Hardware Configuration:

  CETA IoT Robot (schematic #14-00069A/B) based on RPI-Pico-WH, with USER LED
  connected to pin GP14.

  created 24 June 2024
  by dBm Signal Dynamics Inc.

*/

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

#define LED_BLINK_FREQUENCY 1

// the setup function runs once when you press reset or power the board
void setup() {
  myRobot->board->initialize();
  myRobot->board->led_blink(LED_BLINK_FREQUENCY);
}

// the loop function runs over and over again forever
void loop() {
  myRobot->board->tasks();                     
}