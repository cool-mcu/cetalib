/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            diffDrive_interface.h
 * Project:         
 * Date:            July 22, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * "differential drive" driver interface file - defines "DIFFDRIVE_INTERFACE" structure
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH 
 *
 */

#ifndef DIFFDRIVE_INTERFACE_H_
#define DIFFDRIVE_INTERFACE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>

/*** Macros *******************************************************************/

/*** Custom Data Types ********************************************************/
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

/*** Public Function Prototypes ***********************************************/


#endif /* DIFFDRIVE_INTERFACE_H_ */