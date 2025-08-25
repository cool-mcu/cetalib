/*
  CETALIB "board" Library Example: "reflectance_get_sensors.ino"

  This example demonstrates the usage of the "reflectance->get_sensors()" method.

  Hardware Configuration:

  CETA IoT Robot (schematic #14-00069A/B) based on RPI-Pico-WH
  (Select Board: "Raspberry Pi Pico W")
  Reflectance sensors connected to GP26/ADC0, GP27/ADC1 and GP28/ADC2.

  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select Board: "SparkFun XRP Controller")
  Reflectance sensors connected to GP44/ADC4 and GP45/ADC5.

  Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
  (Select "Board = SparkFun XRP Controller (Beta)")
  Reflectance sensors connected to GP26/ADC1 and GP27/ADC2.

  created 10 Aug 2025
  by dBm Signal Dynamics Inc.

*/

#include <stdio.h>
#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define an output print buffer
char outBuffer[128];

// sensor readings
float left, middle, right;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println();
  myRobot->reflectance->initialize();
}

// the loop function runs over and over again forever
void loop() {
  left = myRobot->reflectance->get_left_sensor();
  middle = myRobot->reflectance->get_middle_sensor();
  right = myRobot->reflectance->get_right_sensor();
  sprintf(outBuffer, "left: %4.3f, middle: %4.3f, right: %4.3f\r\n", left, middle, right);
  Serial.print(outBuffer);
  delay(100);
}