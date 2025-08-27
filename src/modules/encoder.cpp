/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            encoder.cpp
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
 * Uses rp2040-encoder-library by Giovanni di Dio Bruno:
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

 /** Include Files *************************************************************/
#include <Arduino.h>                // Required for Arduino functions
#include <pio_encoder.h>            // "PioEncoder" object
#include "encoder.h"

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/
#if defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
//PioEncoder leftEncoder(LEFT_MOTOR_ENCODER_A_PIN, true, 0, COUNT_4X, pio2, -1, 0);
PioEncoder leftEncoder(LEFT_MOTOR_ENCODER_A_PIN, true);
//PioEncoder rightEncoder(RIGHT_MOTOR_ENCODER_A_PIN, false, 0, COUNT_4X, pio2, -1, 0);
PioEncoder rightEncoder(RIGHT_MOTOR_ENCODER_A_PIN, false);
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
//PioEncoder leftEncoder(LEFT_MOTOR_ENCODER_A_PIN, true, 0, COUNT_4X, pio1, -1, 0);
//PioEncoder leftEncoder(LEFT_MOTOR_ENCODER_A_PIN, true, 0, COUNT_4X, pio0, -1, 0);
PioEncoder leftEncoder(LEFT_MOTOR_ENCODER_A_PIN, true);
//PioEncoder rightEncoder(RIGHT_MOTOR_ENCODER_A_PIN, false, 0, COUNT_4X, pio1, -1, 0);
//PioEncoder rightEncoder(RIGHT_MOTOR_ENCODER_A_PIN, false, 0, COUNT_4X, pio0, -1, 0);
PioEncoder rightEncoder(RIGHT_MOTOR_ENCODER_A_PIN, false);
#endif

/*** Type Declarations ********************************************************/
extern const struct ENCODER_INTERFACE ENCODER = {
  .initialize                 = &encoder_init,
  .get_left_position          = &encoder_get_left_position,
  .get_right_position         = &encoder_get_right_position,
  .get_left_position_counts   = &encoder_get_left_position_counts,
  .get_right_position_counts  = &encoder_get_right_position_counts,
  .reset_left_position        = &encoder_reset_left_position,
  .reset_right_position       = &encoder_reset_right_position
};

/*** Private Function Prototypes **********************************************/

/*** Public Function Definitions **********************************************/

void encoder_init(void)
{
    #if defined(ARDUINO_SPARKFUN_XRP_CONTROLLER) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
    leftEncoder.begin();
    rightEncoder.begin();
    #endif
}

int encoder_get_left_position_counts(void)
{
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        return 0;
    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
        return leftEncoder.getCount();
    #endif
}

int encoder_get_right_position_counts(void)
{
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        return 0;
    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
        return rightEncoder.getCount();
    #endif
}

void encoder_reset_left_position(void)
{
    #if defined(ARDUINO_SPARKFUN_XRP_CONTROLLER) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
    leftEncoder.reset();
    #endif
}

void encoder_reset_right_position(void)
{
    #if defined(ARDUINO_SPARKFUN_XRP_CONTROLLER) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
    rightEncoder.reset();
    #endif
}

float encoder_get_left_position(void)
{
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        return 0.0;
    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
        return encoder_get_left_position_counts() / RESOLUTION;
    #endif
}

float encoder_get_right_position(void)
{
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        return 0.0;
    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
        return encoder_get_right_position_counts() / RESOLUTION;
    #endif
}

