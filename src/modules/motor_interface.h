/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            motor_interface.h
 * Project:         
 * Date:            Mar 29, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "motor" driver interface file - defines "MOTOR_INTERFACE" structure
 *
 * Hardware Configurations Supported:
 * 
 * CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
 * (Select "Board = Raspberry Pi Pico W")
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select "Board = SparkFun XRP Controller")
 * Left Motor connected to "Motor3" connector
 * Right Motor connected to "Motor4" connector 
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
  void (*initialize)(bool left_flip_dir, bool right_flip_dir);        // Initiallize pins & state variables
  void (*set_left_effort)(float leftMotorEffort);                     // Set left motor effort
  void (*set_right_effort)(float rightMotorEffort);                   // Set right motor effort
  void (*set_efforts)(float leftMotorEffort, float rightMotorEffort);  // Set both motor efforts
};

/*** Public Function Prototypes ***********************************************/


#endif /* MOTOR_INTERFACE_H_ */