/*
  CETALIB "board" Library Example: "board_led_pattern.ino"

  This example demonstrates the usage of the "board->led_pattern()" method.

  Hardware Configuration:

  CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
  (Select Board: "Raspberry Pi Pico W")
  USER LED is a regular RED LED
  
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select Board: "SparkFun XRP Controller")
  USER LED is a WS2812B NeoPixel driven with fixed color (RED)

  created 17 May 2025
  by dBm Signal Dynamics Inc.

*/

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

int ledPattern = 1;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println();
  myRobot->board->initialize();
  myRobot->board->led_pattern(ledPattern);
  Serial.print("Led pattern: ");
  Serial.println(ledPattern);
}

// the loop function runs over and over again forever
void loop() {
  myRobot->board->tasks();
  if(myRobot->board->is_button_pressed())
  {
    ledPattern++;
    if(ledPattern > 5)
    {
      ledPattern = 1;
    }
    myRobot->board->led_pattern(ledPattern);
    Serial.print("Led pattern: ");
    Serial.println(ledPattern);
  }                          
}