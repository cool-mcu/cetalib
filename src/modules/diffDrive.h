/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            diffDrive.h
 * Project:         
 * Date:            July 22, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * cetalib "differential drive" driver interface functions
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH 
 *
 */

#ifndef DIFFDRIVE_H_
#define DIFFDRIVE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "diffDrive_interface.h"

/*** Macros *******************************************************************/
#define LEFT_RIGHT_COMPENSATION_DEFAULT 1.0f
#define DIFFDRIVE_CAL_EEPROM_ADDRESS_START 385   // EEPROM Start address for calibration data
#define DIFFDRIVE_CAL_EEPROM_ADDRESS_END   511   // EEPROM End address for calibration data

/*** Custom Data Types ********************************************************/

enum DIFFDRIVE_CALIBRATION_STATE {DIFFDRIVE_CAL_WAIT_BEGIN=0, DIFFDRIVE_CAL_L_R_COMP, DIFFDRIVE_CAL_IDLE};

/*** Public Function Prototypes ***********************************************/
void diffDrive_init(bool left_flip_dir, bool right_flip_dir);       // Initiallize pins & state variables
void diffDrive_set_efforts(float leftEffort, float rightEffort);    // Set both motor efforts
void diffDrive_stop(void);                                          // Stop both motors
void diffDrive_straight(float straightEffort);                      // Set identical motor efforts and apply compensation for straight motion
void diffDrive_turn(float turn_degrees, float turnEffort, int timeoutMs);  // Point-Turn the robot some relative heading, then exit when the heading is reached
void diffDrive_clear_calibration(void);                             // Delete EEPROM calibration data
void diffDrive_save_straight_compensation(float leftRightComp);     // Save straight speed calibration value in EEPROM
#endif /* DIFFDRIVE_H_ */