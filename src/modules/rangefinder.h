/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            rangefinder.h
 * Project:         
 * Date:            Junly 05, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * cetalib "rangefinder" ultrasonic sensor driver interface functions
 * 
 * Code is based on "arduino-lib-hc-sr04" library v2.0:
 * https://github.com/Martinsos/arduino-lib-hc-sr04/tree/master
 *
 * Hardware Configuration:
 * 
 * CETA IoT Robot (schematic #14-00069B), based on RPI-Pico-WH with HC-SR04P sensor from ABRA
 * (https://abra-electronics.com/sensors/sensors-proximity-en/hc-sr04p.html)
 * 
 * See "Tech Brief #3 - Add Collision Sensor to the Pico W IoT Robot"
 * (https://www.cool-mcu.com/courses/take/tech-briefs-19-00030a/texts/44839270-purpose-and-overview)
 *
 */

#ifndef RANGEFINDER_H_
#define RANGEFINDER_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "rangefinder_interface.h"

/*** Macros *******************************************************************/
#define HCSR04_TRIGGER_PIN  17
#define HCSR04_ECHO_PIN     16
#define HCSR04_TIMEOUT_US   5000    // blocks for 5ms Max, provides up to ~45cm range
#define MAX_DISTANCE_CM     400     // Maximum distance sensor can measure (in cm)

/*** Custom Data Types ********************************************************/


/*** Public Function Prototypes ***********************************************/
void rangefinder_init(void);                    // Initiallize pins, state variables
float rangefinder_get_distance(void);           // Measure distance to target (in cm)

#endif /* RANGEFINDERE_H_ */