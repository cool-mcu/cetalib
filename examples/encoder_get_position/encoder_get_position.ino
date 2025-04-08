/*
  CETALIB "encoder" Library Example: "encoder_get_position.ino"

  This example demonstrates the usage of the "encoder->get_XXXX_position()" methods.

  Hardware Configuration:

  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select "Board = SparkFun XRP Controller")

  created 7 April 2025
  by dBm Signal Dynamics Inc.

*/

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  myRobot->board->initialize();
  myRobot->encoder->initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  myRobot->board->tasks();
  if(myRobot->board->is_button_pressed())
  {
    myRobot->encoder->reset_left_position();
    myRobot->encoder->reset_right_position();
  }
  Serial.print("Left Motor Position: ");
  Serial.print(myRobot->encoder->get_left_position());
  Serial.print(" Right Motor Position: ");
  Serial.println(myRobot->encoder->get_right_position());
  delay(100);
}