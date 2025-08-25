/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            diffDrive.cpp
 * Project:         
 * Date:            Aug 18, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "differential drive" driver interface file - defines "DIFFDRIVE_INTERFACE" structure
 *
 * Hardware Configurations Supported:
 * 
 * CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
 * (Select "Board = Raspberry Pi Pico W")
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select "Board = SparkFun XRP Controller")
 *
 * Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
 * (Select "Board = SparkFun XRP Controller (Beta)")
 *
 */

/** Include Files *************************************************************/
#include <Arduino.h>            // Required for Arduino functions
#include <EEPROM.h>             // Required for EEPROM emulation functions
#include <math.h>               // Required for standard C math library routines
#include "motor.h"              // "motor" functions
#include "imu.h"                // "imu" functions
#include "board.h"              // "board" functions
#include "diffDrive.h"          // "diffDrive" functions

/*** Symbolic Constants used in this module ***********************************/
#define SERIAL_PORT Serial  // Default to Serial
#if defined(NO_USB)
    #undef SERIAL_PORT
    #define SERIAL_PORT Serial1     // Use Serial1 if USB is disabled
#endif
/*** Global Variable Declarations *********************************************/

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  static float left_right_compensation = LEFT_RIGHT_COMPENSATION_DEFAULT;
#endif

/*** Type Declarations ********************************************************/

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
extern const struct DIFFDRIVE_INTERFACE DIFFDRIVE = {
    .initialize             = &diffDrive_init,
    .set_efforts            = &diffDrive_set_efforts,
    .stop                   = &diffDrive_stop,
    .straight               = &diffDrive_straight,
    .turn                   = &diffDrive_turn,
    .clear_calibration      = &diffDrive_clear_calibration,
    .save_straight_compensation = &diffDrive_save_straight_compensation
};

#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
extern const struct DIFFDRIVE_INTERFACE DIFFDRIVE = {
    .initialize             = &diffDrive_init,
    .set_efforts            = &diffDrive_set_efforts,
    .stop                   = &diffDrive_stop
};
#else
  #error Unsupported board selection
#endif

/*** Private Function Prototypes **********************************************/

/*** Public Function Definitions **********************************************/

void diffDrive_init(bool left_flip_dir, bool right_flip_dir)
{
    motor_init(left_flip_dir, right_flip_dir);
    
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    // Update left_right_compensation variable with default or saved value
    EEPROM.begin(1024);
    uint32_t testRead = 0;
    if(EEPROM.get(DIFFDRIVE_CAL_EEPROM_ADDRESS_START, testRead) == 0xFFFFFFFF)
    {
        left_right_compensation = LEFT_RIGHT_COMPENSATION_DEFAULT;
        EEPROM.put(DIFFDRIVE_CAL_EEPROM_ADDRESS_START, left_right_compensation);
    }
    else
    {
        EEPROM.get(DIFFDRIVE_CAL_EEPROM_ADDRESS_START, left_right_compensation);
    }
    EEPROM.end();
    SERIAL_PORT.print("\"diffDrive_straight()\" Left Right Compensation: ");
    SERIAL_PORT.println(left_right_compensation);
    SERIAL_PORT.println();
    #endif
}

void diffDrive_set_efforts(float leftEffort, float rightEffort)
{
    motor_set_efforts(leftEffort, rightEffort);
}

void diffDrive_stop(void)
{
    motor_set_efforts(0, 0);
}

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
void diffDrive_straight(float straightEffort)
{
    float leftMotorEffort = straightEffort;
    float rightMotorEffort = leftMotorEffort/left_right_compensation;
    motor_set_efforts(leftMotorEffort, rightMotorEffort);
}
#endif

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
void diffDrive_turn(float turnDegrees, float turnEffort)
{
    float heading;
    double temp = turnDegrees / 360.0;
    float turn_degrees = 360.0f * float(temp - floor(temp));
    motor_set_efforts(0.0, 0.0);
    imu_reset_heading();
    float effort = abs(turnEffort);
    if (turnEffort > 0.0f)
    {
        // turn cw
        motor_set_efforts(effort, -effort);
    }
    else if (turnEffort < 0.0f)
    {
        // turn ccw
        motor_set_efforts(-effort, effort);
    }
    else
    {
        return;
    }
    do
    {
        imu_tasks();
        heading = imu_get_heading();
    } while (abs(heading) < turn_degrees);
    motor_set_efforts(0.0, 0.0);
}
#endif

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
void diffDrive_clear_calibration(void)
{
    EEPROM.begin(1024);
    // Erase EEPROM memory to trigger calibration routines during initialization
    for (int i = DIFFDRIVE_CAL_EEPROM_ADDRESS_START; i <= DIFFDRIVE_CAL_EEPROM_ADDRESS_END; i++) {
      EEPROM.write(i, 255);
    }
    EEPROM.end();
}
#endif

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
void diffDrive_save_straight_compensation(float leftRightComp)
{
    // update the compensation value
    left_right_compensation = leftRightComp;
    EEPROM.begin(1024);
    EEPROM.put(DIFFDRIVE_CAL_EEPROM_ADDRESS_START, leftRightComp);
    EEPROM.end();
}
#endif

