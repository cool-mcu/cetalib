/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            rangefinder_interface.h
 * Project:         
 * Date:            July 05, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * "rangefinder" driver interface file - defines "RANGEFINDER_INTERFACE" structure
 *
 * Hardware Configuration:
 * 
 * CETA IoT Robot (schematic #14-00069B), based on RPI-Pico-WH with HC-SR04P sensor from ABRA
 * (https://abra-electronics.com/sensors/sensors-proximity-en/hc-sr04p.html)
 */

#ifndef RANGEFINDER_INTERFACE_H_
#define RANGEFINDER_INTERFACE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>

/*** Macros *******************************************************************/

/*** Custom Data Types ********************************************************/
struct RANGEFINDER_INTERFACE
{
  void (*initialize)(void);                   // Initiallize pins, state variables
  float (*get_distance)(void);                // Measure distance to target (in cm)
};

/*** Public Function Prototypes ***********************************************/


#endif /* RANGEFINDER_INTERFACE_H_ */