/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            diffDrive_interface.h
 * Project:         
 * Date:            Aug 18, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "differential drive" driver interface file - defines "DIFFDRIVE_INTERFACE" structure
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

#ifndef DIFFDRIVE_INTERFACE_H_
#define DIFFDRIVE_INTERFACE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>

/*** Macros *******************************************************************/

/*** Custom Data Types ********************************************************/
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
struct DIFFDRIVE_INTERFACE
{
  void (*initialize)(bool left_flip_dir, bool right_flip_dir);  // Initiallize pins & state variables
  void (*set_efforts)(float leftEffort, float rightEffort);     // Set both motor efforts
  void (*stop)(void);                                           // Stop both motors
  void (*straight)(float straightEffort);                       // Set identical motor efforts and apply compensation for straight motion
  void (*turn)(float turnDegrees, float turnEffort);            // Point-Turn the robot some relative heading, then exit when the heading is reached
  void (*clear_calibration)(void);                              // Delete EEPROM calibration data
  void (*save_straight_compensation)(float leftRightComp);      // Save straight speed calibration value in EEPROM
};
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
struct DIFFDRIVE_INTERFACE
{
  void (*initialize)(bool left_flip_dir, bool right_flip_dir);  // Initiallize pins & state variables
  void (*set_efforts)(float leftEffort, float rightEffort);     // Set both motor efforts
  void (*stop)(void);                                           // Stop both motors
};
#else
   #error Unsupported board selection
 #endif

/*** Public Function Prototypes ***********************************************/


#endif /* DIFFDRIVE_INTERFACE_H_ */