/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            motor.h
 * Project:         
 * Date:            Mar 29, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "motor" driver interface file - defines "MOTOR_INTERFACE" structure
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

#ifndef MOTOR_H_
#define MOTOR_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "motor_interface.h"

/*** Macros *******************************************************************/
#define LEFT_MOTOR_PWM_PIN 4    // OUTPUT - connected to motor controller "S2" input ("Left" Motor)
#define RIGHT_MOTOR_PWM_PIN 5   // OUTPUT - connected to motor controller "S1" input ("Right" Motor)
#define SERVO_RESOLUTION  90    // 90 possible adjustment steps in servo control signal

/*** Custom Data Types ********************************************************/


/*** Public Function Prototypes ***********************************************/
void motor_init(bool left_flip_dir, bool right_flip_dir);  // Initiallize pins & state variables
void motor_set_left_effort(float leftMotorEffort);
void motor_set_right_effort(float rightMotorEffort);
void motor_set_efforts(float leftMotorEffort, float rightMotorEffort);

#endif /* MOTOR_H_ */