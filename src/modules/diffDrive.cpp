/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            diffDrive.cpp
 * Project:         
 * Date:            July 22, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * cetalib "differential drive" driver interface functions
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH 
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
static float left_right_compensation = LEFT_RIGHT_COMPENSATION_DEFAULT;

/*** Type Declarations ********************************************************/
extern const struct DIFFDRIVE_INTERFACE DIFFDRIVE = {
    .initialize             = &diffDrive_init,
    .set_efforts            = &diffDrive_set_efforts,
    .stop                   = &diffDrive_stop,
    .straight               = &diffDrive_straight,
    .turn                   = &diffDrive_turn,
    .clear_calibration      = &diffDrive_clear_calibration,
    .save_straight_compensation = &diffDrive_save_straight_compensation
};

/*** Private Function Prototypes **********************************************/

/*** Public Function Definitions **********************************************/

void diffDrive_init(bool left_flip_dir, bool right_flip_dir)
{
    motor_init(left_flip_dir, right_flip_dir);
    
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
}

void diffDrive_set_efforts(float leftEffort, float rightEffort)
{
    motor_set_efforts(leftEffort, rightEffort);
}

void diffDrive_stop(void)
{
    motor_set_efforts(0, 0);
}

void diffDrive_straight(float straightEffort)
{
    float leftMotorEffort = straightEffort;
    float rightMotorEffort = leftMotorEffort/left_right_compensation;
    motor_set_efforts(leftMotorEffort, rightMotorEffort);
}

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

void diffDrive_clear_calibration(void)
{
    EEPROM.begin(1024);
    // Erase EEPROM memory to trigger calibration routines during initialization
    for (int i = DIFFDRIVE_CAL_EEPROM_ADDRESS_START; i <= DIFFDRIVE_CAL_EEPROM_ADDRESS_END; i++) {
      EEPROM.write(i, 255);
    }
    EEPROM.end();
}

void diffDrive_save_straight_compensation(float leftRightComp)
{
    // update the compensation value
    left_right_compensation = leftRightComp;
    EEPROM.begin(1024);
    EEPROM.put(DIFFDRIVE_CAL_EEPROM_ADDRESS_START, leftRightComp);
    EEPROM.end();
}

