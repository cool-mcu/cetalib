/*
  CETALIB "board" Library Example: "board_led_set_color.ino"

  This example demonstrates the usage of the "board->led_set_color()" method.

  Hardware Configuration:
  
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select Board: "SparkFun XRP Controller")
  USER LED is a WS2812B NeoPixel driven with an adjustable color palette:

  Color Palette: WHITE, RED, GREEN, BLUE, CYAN, YELLOW, PURPLE

  created 03 June 2026
  by dBm Signal Dynamics Inc.

*/

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

RGB_LED_COLOR ledColor = PURPLE; // Color Palette: WHITE, RED, GREEN, BLUE, CYAN, YELLOW, PURPLE

// the setup function runs once when you press reset or power the board
void setup() {
  myRobot->board->initialize();
  myRobot->board->led_set_color(ledColor);
}

// the loop function runs over and over again forever
void loop() {
  myRobot->board->led_on();
  delay(500);
  myRobot->board->led_off();
  delay(500);                   
}