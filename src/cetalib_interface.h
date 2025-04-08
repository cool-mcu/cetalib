/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            cetalib_interface.h
 * Project:         
 * Date:            Mar 29, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "cetalib" driver interface file - defines "CETALIB_INTERFACE" structure
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

 #ifndef CETALIB_INTERFACE_H_
 #define CETALIB_INTERFACE_H_
 
 /*** Include Files ************************************************************/
 #include <Arduino.h>
 #include "./modules/board_interface.h"
 #include "./modules/motor_interface.h"
 #include "./modules/encoder_interface.h"
 #include "./modules/reflectance_interface.h"
 #include "./modules/servoarm_interface.h"
 #include "./modules/rangefinder_interface.h"
 #include "./modules/imu_interface.h"
 #include "./modules/mqttc_interface.h"
 #include "./modules/diffDrive_interface.h"
 #include "./modules/oled_interface.h"
 
 /*** Macros *******************************************************************/
 
 /*** Custom Data Types ********************************************************/
 
 #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
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
   const struct OLED_INTERFACE *oled;                // Pointer to a OLED_INTERFACE instance
 };
 #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
 struct CETALIB_INTERFACE
 {
   const struct BOARD_INTERFACE *board;              // Pointer to a BOARD_INTERFACE instance
   const struct MOTOR_INTERFACE *motor;              // Pointer to a MOTOR_INTERFACE instance
   const struct ENCODER_INTERFACE *encoder;          // Pointer to a ENCODER_INTERFACE instance
   const struct RANGEFINDER_INTERFACE *rangefinder;  // Pointer to a RANGEFINDER_INTERFACE instance
   const struct OLED_INTERFACE *oled;                // Pointer to a OLED_INTERFACE instance
  };
 #else
   #error Unsupported board selection
 #endif
 
 /*** Public Function Prototypes ***********************************************/
 
 
 #endif /* CETALIB_INTERFACE_H_ */
 