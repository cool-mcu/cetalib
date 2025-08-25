/*
  CETALIB "servoarm" Library Example: "servoarm_lift_drop.ino"

  This example shows how to calibrate 3 fixed positions for the servoarm then
  cycle through the 3 positions when pressing the pushbutton. This task will be 
  needed for challenges where a bucket is transported by the robot.

  Hardware Configurations Supported:
 
  CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
  (Select Board: "Raspberry Pi Pico W")
  Uses SG92R type servo connected to GP22
  
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select Board: "SparkFun XRP Controller")
  Uses SG92R type servo connected to GP6
 
  Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
  (Select "Board = SparkFun XRP Controller (Beta)")
  Uses SG92R type servo connected to GP16

  created 18 Aug 2025
  by dBm Signal Dynamics Inc.

*/

#include <stdio.h>
#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define an output print buffer
char outBuffer[128];

// define some servoarm states
enum SERVOARM_POSITION {HOME=0, LIFT, DROP} servoPosition = HOME;

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
    myRobot->servoarm->clear_calibration();
  }
  myRobot->servoarm->initialize();
  myRobot->servoarm->home();
  Serial.println("ServoArm Position: HOME");
}

// the loop function runs over and over again forever
void loop() {
  myRobot->board->tasks();
  if(myRobot->board->is_button_pressed())
  {
    switch(servoPosition)
    {
      case HOME:
        servoPosition = LIFT;
        myRobot->servoarm->lift();
        Serial.println("ServoArm Position: LIFT");
        break;
      case LIFT:
        servoPosition = DROP;
        myRobot->servoarm->drop();
        Serial.println("ServoArm Position: DROP");
        break;
      case DROP:
        servoPosition = HOME;
        myRobot->servoarm->home();
        Serial.println("ServoArm Position: HOME");
        break;
      default:
        break;    
    }
  }
}