/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            servoarm_interface.h
 * Project:         
 * Date:            June 28, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * "servoarm" driver interface file - defines "SERVOARM_INTERFACE" structure
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069B), based on RPI-Pico-WH 
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