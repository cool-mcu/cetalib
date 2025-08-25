/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            servoarm_interface.h
 * Project:         
 * Date:            Aug 18, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "servoarm" driver interface file - defines "SERVOARM_INTERFACE" structure
 *
 * Hardware Configurations Supported:
 * 
 * CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
 * (Select "Board = Raspberry Pi Pico W")
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select "Board = SparkFun XRP Controller")
 *
 * Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
 * (Select "Board = SparkFun XRP Controller (Beta)")
 *
 */

#ifndef SERVOARM_INTERFACE_H_
#define SERVOARM_INTERFACE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>

/*** Macros *******************************************************************/

/*** Custom Data Types ********************************************************/
struct SERVOARM_INTERFACE
{
  void (*initialize)(void);             // Initiallize pins, angle & state variables, calibrate set positions
  void (*set_angle)(int angle);         // Set servo angle (0 to 180 degrees)
  int (*get_angle)(void);               // Retrieve most recent angle setting
  void (*home)(void);                   // Set servo to "home" position (requires calibration)
  void (*lift)(void);                   // Set servo to "lift" position (requires calibration)
  void (*drop)(void);                   // Set servo to "drop" position (requires calibration)
  void (*clear_calibration)(void);      // Delete calibration data
};

/*** Public Function Prototypes ***********************************************/


#endif /* SERVOARM_INTERFACE_H_ */