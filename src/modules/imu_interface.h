/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            imu_interface.h
 * Project:         
 * Date:            July 07, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * "imu" driver interface file - defines "IMU_INTERFACE" structure
 *
 * Hardware Configuration:
 * 
 * CETA IoT Robot (schematic #14-00069B), based on RPI-Pico-WH,
 * 
 * Uses Adafruit LSM6DSOX 6 DoF Accelerometer and Gyroscope board (#4438),
 * connected to "Wire1" instance (I2C1 on pins SDA/GP18 & SCL/GP19) 
 *
 */

#ifndef IMU_INTERFACE_H_
#define IMU_INTERFACE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>

/*** Macros *******************************************************************/

/*** Custom Data Types ********************************************************/
struct CETA_IMU_INTERFACE
{
  bool (*initialize)(void);               // Initiallize pins & state variables, calibrate if needed
  void (*tasks)(void);                    // Run all background tasks
  float (*get_temperature)(void);         // Get the ambient temperature in degrees celcius
  float (*get_heading)(void);             // Return the current robot heading ("yaw") (0-360 degrees)
  void (*reset_heading)(void);            // Reset the heading value
  void (*clear_calibration)(void);        // Delete calibration data
};

/*** Public Function Prototypes ***********************************************/


#endif /* IMU_INTERFACE_H_ */