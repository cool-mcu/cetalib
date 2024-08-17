/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            motor.cpp
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

/** Include Files *************************************************************/
#include <Arduino.h>            // Required for Arduino functions
#include <Servo.h>              // Required for Servo functions
#include "motor.h"              // "motor" API declarations

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/
Servo lServo;             // LEFT motor servo object
Servo rServo;             // RIGHT motor servo object
int leftMotorDir, leftMotorDirFwd, rightMotorDir, rightMotorDirFwd;

/*** Type Declarations ********************************************************/
extern const struct MOTOR_INTERFACE MOTOR = {
    .initialize             = &motor_init,
    .set_left_effort        = &motor_set_left_effort,
    .set_right_effort       = &motor_set_right_effort,
    .set_efforts            = &motor_set_efforts
};

/*** Private Function Prototypes **********************************************/

/*** Public Function Definitions **********************************************/

void motor_init(bool left_flip_dir, bool right_flip_dir)
{
    // Initialize servo output signals
    // Servo Angle parameter vs Motor Torque/Direction:
    //
    //  0   = full speed in one direction,
    //  90  = no movement
    //  180 = full speed in the other direction
    lServo.attach(LEFT_MOTOR_PWM_PIN);                  // Initialize LEFT MOTOR PWM Digital Control Output
    lServo.write(90);                                   // Motor state = STOP
    rServo.attach(RIGHT_MOTOR_PWM_PIN);                 // Initialize RIGHT MOTOR PWM Digital Control Output
    rServo.write(90);                                   // Motor state = STOP
    
    // Initiallize Left Motor "forward" direction
    if(left_flip_dir == false)
    {
        leftMotorDir = 0;   // default motor direction setting
    }
    else
    {
        leftMotorDir = 1;
    }

    // Initiallize Right Motor "forward" direction
    if(right_flip_dir == false)
    {
        rightMotorDir = 0;   // default motor direction setting
    }
    else
    {
        rightMotorDir = 1;
    }
    rightMotorDirFwd = rightMotorDir;
}

void motor_set_left_effort(float effort)
{
    int reverse = 0;

    if(effort < 0)
    {
        effort = -effort;   // make effort a positive quantity
        reverse = 1;        // preserve the direction
    }

    if(effort > 1)
    {
        effort = 1;         // clamp at maximum effort
    }
    
    // Update motor direction
    if(reverse == 0)
    {
        leftMotorDir = leftMotorDirFwd;
    }
    else
    {
        leftMotorDir  = (leftMotorDirFwd ^ 1);
    }
    
    switch (leftMotorDir)
    {
      case 0:
        lServo.write(90-(effort*SERVO_RESOLUTION));
        break;
      case 1:
        lServo.write(90+(effort*SERVO_RESOLUTION));
        break;
      default:
        break;
    }
}

void motor_set_right_effort(float effort)
{
    int reverse = 0;

    if(effort < 0)
    {
        effort = -effort;   // make effort a positive quantity
        reverse = 1;        // preserve the direction
    }

    if(effort > 1)
    {
        effort = 1;         // clamp at maximum effort
    }
    
    // Update motor direction
    if(reverse == 0)
    {
        rightMotorDir = rightMotorDirFwd;
    }
    else
    {
        rightMotorDir  = (rightMotorDirFwd ^ 1);
    }
    
    switch (rightMotorDir)
    {
      case 0:
        rServo.write(90-(effort*SERVO_RESOLUTION));
        break;
      case 1:
        rServo.write(90+(effort*SERVO_RESOLUTION));
        break;
      default:
        break;
    }

}

void motor_set_efforts(float leftEffort, float rightEffort)
{
    motor_set_left_effort(leftEffort);
    motor_set_right_effort(rightEffort);
}


