/*
  CETALIB "encoder" Library Example: "encoder_simple_distance.ino"

  This example demonstrates how to compute distance travelled (in cm) using
  the "encoder->get_position_XXX_counts()" methods.
  
  Rotate the wheels manually to simulate robot motion (produces ~18.8 cm per wheel revolution)

  Hardware Configuration:

  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select "Board = SparkFun XRP Controller")

  Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
  (Select "Board = SparkFun XRP Controller (Beta)")

  created 29 April 2026
  by dBm Signal Dynamics Inc.

*/

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  myRobot->encoder->initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  float leftDistance = (3.14159 * 6.0) * (myRobot->encoder->get_left_position_counts()/585.0);
  float rightDistance = (3.14159 * 6.0) * (myRobot->encoder->get_right_position_counts()/585.0);
  Serial.printf("Left Distance: %.2f Right Distance: %.2f\r\n", leftDistance, rightDistance);
  delay(100);
}
