/*
  CETALIB "oled" Library Example: "oled_print_switch_pot_scroll.ino"

  This example demonstrates usage of the "oled" methods to display the
  user switch state as well as the potentiometer value in a scrolling display
  similar to output on a serial terminal

  Hardware Configuration:

  CETA IoT Robot (schematic #14-00069B) based on RPI-Pico-WH, with Adafruit 
  128x64 OLED display (#938) connected to I2C1 pins (SDA/GP18)/(SCL/GP19)

  created 30 Aug 2024
  by dBm Signal Dynamics Inc.

*/

#include <cetalib.h>
#include <stdio.h>

// define an output print buffer for the OLED
char oledOutBuffer[64];

int potValue, switchLevel;

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// the setup function runs once when you press reset or power the board
void setup() {
  myRobot->board->initialize();
  if (!myRobot->oled->initialize())
  {
    Serial.println("Failed to initialize OLED!. Stopping.");
    myRobot->board->led_blink(10);
    while (1)
    {
      myRobot->board->tasks();
    }
  }
}

// the loop function runs over and over again forever
void loop() {
  potValue = myRobot->board->get_potentiometer();
  switchLevel = myRobot->board->get_button_level();
  sprintf(oledOutBuffer, "Switch: %d, Pot: %d", switchLevel, potValue);
  myRobot->oled->println(oledOutBuffer);
  delay(1000);
}