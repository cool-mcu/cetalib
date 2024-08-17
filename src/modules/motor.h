/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            motor.h
 * Project:         
 * Date:            June 25, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * cetalib "motor" driver interface functions
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH 
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
void motor_set_left_effort(float effort);
void motor_set_right_effort(float effort);
void motor_set_efforts(float leftEffort, float rightEffort);

#endif /* MOTOR_H_ */