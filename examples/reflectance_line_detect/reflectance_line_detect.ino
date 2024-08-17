/*
  CETALIB "board" Library Example: "reflectance_line_detect.ino"

  This example demonstrates the usage of the "reflectance->get_line_status()" method.

  It also demonstrates how to trigger a calibration sequence to compute/save the
  optimal line detection trip threshbold for the reflectance sensors.

  Hardware Configuration:

  CETA IoT Robot (schematic #14-00069A/B) based on RPI-Pico-WH, with reflectance
  sensors connected to GP26/ADC0, GP27/ADC1 and GP28/ADC2.

  created 29 June 2024
  by dBm Signal Dynamics Inc.

*/

#include <stdio.h>
#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define an output print buffer
char outBuffer[256];

// sensor readings
int line_detect;
int left_detect, middle_detect, right_detect;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println();
  myRobot->board->initialize();
  if(0 == myRobot->board->get_button_level())
  {
    while(0 == myRobot->board->get_button_level());
    myRobot->reflectance->clear_calibration();
  }
  myRobot->reflectance->initialize();
}

// the loop function runs over and over again forever
void loop() {
  line_detect = myRobot->reflectance->get_line_status();
  if(line_detect & 4)
  {
    left_detect = 1;
  }
  else
  {
    left_detect = 0;
  }
  if(line_detect & 2)
  {
    middle_detect = 1;
  }
  else
  {
    middle_detect = 0;
  }
  if(line_detect & 1)
  {
    right_detect = 1;
  }
  else
  {
    right_detect = 0;
  }
  sprintf(outBuffer, "left: %4.3f, left_det: %d, middle: %4.3f, middle_det: %d, right: %4.3f, right_det: %d\r\n", myRobot->reflectance->get_left_sensor(), left_detect, myRobot->reflectance->get_middle_sensor(), middle_detect, myRobot->reflectance->get_right_sensor(), right_detect);
  Serial.print(outBuffer);
  delay(1000);
}