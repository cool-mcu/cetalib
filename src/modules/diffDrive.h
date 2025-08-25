/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            diffDrive.h
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

#ifndef DIFFDRIVE_H_
#define DIFFDRIVE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "diffDrive_interface.h"

/*** Macros *******************************************************************/

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  #define LEFT_RIGHT_COMPENSATION_DEFAULT 1.0f
  #define DIFFDRIVE_CAL_EEPROM_ADDRESS_START 385   // EEPROM Start address for calibration data
  #define DIFFDRIVE_CAL_EEPROM_ADDRESS_END   511   // EEPROM End address for calibration data
#endif

/*** Custom Data Types ********************************************************/

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  enum DIFFDRIVE_CALIBRATION_STATE {DIFFDRIVE_CAL_WAIT_BEGIN=0, DIFFDRIVE_CAL_L_R_COMP, DIFFDRIVE_CAL_IDLE};
#endif

/*** Public Function Prototypes ***********************************************/

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
void diffDrive_init(bool left_flip_dir, bool right_flip_dir);       // Initiallize pins & state variables
void diffDrive_set_efforts(float leftEffort, float rightEffort);    // Set both motor efforts
void diffDrive_stop(void);                                          // Stop both motors
void diffDrive_straight(float straightEffort);                      // Set identical motor efforts and apply compensation for straight motion
void diffDrive_turn(float turnDegrees, float turnEffort);           // Point-Turn the robot some relative heading, then exit when the heading is reached
void diffDrive_clear_calibration(void);                             // Delete EEPROM calibration data
void diffDrive_save_straight_compensation(float leftRightComp);     // Save straight speed calibration value in EEPROM
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
void diffDrive_init(bool left_flip_dir, bool right_flip_dir);       // Initiallize pins & state variables
void diffDrive_set_efforts(float leftEffort, float rightEffort);    // Set both motor efforts
void diffDrive_stop(void);                                          // Stop both motors
#else
   #error Unsupported board selection
#endif


#endif /* DIFFDRIVE_H_ */