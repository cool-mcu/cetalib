/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            rangefinder.cpp
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

/** Include Files *************************************************************/
#include <Arduino.h>                // Required for Arduino functions
#include "rangefinder.h"            // "rangefinder" API declarations

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/

// initialize reflectance function pointers
extern const struct RANGEFINDER_INTERFACE RANGEFINDER = {
    .initialize             = &rangefinder_init,
    .get_distance           = &rangefinder_get_distance
};

static unsigned int timeoutUs, maxDistanceCm, maxTimeoutMicroSec;

/*** Private Function Prototypes **********************************************/
static float measureDistanceCm(float temperature);

/*** Public Function Definitions **********************************************/

void rangefinder_init(void)
{
    timeoutUs = HCSR04_TIMEOUT_US;
    maxDistanceCm = (unsigned int)MAX_DISTANCE_CM;
    maxTimeoutMicroSec = HCSR04_TIMEOUT_US;
    pinMode(HCSR04_TRIGGER_PIN, OUTPUT);
    pinMode(HCSR04_ECHO_PIN, INPUT);
}

float rangefinder_get_distance(void)
{
    //Using the approximate formula 19.307°C results in roughly 343m/s which is the commonly used value for air.
    return measureDistanceCm(19.307);
}

static float measureDistanceCm(float temperature)
{
    unsigned long maxDistanceDurationMicroSec;

    // Make sure that trigger pin is LOW.
    digitalWrite(HCSR04_TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    // Hold trigger for 10 microseconds, which is signal for sensor to measure distance.
    digitalWrite(HCSR04_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(HCSR04_TRIGGER_PIN, LOW);
    float speedOfSoundInCmPerMicroSec = 0.03313 + 0.0000606 * temperature; // C (air) ≈ (331.3 + 0.606 ⋅ ϑ) m/s

    // Compute max delay based on max distance with 25% margin in microseconds
    maxDistanceDurationMicroSec = 2.5 * maxDistanceCm / speedOfSoundInCmPerMicroSec;
    if (maxTimeoutMicroSec > 0) {
    	maxDistanceDurationMicroSec = min(maxDistanceDurationMicroSec, maxTimeoutMicroSec);
    }

    // Measure the length of echo signal, which is equal to the time needed for sound to go there and back.
    unsigned long durationMicroSec = pulseIn(HCSR04_ECHO_PIN, HIGH, maxDistanceDurationMicroSec); // can't measure beyond max distance

    float distanceCm = durationMicroSec / 2.0 * speedOfSoundInCmPerMicroSec;
    if (distanceCm == 0 || distanceCm > maxDistanceCm) {
        return -1.0 ;
    } else {
        return distanceCm;
    }
}