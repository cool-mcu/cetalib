/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            encoder.cpp
 * Project:         
 * Date:            Mar 29, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "encoder" driver interface functions
 * 
 * API based on XRPlib library APIs
 * https://open-stem.github.io/XRP_MicroPython/index.html
 *
 * Based on "quadrature_encoder" example in the Pico SDK:
 * https://github.com/raspberrypi/pico-examples/tree/master/pio/quadrature_encoder 
 * 
 * Used online web-based PIOASM to create .h file from .pio assembler
 * https://wokwi.com/tools/pioasm 
 * 
 * Hardware Configurations Supported:
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select "Board = SparkFun XRP Controller")
 *
 */

 /** Include Files *************************************************************/
#include <Arduino.h>                // Required for Arduino functions
#include <pio_encoder.h>            // "PioEncoder" object
#include "encoder.h"

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/
PioEncoder leftEncoder(LEFT_MOTOR_ENCODER_A_PIN, false, 0, COUNT_4X, pio2, -1, 0);
PioEncoder rightEncoder(RIGHT_MOTOR_ENCODER_A_PIN, false, 0, COUNT_4X, pio2, -1, 0);

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
    leftEncoder.begin();
    rightEncoder.begin();
}

int encoder_get_left_position_counts(void)
{
    // left motor encoder output is negative for forward motion
    // need to correct with multiplication by -1.
    return (-1)*leftEncoder.getCount();
}

int encoder_get_right_position_counts(void)
{
    return rightEncoder.getCount();
}

void encoder_reset_left_position(void)
{
    leftEncoder.reset();
}

void encoder_reset_right_position(void)
{
    rightEncoder.reset();
}

float encoder_get_left_position(void)
{
    return encoder_get_left_position_counts() / RESOLUTION;
}

float encoder_get_right_position(void)
{
    return encoder_get_right_position_counts() / RESOLUTION;
}

