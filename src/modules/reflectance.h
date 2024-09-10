/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            reflectance.h
 * Project:         
 * Date:            June 26, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * cetalib "reflectance" opto-sensor driver interface functions
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH 
 *
 */

#ifndef REFLECTANCE_H_
#define REFLECTANCE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "reflectance_interface.h"

/*** Macros *******************************************************************/
#define LEFT_SENSOR_PIN             A2        // Left sensor connected to analog pin AIN2
#define MIDDLE_SENSOR_PIN           A1        // Middle sensor connected to analog pin AIN1
#define RIGHT_SENSOR_PIN            A0        // Right sensor connected to analog pin AIN0
#define MAX_ADC_VALUE               4096.0f   // 12-bit ADC max value
#define LEFT_SENSOR_TRIP_DEFAULT    0.733f    // Default trip threshold
#define MIDDLE_SENSOR_TRIP_DEFAULT  0.733f    // Default trip threshold
#define RIGHT_SENSOR_TRIP_DEFAULT   0.733f    // Default trip threshold
#define REFLECTANCE_CAL_EEPROM_ADDRESS_START  0  // EEPROM Start address for calibration data
#define REFLECTANCE_CAL_EEPROM_ADDRESS_END  127  // EEPROM End address for calibration data
#define CALIBRATION_INTERVAL        10000     // Average sensor readings over this interval during calibration (in mS)
#define SAMPLE_INTERVAL             10        // Sensor sample interval during calibration (in mS)


/*** Custom Data Types ********************************************************/

enum REFLECTANCE_CALIBRATION_STATE {WAIT_BEGIN_WHITE=0, WAIT_BEGIN_BLACK, CALIBRATE_WHITE, CALIBRATE_BLACK, IDLE};

struct REFLECTANCE_CAL
{
  float left_opto_trip;
  float middle_opto_trip;
  float right_opto_trip;
};

/*** Public Function Prototypes ***********************************************/
void reflectance_init(void);                    // Initiallize pins, state variables, calibrate if needed
float reflectance_get_left_sensor(void);        // Sample left sensor reading
float reflectance_get_middle_sensor(void);      // Sample middle sensor reading
float reflectance_get_right_sensor(void);       // Sample right sensor reading
int reflectance_get_line_status(void);          // Sample/Return current line detection status
void reflectance_clear_calibration(void);       // Delete calibration data

#endif /* REFLECTANCE_H_ */