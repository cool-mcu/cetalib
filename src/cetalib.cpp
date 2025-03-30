/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            cetalib.cpp
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

/** Include Files *************************************************************/
#include <Arduino.h>            // Required for Arduino functions
#include "cetalib.h"            // "cetalib" API declarations

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
extern const struct BOARD_INTERFACE BOARD;
//extern const struct MOTOR_INTERFACE MOTOR;
extern const struct REFLECTANCE_INTERFACE REFLECTANCE;
extern const struct SERVOARM_INTERFACE SERVOARM;
extern const struct RANGEFINDER_INTERFACE RANGEFINDER;
extern const struct CETA_IMU_INTERFACE CETA_IMU_IF;
extern const struct MQTTC_INTERFACE MQTTC;
extern const struct DIFFDRIVE_INTERFACE DIFFDRIVE;
extern const struct OLED_INTERFACE OLED;

extern const struct CETALIB_INTERFACE CETALIB = {
  .board = &BOARD,
  //.motor = &MOTOR,
  .reflectance = &REFLECTANCE,
  .servoarm = &SERVOARM,
  .rangefinder = &RANGEFINDER,
  .imu = &CETA_IMU_IF,
  .mqttc = &MQTTC,
  .diffDrive = &DIFFDRIVE,
  .oled = &OLED
};
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
extern const struct BOARD_INTERFACE BOARD;
extern const struct OLED_INTERFACE OLED;

extern const struct CETALIB_INTERFACE CETALIB = {
  .board = &BOARD,
  .oled = &OLED
};
#else
  #error Unsupported board selection
#endif

/*** Type Declarations ********************************************************/


/*** Private Function Prototypes **********************************************/

/*** Public Function Definitions **********************************************/

