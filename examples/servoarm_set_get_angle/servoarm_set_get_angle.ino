/*
  CETALIB "servoarm" Library Example: "servoarm_set_get_angle.ino"

  This example shows usage of the servoarm "set_angle()" and "get_angle()" methods.

  Hardware Configurations Supported:
 
  CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
  (Select Board: "Raspberry Pi Pico W")
  Uses SG92R type servo connected to GP22

  created 18 Aug 2025
  by dBm Signal Dynamics Inc.

*/

#include <stdio.h>
#include <cetalib.h>

// define pin for potentiometer input
#define POT_PIN A2

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define an output print buffer
char outBuffer[128];

// servo angle
int angle;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println();
  myRobot->board->initialize();
  myRobot->servoarm->initialize();
  analogReadResolution(12);
  delay(1000); // delay to let servo settle to initial position
}

// the loop function runs over and over again forever
void loop() {
  myRobot->board->tasks();
  if(myRobot->board->is_button_pressed())
  {
    Serial.print("Current angle: ");
    Serial.println(myRobot->servoarm->get_angle());
  }
  angle = (int)map(analogRead(POT_PIN), 0, 4096, 0, 180);
  myRobot->servoarm->set_angle(angle);
  sprintf(outBuffer, "Set angle: %d\r\n", angle);
  Serial.print(outBuffer);
  delay(100);
}