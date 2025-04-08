/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            encoder_interface.h
 * Project:         
 * Date:            Mar 29, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "encoder" driver interface file - defines "ENCODER_INTERFACE" structure
 *
 * Hardware Configurations Supported:
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select "Board = SparkFun XRP Controller")
 *
 */

 #ifndef ENCODER_INTERFACE_H_
 #define ENCODER_INTERFACE_H_
 
 /*** Include Files ************************************************************/
 #include <Arduino.h>
 
 /*** Macros *******************************************************************/
 
 /*** Custom Data Types ********************************************************/
 struct ENCODER_INTERFACE
 {
   void   (*initialize)(void);                // Initiallize pins & state variables
   float  (*get_left_position)(void);         // Return the position of the left encoded motor (in revolutions) since last reset
   float  (*get_right_position)(void);        // Return the position of the right encoded motor (in revolutions) since last reset
   int    (*get_left_position_counts)(void);  // Return the raw encoder count of the left encoder since last reset 
   int    (*get_right_position_counts)(void); // Return the raw encoder count of the right encoder since last reset
   void   (*reset_left_position)(void);       // Reset left encoder count
   void   (*reset_right_position)(void);      // Reset right encoder count
 };
 
 /*** Public Function Prototypes ***********************************************/
 
 
 #endif /* ENCODER_INTERFACE_H_ */