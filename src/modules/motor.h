/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            motor.h
 * Project:         
 * Date:            Aug 18, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "motor" driver interface functions
 *
 * Hardware Configurations Supported:
 * 
 * CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
 * (Select Board: "Raspberry Pi Pico W")
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select Board: "SparkFun XRP Controller")
 * Left Motor connected to "Motor3" connector
 * Right Motor connected to "Motor4" connector 
 *
 * Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
 * (Select "Board = SparkFun XRP Controller (Beta)")
 * Left Motor connected to "MotorL" connector
 * Right Motor connected to "MotorR" connector
 */

#ifndef MOTOR_H_
#define MOTOR_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "motor_interface.h"

/*** Macros *******************************************************************/

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  #define LEFT_MOTOR_PWM_PIN 4          // OUTPUT - connected to motor controller "S2" input ("Left" Motor)
  #define RIGHT_MOTOR_PWM_PIN 5         // OUTPUT - connected to motor controller "S1" input ("Right" Motor)
  #define SERVO_RESOLUTION  90          // 90 possible adjustment steps in servo control signal
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
  #define LEFT_MOTOR_IN1_PIN        35      // Left Motor IN1 Pin:    Motor_L_AIN1
  #define LEFT_MOTOR_IN2_PIN        34      // Left Motor IN2 Pin:    Motor_L_AIN2
  #define RIGHT_MOTOR_IN1_PIN       32      // Right Motor IN1 Pin:   Motor_R_BIN1
  #define RIGHT_MOTOR_IN2_PIN       33      // Right Motor IN2 Pin:   Motor_R_BIN2 
  #define PWM_FREQ        500           // 500 Hz
  #define PWM_RESOLUTION  1000          // 1000 steps
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
  #define LEFT_MOTOR_DIR_PIN      6
  #define LEFT_MOTOR_SPEED_PIN    7
  #define RIGHT_MOTOR_DIR_PIN     14
  #define RIGHT_MOTOR_SPEED_PIN   15 
  #define PWM_FREQ        500           // 500 Hz
  #define PWM_RESOLUTION  1000          // 1000 steps
#else
  #error Unsupported board selection
#endif

/*** Custom Data Types ********************************************************/


/*** Public Function Prototypes ***********************************************/
void motor_init(bool left_flip_dir, bool right_flip_dir);  // Initiallize pins & state variables
void motor_set_left_effort(float leftMotorEffort);
void motor_set_right_effort(float rightMotorEffort);
void motor_set_efforts(float leftMotorEffort, float rightMotorEffort);

#endif /* MOTOR_H_ */