/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            servoarm.cpp
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

/** Include Files *************************************************************/
#include <Arduino.h>                // Required for Arduino functions
#include <EEPROM.h>                 // Required for EEPROM emulation functions
#include <Servo.h>                  // Required for the Servo library
#include "servoarm.h"               // "servoarm" API declarations
#include "board.h"                  // "board" functions

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/
Servo Servo1;
static int setAngle;

extern const struct SERVOARM_INTERFACE SERVOARM = {
    .initialize             = &servoarm_init,
    .set_angle              = &servoarm_set_angle,
    .get_angle              = &servoarm_get_angle,
    .home                   = &servoarm_home,
    .lift                   = &servoarm_lift,
    .drop                   = &servoarm_drop,
    .clear_calibration      = &servoarm_clear_calibration
};

// calibration objects with default values assigned
static struct SERVOARM_CAL servoarmCal = {
    .home_angle = HOME_POSITION_DEFAULT_ANGLE,
    .lift_angle = LIFT_POSITION_DEFAULT_ANGLE,
    .drop_angle = DROP_POSITION_DEFAULT_ANGLE
};

static enum SERVOARM_CALIBRATION_STATE servoarmCalState = SERVOARM_CAL_IDLE;

/*** Private Function Prototypes **********************************************/

/*** Public Function Definitions **********************************************/

void servoarm_init(void)
{
    // calibraation variables
    int calAngle = 0;

    // Initiallize Servo1 using min/max pulses experimentally confirming 0-180 deg rotation
    Servo1.attach(SERVOARM_PIN, SERVOARM_MIN_PULSE_WIDTH, SERVOARM_MAX_PULSE_WIDTH);

    // Initiallize a default (safe) home position for the servo
    setAngle = servoarmCal.home_angle;
    servoarm_set_angle(setAngle);

    // set ADC resolution to 12-bit
    analogReadResolution(12);

    // Perform servoarm position calibration if EEPROM calibration memory is blank
    EEPROM.begin(1024);
    uint32_t testRead = 0;
    if(EEPROM.get(SERVOARM_CAL_EEPROM_ADDRESS_START, testRead) == 0xFFFFFFFF)
    {
        Serial.println("ServoArm Calibration Routine Triggered. Press button to begin.");
        // EEPROM is blank, perform calibration procedure
        servoarmCalState = SERVOARM_CAL_WAIT_BEGIN;
        board_led_pattern(5);
        while (servoarmCalState != SERVOARM_CAL_IDLE)
        {
            board_tasks();
            switch(servoarmCalState)
            {
                case SERVOARM_CAL_WAIT_BEGIN:
                    if(board_is_button_pressed())
                    {
                        board_led_pattern(1);                   // indicate "SERVOARM_CAL_HOME" state
                        servoarmCalState = SERVOARM_CAL_HOME;   // set the "home" angle
                        Serial.println("Rotate POT to set HOME position. Press button when done.");
                    }
                    break;
                case SERVOARM_CAL_HOME:
                    if(board_is_button_pressed())
                    {
                        servoarmCal.home_angle = calAngle;      // save current setting as "HOME"
                        board_led_pattern(2);                   // indicate "SERVOARM_CAL_LIFT" state
                        servoarmCalState = SERVOARM_CAL_LIFT;   // set the "lift" angle
                        Serial.println("Rotate POT to set LIFT position. Press button when done.");
                    }
                    else
                    {
                        calAngle = (int)map(analogRead(POT_PIN), 0, 4096, 0, 180);
                        Servo1.write(calAngle);
                    }
                    break;
                case SERVOARM_CAL_LIFT:
                    if(board_is_button_pressed())
                    {
                        servoarmCal.lift_angle = calAngle;      // save current setting as "LIFT"
                        board_led_pattern(3);                   // indicate "SERVOARM_CAL_DROP" state
                        servoarmCalState = SERVOARM_CAL_DROP;   // set the "drop" angle
                        Serial.println("Rotate POT to set DROP position. Press button when done.");
                    }
                    else
                    {
                        calAngle = (int)map(analogRead(POT_PIN), 0, 4096, 0, 180);
                        Servo1.write(calAngle);
                    }
                    break;
                case SERVOARM_CAL_DROP:
                    if(board_is_button_pressed())
                    {
                        servoarmCal.drop_angle = calAngle;      // save current setting as "DROP"
                        board_led_off();                        // turn off the led
                        servoarmCalState = SERVOARM_CAL_IDLE;   // terminate calibration
                        Serial.println("ServoArm Calibration Routine Completed.");
                    }
                    else
                    {
                        calAngle = (int)map(analogRead(POT_PIN), 0, 4096, 0, 180);
                        Servo1.write(calAngle);
                    }
                    break;    
                default:
                    servoarmCalState = SERVOARM_CAL_IDLE;
                    board_led_off();
                    break;
            }
        }
        // Save calibration values to EEPROM memory
        EEPROM.put(SERVOARM_CAL_EEPROM_ADDRESS_START, servoarmCal);
        Serial.print("ServoArm Home Position (angle): ");
        Serial.print(servoarmCal.home_angle);
        Serial.print(" ServoArm Lift Position (angle): ");
        Serial.print(servoarmCal.lift_angle);
        Serial.print(" ServoArm Drop Position (angle): ");
        Serial.println(servoarmCal.drop_angle);
    }
    else
    {
        // EEPROM is programmed with calibration values, so use them
        EEPROM.get(SERVOARM_CAL_EEPROM_ADDRESS_START, servoarmCal);
        Serial.print("ServoArm Home Position (angle): ");
        Serial.print(servoarmCal.home_angle);
        Serial.print(" ServoArm Lift Position (angle): ");
        Serial.print(servoarmCal.lift_angle);
        Serial.print(" ServoArm Drop Position (angle): ");
        Serial.println(servoarmCal.drop_angle);
    }
    // Write any changes to EEPROM and close interface.
    EEPROM.end();
}

void servoarm_set_angle(int desiredAngle)
{
    int delta, i;

    if(desiredAngle < 0)
    {
        Servo1.write(0);    // clamp at min angle
        setAngle = 0;
    }

    else if(desiredAngle > 180)
    {
        Servo1.write(180);  // clamp at max angle
        setAngle = 180;
    }

    else
    {
        if(desiredAngle > setAngle)
        {
            for(i=setAngle; i< desiredAngle; i++)
            {
                Servo1.write(i);
                delay(20);
            }
            setAngle = i;
        }
        else if(desiredAngle < setAngle)
        {
            for(i=setAngle; i> desiredAngle; i--)
            {
                Servo1.write(i);
                delay(20);
            }
            setAngle = i;
        }
        else
        {
            Servo1.write(desiredAngle);
            setAngle = desiredAngle;      
        }
    } 
}

int servoarm_get_angle(void)
{
  return setAngle;
}

void servoarm_home(void)
{
    servoarm_set_angle(servoarmCal.home_angle);
}

void servoarm_lift(void)
{
    servoarm_set_angle(servoarmCal.lift_angle);
}

void servoarm_drop(void)
{
    servoarm_set_angle(servoarmCal.drop_angle);
}

void servoarm_clear_calibration(void)
{
    EEPROM.begin(1024);
    // Erase EEPROM memory to trigger calibration routines during initialization
    for (int i = SERVOARM_CAL_EEPROM_ADDRESS_START; i <= SERVOARM_CAL_EEPROM_ADDRESS_END; i++) {
      EEPROM.write(i, 255);
    }
    EEPROM.end();
}

