/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            cetalib.h
 * Project:         
 * Date:            June 24, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * cetalib global driver interface
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH 
 *
 */

#ifndef CETALIB_H_
#define CETALIB_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "cetalib_interface.h"

/*** Macros *******************************************************************/


/*** Custom Data Types ********************************************************/


/*** Public Function Prototypes ***********************************************/

// CETALIB function interface object defined in cetalib.cpp
extern const struct CETALIB_INTERFACE CETALIB;

#endif /* CETALIB_H_ */
