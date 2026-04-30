/*
  CETALIB "encoder" Library Example: "encoder_simple_speed.ino"

  This example demonstrates how to compute the robot's RIGHT wheel speed (in cm/s)
  using the "encoder->get_position_XXX_counts()" methods.
  
  Rotate the RIGHT wheel manually to simulate robot motion.

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

// variables used to track speed
float speed;
float currentDistance = 0.0f;
float previousDistance = 0.0f;
unsigned long currentSampleTime;
unsigned long previousSampleTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  myRobot->encoder->initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  currentSampleTime = millis();
  currentDistance = (3.14159 * 6.0) * (myRobot->encoder->get_right_position_counts()/585.0);
  // speed in cm/s = distance travelled in 1 second (1000 mS) !
  if((currentSampleTime - previousSampleTime) >= 1000)
  {
    previousSampleTime = currentSampleTime;
    speed = currentDistance - previousDistance;
    previousDistance = currentDistance;
    Serial.printf("Speed (cm/s): %.2f\r\n", speed);
  }
}

