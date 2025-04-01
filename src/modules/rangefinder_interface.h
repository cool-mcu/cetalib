/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            rangefinder_interface.h
 * Project:         
 * Date:            Mar 29, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "rangefinder" driver interface file - defines "RANGEFINDER_INTERFACE" structure
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