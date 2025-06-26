/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            cetalib.h
 * Project:         
 * Date:            Mar 29, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib global driver interface
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

 #ifndef CETALIB_H_
 #define CETALIB_H_
 
 /*** Include Files ************************************************************/
 #include <Arduino.h>
 #include "cetalib_interface.h"
 
 /*** Macros *******************************************************************/
 #define SERIAL_PORT Serial  // Default to Serial
 #if defined(NO_USB)
    #undef SERIAL_PORT
    #define SERIAL_PORT Serial1     // Use Serial1 if USB is disabled
 #endif
 /*** Custom Data Types ********************************************************/
 
 
 /*** Public Function Prototypes ***********************************************/
 
 // CETALIB function interface object defined in cetalib.cpp
 extern const struct CETALIB_INTERFACE CETALIB;
 
 #endif /* CETALIB_H_ */
 