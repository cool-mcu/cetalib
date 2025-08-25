/*
  CETALIB "board" Library Example: "board_led_blink.ino"

  This example demonstrates the usage of the "board->led_blink()" method.

  Hardware Configuration:

  CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
  (Select Board: "Raspberry Pi Pico W")
  USER LED is a regular RED LED
  
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select Board: "SparkFun XRP Controller")
  USER LED is a WS2812B NeoPixel driven with fixed color (RED)

  Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
  (Select "Board = SparkFun XRP Controller (Beta)")
  USER LED is the built-in LED on the Pico W board

  created 18 Aug 2025
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