/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            reflectance_interface.h
 * Project:         
 * Date:            May 18, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "reflectance" sensors driver interface file - defines "REFLECTANCE_INTERFACE" structure
 *
 * Hardware Configurations Supported:
 * 
 * CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
 * (Select "Board = Raspberry Pi Pico W")
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select "Board = SparkFun XRP Controller")
 *
 */

#ifndef REFLECTANCE_INTERFACE_H_
#define REFLECTANCE_INTERFACE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>

/*** Macros *******************************************************************/

/*** Custom Data Types ********************************************************/
struct REFLECTANCE_INTERFACE
{
  void (*initialize)(void);                   // Initiallize pins, state variables, calibrate if needed
  float (*get_left_sensor)(void);             // Sample left opto reading
  float (*get_middle_sensor)(void);           // Sample middle opto reading (returns "0.0" for XRP Robot)
  float (*get_right_sensor)(void);            // Sample right opto reading
  int (*get_line_status)(void);               // Sample/Return current line detection status
  void (*clear_calibration)(void);            // Delete calibration data
};

/*** Public Function Prototypes ***********************************************/


#endif /* REFLECTANCE_INTERFACE_H_ */