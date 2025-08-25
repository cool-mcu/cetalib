/*
  CETALIB "oled" Library Example: "oled_print_switch_pot.ino"

  This example demonstrates usage of the "oled" methods to display the
  user switch state ("pressed" or "released") as well as the potentiometer
  value in the upper left corner of the display.

  Hardware Configuration:

  CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
  (Select "Board = Raspberry Pi Pico W")
  (Uses "Wire1" instance (I2C1 on pins SDA/GP18 & SCL/GP19))
  
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select "Board = SparkFun XRP Controller")
  (Uses "Wire" instance (I2C0 on pins SDA/GP04 & SCL/GP05))

  created 18 Aug 2025
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
    myRobot->board->led_blink(10);
    while (1)
    {
      myRobot->board->tasks();
    }
  }
}

// the loop function runs over and over again forever
void loop() {
  #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  potValue = myRobot->board->get_potentiometer();
  #else
  potValue = random(0, 4096);
  #endif
  switchLevel = myRobot->board->get_button_level();
  if (0 == switchLevel)
  {
    sprintf(oledOutBuffer, "Switch: Pressed \nPotentiometer: %4d\n", potValue);
    myRobot->oled->print(oledOutBuffer);
    myRobot->oled->home();
  }
  else
  {
    sprintf(oledOutBuffer, "Switch: Released\nPotentiometer: %4d\n", potValue);
    myRobot->oled->print(oledOutBuffer);
    myRobot->oled->home();
  }
  delay(100);
}