/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            rangefinder.h
 * Project:         
 * Date:            Mar 29, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "rangefinder" ultrasonic sensor driver interface functions
 * 
 * Code is based on "arduino-lib-hc-sr04" library v2.0:
 * https://github.com/Martinsos/arduino-lib-hc-sr04/tree/master
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

#ifndef RANGEFINDER_H_
#define RANGEFINDER_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "rangefinder_interface.h"

/*** Macros *******************************************************************/

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    #define HCSR04_TRIGGER_PIN  17
    #define HCSR04_ECHO_PIN     16
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
    #define HCSR04_TRIGGER_PIN  0
    #define HCSR04_ECHO_PIN     1
#else
    #error Unsupported board selection
#endif

#define HCSR04_TIMEOUT_US   5000    // blocks for 5ms Max, provides up to ~45cm range
#define MAX_DISTANCE_CM     400     // Maximum distance sensor can measure (in cm)

/*** Custom Data Types ********************************************************/


/*** Public Function Prototypes ***********************************************/
void rangefinder_init(void);                    // Initiallize pins, state variables
float rangefinder_get_distance(void);           // Measure distance to target (in cm)

#endif /* RANGEFINDERE_H_ */