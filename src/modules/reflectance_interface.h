/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            reflectance_interface.h
 * Project:         
 * Date:            June 26, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * "reflectance" sensors driver interface file - defines "REFLECTANCE_INTERFACE" structure
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH 
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
  float (*get_middle_sensor)(void);           // Sample middle opto reading
  float (*get_right_sensor)(void);            // Sample right opto reading
  int (*get_line_status)(void);               // Sample/Return current line detection status
  void (*clear_calibration)(void);            // Delete calibration data
};

/*** Public Function Prototypes ***********************************************/


#endif /* REFLECTANCE_INTERFACE_H_ */