/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            motor_interface.h
 * Project:         
 * Date:            June 25, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * "motor" driver interface file - defines "MOTOR_INTERFACE" structure
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH 
 *
 */

#ifndef MOTOR_INTERFACE_H_
#define MOTOR_INTERFACE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>

/*** Macros *******************************************************************/

/*** Custom Data Types ********************************************************/
struct MOTOR_INTERFACE
{
  void (*initialize)(bool left_flip_dir, bool right_flip_dir); // Initiallize pins & state variables
  void (*set_left_effort)(float effort);      // Set left motor effort
  void (*set_right_effort)(float effort);     // Set right motor effort
  void (*set_efforts)(float leftEffort, float rightEffort);     // Set both motor efforts
  // bool (*are_motors_powered)(void);  // Checks if battery power is applied to the robot
};

/*** Public Function Prototypes ***********************************************/


#endif /* MOTOR_INTERFACE_H_ */