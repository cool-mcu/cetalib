/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            imu.h
 * Project:         
 * Date:            July 08, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * cetalib "imu" driver interface functions
 *
 * This library depends on the Arduino_LSM6DSOX Library:
 * https://github.com/arduino-libraries/Arduino_LSM6DSOX/tree/master
 * https://www.arduino.cc/reference/en/libraries/arduino_lsm6dsox 
 * 
 * Arduino_LSM6DSOX Library initializes the sensor with the following settings:
 *  - Accelerometer range is set at ±4 g with a resolution of 0.122 mg.
 *  - Gyroscope range is set at ±2000 dps with a resolution of 70 mdps.
 *  - Accelerometer and gyrospcope output data rate is fixed at 104 Hz.
 * 
 * Hardware Configuration:
 * 
 * CETA IoT Robot (schematic #14-00069B), based on RPI-Pico-WH,
 * 
 * Uses Adafruit LSM6DSOX 6 DoF Accelerometer and Gyroscope board (#4438),
 * connected to "Wire1" instance (I2C1 on pins SDA/GP18 & SCL/GP19) 
 *
 */

#ifndef IMU_H_
#define IMU_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "imu_interface.h"

/*** Macros *******************************************************************/
#define IMU_SCL_PIN 19
#define IMU_SDA_PIN 18
#define IMU_I2C_ADDRESS 0x6A
#define IMU_I2C_BAUD 400000
#define IMU_CAL_EEPROM_ADDRESS_START 256    // EEPROM Start address for calibration data
#define IMU_CAL_EEPROM_ADDRESS_END   383    // EEPROM End address for calibration data
#define IMU_SAMPLE_INTERVAL_MS  50          // Sensor sample interval (in mSec)
#define IMU_SAMPLE_INTERVAL_S   0.05f       // Sensor sample interval (in Seconds)
#define IMU_YAW_OFFSET_ERROR_DEFAULT  0.02f // Default yaw reading offset error
#define IMU_YAW_GAIN_COEFFICIENT_DEFAULT 1.125f // Default yaw gain coefficient

/*** Custom Data Types ********************************************************/

enum IMU_CALIBRATION_STATE {IMU_CAL_WAIT_BEGIN=0, IMU_CAL_HEADING_OFFSET_ERROR, IMU_CAL_HEADING_GAIN, IMU_CAL_IDLE};

struct IMU_CAL
{
  float yaw_offset_error;       // error term to subract from gyro reading
  float yaw_gain_coefficient;   // coefficient to multply heading by
};

/*** Public Function Prototypes ***********************************************/
bool  imu_init(void);
void  imu_tasks(void);
float imu_get_temperature(void);
float imu_get_heading(void);
void  imu_reset_heading(void);
void  imu_clear_calibration(void);

#endif /* IMU_H_ */