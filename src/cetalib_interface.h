/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            cetalib_interface.h
 * Project:         
 * Date:            June 24, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * "cetalib" driver interface file - defines "CETALIB_INTERFACE" structure
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH 
 *
 */

#ifndef CETALIB_INTERFACE_H_
#define CETALIB_INTERFACE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "./modules/board_interface.h"
#include "./modules/motor_interface.h"
#include "./modules/reflectance_interface.h"
#include "./modules/servoarm_interface.h"
#include "./modules/rangefinder_interface.h"
#include "./modules/imu_interface.h"
#include "./modules/mqttc_interface.h"
#include "./modules/diffDrive_interface.h"

/*** Macros *******************************************************************/

/*** Custom Data Types ********************************************************/
struct CETALIB_INTERFACE
{
  const struct BOARD_INTERFACE *board;              // Pointer to a BOARD_INTERFACE instance
  const struct MOTOR_INTERFACE *motor;              // Pointer to a MOTOR_INTERFACE instance
  const struct REFLECTANCE_INTERFACE *reflectance;  // Pointer to a REFLECTANCE_INTERFACE instance
  const struct SERVOARM_INTERFACE *servoarm;        // Pointer to a SERVOARM_INTERFACE instance
  const struct RANGEFINDER_INTERFACE *rangefinder;  // Pointer to a RANGEFINDER_INTERFACE instance
  const struct CETA_IMU_INTERFACE *imu;             // Pointer to a IMU_INTERFACE instance 
  const struct MQTTC_INTERFACE *mqttc;              // Pointer to a MQTTC_INTERFACE instance
  const struct DIFFDRIVE_INTERFACE *diffDrive;      // Pointer to a DIFFDRIVE_INTERFACE instance
};

/*** Public Function Prototypes ***********************************************/


#endif /* CETALIB_INTERFACE_H_ */
