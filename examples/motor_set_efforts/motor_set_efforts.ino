/*
  CETALIB "motor" Library Example: "motor_set_efforts.ino"

  This example uses the "motor->set_efforts()" API to cycle the motors through some basic
  robot drive states when the user push button is pressed

  Hardware Configurations Supported:
 
  CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
  (Select Board: "Raspberry Pi Pico W")
 
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select Board: "SparkFun XRP Controller")

  Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
  (Select "Board = SparkFun XRP Controller (Beta)")

  created 18 Aug 2025
  by dBm Signal Dynamics Inc.

*/

#include <Arduino.h>
#include <stdio.h>
#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define an output buffer for Serial.print()
char outBuffer[128];

// define the initial efforts
float left_effort = 0.2;
float right_effort = 0.2;

// define the robot states
enum ROBOT_STATE {STOP1, FWD, STOP2, REV, STOP3, ROTATE_LEFT, STOP4, ROTATE_RIGHT};

// declare & initialize a variable that holds the current state
enum ROBOT_STATE robotState = STOP1;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println();
  myRobot->board->initialize();
  myRobot->motor->initialize(true, true);
  Serial.println("robotState: STOP1");
}

// the loop function runs over and over again forever
void loop() {
  myRobot->board->tasks();
  if(myRobot->board->is_button_pressed())
  {
    switch(robotState)
    {
      case STOP1:
        myRobot->motor->set_efforts(left_effort, right_effort);
        sprintf(outBuffer, "robotState: FORWARD\r\n");
        Serial.print(outBuffer);
        robotState = FWD;
        break;

      case FWD:
        myRobot->motor->set_efforts(0.0f,0.0f);
        sprintf(outBuffer, "robotState: STOP2\r\n");
        Serial.print(outBuffer);
        robotState = STOP2;
        break;

      case STOP2:
        myRobot->motor->set_efforts(-left_effort, -right_effort);
        sprintf(outBuffer, "robotState: REVERSE\r\n");
        Serial.print(outBuffer);
        robotState = REV;
        break;

      case REV:
        myRobot->motor->set_efforts(0.0f,0.0f);
        sprintf(outBuffer, "robotState: STOP3\r\n");
        Serial.print(outBuffer);
        robotState = STOP3;
        break;

      case STOP3:
        myRobot->motor->set_efforts(-left_effort, right_effort);
        sprintf(outBuffer, "robotState: ROTATE_LEFT\r\n");
        Serial.print(outBuffer);
        robotState = ROTATE_LEFT;
        break;

      case ROTATE_LEFT:
        myRobot->motor->set_efforts(0.0f,0.0f);
        sprintf(outBuffer, "robotState: STOP4\r\n");
        Serial.print(outBuffer);
        robotState = STOP4;
        break;

      case STOP4:
        myRobot->motor->set_efforts(left_effort, -right_effort);
        sprintf(outBuffer, "robotState: ROTATE_RIGHT\r\n");
        Serial.print(outBuffer);
        robotState = ROTATE_RIGHT;
        break;

      case ROTATE_RIGHT:
        myRobot->motor->set_efforts(0.0f,0.0f);
        sprintf(outBuffer, "robotState: STOP1\r\n");
        Serial.print(outBuffer);
        robotState = STOP1;
        break;
    }   
  }
            
}