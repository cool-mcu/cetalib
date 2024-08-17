/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            servoarm.h
 * Project:         
 * Date:            June 28, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * cetalib "servoarm" driver interface functions
 *
 * This library uses the "Servo" library implementation in Arduino-Pico:
 * https://github.com/earlephilhower/arduino-pico/blob/master/libraries/Servo/src/Servo.h
 * 
 * - initialized min pulse width: 544 uS (0 degrees)
 * - initialized max pulse width: 2500 uS (180 degrees)
 * - default neutral pulse width when servo is attached: 1500 uS (90 degrees)
 * - refresh interval: 20000 uS
 * - uses PIO0 peripheral
 * 
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069B), based on RPI-Pico-WH with SG92R Servo  
 *
 */

#ifndef SERVOARM_H_
#define SERVOARM_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "servoarm_interface.h"

/*** Macros *******************************************************************/
#define SERVOARM_PIN                      22
#define POT_PIN                           A2
#define SERVOARM_MIN_PULSE_WIDTH          544   // Corresponds to 0 deg position
#define SERVOARM_MAX_PULSE_WIDTH          2500  // Corresponds to 180 deg position
#define HOME_POSITION_DEFAULT_ANGLE       102   // Default "home" position servo angle
#define LIFT_POSITION_DEFAULT_ANGLE       115   // Default "lift" position servo angle
#define DROP_POSITION_DEFAULT_ANGLE       98    // Default "drop" position servo angle
#define SERVOARM_CAL_EEPROM_ADDRESS_START 128   // EEPROM Start address for calibration data
#define SERVOARM_CAL_EEPROM_ADDRESS_END   255   // EEPROM End address for calibration data

/*** Custom Data Types ********************************************************/

enum SERVOARM_CALIBRATION_STATE {SERVOARM_CAL_WAIT_BEGIN=0, SERVOARM_CAL_HOME, SERVOARM_CAL_LIFT, SERVOARM_CAL_DROP, SERVOARM_CAL_IDLE};

struct SERVOARM_CAL
{
  int home_angle;
  int lift_angle;
  int drop_angle;
};

/*** Public Function Prototypes ***********************************************/
void servoarm_init(void);
void servoarm_set_angle(int angle);
int servoarm_get_angle(void);
void servoarm_home(void);
void servoarm_lift(void);
void servoarm_drop(void);
void servoarm_clear_calibration(void);

#endif /* SERVOARM_H_ */