/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            cetalib.cpp
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

/** Include Files *************************************************************/
#include <Arduino.h>            // Required for Arduino functions
#include "cetalib.h"            // "cetalib" API declarations

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/
extern const struct BOARD_INTERFACE BOARD;
//extern const struct MOTOR_INTERFACE MOTOR;
extern const struct REFLECTANCE_INTERFACE REFLECTANCE;
extern const struct SERVOARM_INTERFACE SERVOARM;
extern const struct RANGEFINDER_INTERFACE RANGEFINDER;
extern const struct CETA_IMU_INTERFACE CETA_IMU_IF;
extern const struct MQTTC_INTERFACE MQTTC;
extern const struct DIFFDRIVE_INTERFACE DIFFDRIVE;

extern const struct CETALIB_INTERFACE CETALIB = {
  .board = &BOARD,
  //.motor = &MOTOR,
  .reflectance = &REFLECTANCE,
  .servoarm = &SERVOARM,
  .rangefinder = &RANGEFINDER,
  .imu = &CETA_IMU_IF,
  .mqttc = &MQTTC,
  .diffDrive = &DIFFDRIVE
};

/*** Type Declarations ********************************************************/


/*** Private Function Prototypes **********************************************/

/*** Public Function Definitions **********************************************/

