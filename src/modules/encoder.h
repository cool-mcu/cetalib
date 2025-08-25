/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            encoder.h
 * Project:         
 * Date:            Aug 18, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "encoder" driver interface functions
 * 
 * API based on XRPlib library APIs
 * https://open-stem.github.io/XRP_MicroPython/index.html
 *
 * Dependencies:
 * 
 * rp2040-encoder-library by Giovanni di Dio Bruno:
 * https://github.com/gbr1/rp2040-encoder-library
 * 
 * Hardware Configurations Supported:
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select "Board = SparkFun XRP Controller")
 *
 * Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
 * (Select "Board = SparkFun XRP Controller (Beta)")
 *
 */

#ifndef ENCODER_H_
#define ENCODER_H_
 
/*** Include Files ************************************************************/
#include <Arduino.h>
#include "encoder_interface.h"
 
/*** Macros *******************************************************************/
#if defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
#define LEFT_MOTOR_ENCODER_A_PIN  22
#define RIGHT_MOTOR_ENCODER_A_PIN 2
#define RESOLUTION 585.0  // Number of counts per wheel rotation  
                          // (12 counts/motor shaft revolution) * (48.75:1 gear ratio)
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
#define LEFT_MOTOR_ENCODER_A_PIN  4
#define RIGHT_MOTOR_ENCODER_A_PIN 12
#define RESOLUTION 585.0
#endif
 
/*** Custom Data Types ********************************************************/
 
 
/*** Public Function Prototypes ***********************************************/
void  encoder_init(void);                       // Initiallize pins & state variables
float encoder_get_left_position(void);          // Return the position of the left encoded motor (in revolutions) since last reset
float encoder_get_right_position(void);         // Return the position of the right encoded motor (in revolutions) since last reset
int   encoder_get_left_position_counts(void);   // Return the raw encoder count of the left encoder since last reset
int   encoder_get_right_position_counts(void);  // Return the raw encoder count of the right encoder since last reset 
void  encoder_reset_left_position(void);        // Reset left encoder count
void  encoder_reset_right_position(void);       // Reset right encoder count

#endif /* ENCODER_H_ */