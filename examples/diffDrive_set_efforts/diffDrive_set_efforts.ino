/*
  CETALIB "diffDrive" Library Example: "diffDrive_set_efforts.ino"

  This example uses the "diffDrive->set_efforts()" API to cycle the motors through some basic
  robot drive states when the user push button is pressed

  Hardware Configuration:

  CETA IoT Robot (schematic #14-00069A/B) based on RPI-Pico-WH.

  created 23 July 2024
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
  myRobot->diffDrive->initialize(false, false);
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
        myRobot->diffDrive->set_efforts(left_effort, right_effort);
        sprintf(outBuffer, "robotState: FORWARD\r\n");
        Serial.print(outBuffer);
        robotState = FWD;
        break;

      case FWD:
        myRobot->diffDrive->stop();
        sprintf(outBuffer, "robotState: STOP2\r\n");
        Serial.print(outBuffer);
        robotState = STOP2;
        break;

      case STOP2:
        myRobot->diffDrive->set_efforts(-left_effort, -right_effort);
        sprintf(outBuffer, "robotState: REVERSE\r\n");
        Serial.print(outBuffer);
        robotState = REV;
        break;

      case REV:
        myRobot->diffDrive->stop();
        sprintf(outBuffer, "robotState: STOP3\r\n");
        Serial.print(outBuffer);
        robotState = STOP3;
        break;

      case STOP3:
        myRobot->diffDrive->set_efforts(-left_effort, right_effort);
        sprintf(outBuffer, "robotState: ROTATE_LEFT\r\n");
        Serial.print(outBuffer);
        robotState = ROTATE_LEFT;
        break;

      case ROTATE_LEFT:
        myRobot->diffDrive->stop();
        sprintf(outBuffer, "robotState: STOP4\r\n");
        Serial.print(outBuffer);
        robotState = STOP4;
        break;

      case STOP4:
        myRobot->diffDrive->set_efforts(left_effort, -right_effort);
        sprintf(outBuffer, "robotState: ROTATE_RIGHT\r\n");
        Serial.print(outBuffer);
        robotState = ROTATE_RIGHT;
        break;

      case ROTATE_RIGHT:
        myRobot->diffDrive->stop();
        sprintf(outBuffer, "robotState: STOP1\r\n");
        Serial.print(outBuffer);
        robotState = STOP1;
        break;
    }   
  }
            
}