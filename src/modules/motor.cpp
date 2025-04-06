/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            motor.cpp
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

/** Include Files *************************************************************/
#include <Arduino.h>            // Required for Arduino functions
#include <Servo.h>              // Required for Servo functions
#include "motor.h"              // "motor" API declarations

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/
Servo lServo;             // LEFT motor servo object
Servo rServo;             // RIGHT motor servo object
static int leftMotorDir, leftMotorDirFwd, rightMotorDir, rightMotorDirFwd;

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
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
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
    
    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
    
    // Initiallize Left Motor PWM Pins and "forward" direction
    digitalWrite(LEFT_MOTOR_IN1_PIN, LOW);
    digitalWrite(LEFT_MOTOR_IN2_PIN, LOW);
    pinMode(LEFT_MOTOR_IN1_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_IN2_PIN, OUTPUT);
    if(left_flip_dir == false)
    {
        leftMotorDir = 0;   // default motor direction setting
    }
    else
    {
        leftMotorDir = 1;
    }
    leftMotorDirFwd = leftMotorDir;

    // Initiallize Right Motor PWM Pins and "forward" direction
    digitalWrite(RIGHT_MOTOR_IN1_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_IN2_PIN, LOW);
    pinMode(RIGHT_MOTOR_IN1_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_IN2_PIN, OUTPUT);
    if(right_flip_dir == false)
    {
        rightMotorDir = 0;   // default motor direction setting
    }
    else
    {
        rightMotorDir = 1;
    }
    rightMotorDirFwd = rightMotorDir;
    
    // Initiallize PWM functionalty
    analogWriteFreq(PWM_FREQ);
    analogWriteRange(PWM_RESOLUTION);
    
    #else
        #error Unsupported board selection
    #endif
}

void motor_set_left_effort(float leftMotorEffort)
{
    int reverse = 0;

    if(leftMotorEffort < 0)
    {
        leftMotorEffort = -leftMotorEffort;   // make effort a positive quantity
        reverse = 1;        // preserve the direction
    }

    if(leftMotorEffort > 1)
    {
        leftMotorEffort = 1;         // clamp at maximum effort
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
    
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    switch (leftMotorDir)
    {
      case 0:
        lServo.write(90-(leftMotorEffort*SERVO_RESOLUTION));
        break;
      case 1:
        lServo.write(90+(leftMotorEffort*SERVO_RESOLUTION));
        break;
      default:
        break;
    }
    
    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
    // Perform DRV8411 "slow decay" PWM control  
    switch (leftMotorDir)
    {
      case 0:
        digitalWrite(LEFT_MOTOR_IN1_PIN, HIGH);
        analogWrite(LEFT_MOTOR_IN2_PIN, int((1-leftMotorEffort)*PWM_RESOLUTION));
        break;
      case 1:
        analogWrite(LEFT_MOTOR_IN1_PIN, int((1-leftMotorEffort)*PWM_RESOLUTION));
        digitalWrite(LEFT_MOTOR_IN2_PIN, HIGH);
        break;
      default:
        break;
    }
    
    #else
        #error Unsupported board selection
    #endif
}

void motor_set_right_effort(float rightMotorEffort)
{
    int reverse = 0;

    if(rightMotorEffort < 0)
    {
        rightMotorEffort = -rightMotorEffort;   // make effort a positive quantity
        reverse = 1;        // preserve the direction
    }

    if(rightMotorEffort > 1)
    {
        rightMotorEffort = 1;         // clamp at maximum effort
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
    
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    switch (rightMotorDir)
    {
      case 0:
        rServo.write(90-(rightMotorEffort*SERVO_RESOLUTION));
        break;
      case 1:
        rServo.write(90+(rightMotorEffort*SERVO_RESOLUTION));
        break;
      default:
        break;
    }
    
    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
    // Perform DRV8411 "slow decay" PWM direction control
    switch (rightMotorDir)
    {
      case 0:
        digitalWrite(RIGHT_MOTOR_IN1_PIN, HIGH);
        analogWrite(RIGHT_MOTOR_IN2_PIN, int((1-rightMotorEffort)*PWM_RESOLUTION));
        break;
      case 1:
        analogWrite(RIGHT_MOTOR_IN1_PIN, int((1-rightMotorEffort)*PWM_RESOLUTION));
        digitalWrite(RIGHT_MOTOR_IN2_PIN, HIGH);
        break;
      default:
        break;
    }
    
    #else
        #error Unsupported board selection
    #endif

}

void motor_set_efforts(float leftMotorEffort, float rightMotorEffort)
{
    motor_set_left_effort(leftMotorEffort);
    motor_set_right_effort(rightMotorEffort);
}


