/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            reflectance.cpp
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

/** Include Files *************************************************************/
#include <Arduino.h>                // Required for Arduino functions
#include <EEPROM.h>                 // EEPROM emulation routines
#include "reflectance.h"            // "reflectance" API declarations
#include "board.h"                  // "board" functions

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/

// initialize reflectance function pointers
extern const struct REFLECTANCE_INTERFACE REFLECTANCE = {
    .initialize             = &reflectance_init,
    .get_left_sensor        = &reflectance_get_left_sensor,
    .get_middle_sensor      = &reflectance_get_middle_sensor,
    .get_right_sensor       = &reflectance_get_right_sensor,
    .get_line_status        = &reflectance_get_line_status,
    .clear_calibration      = &reflectance_clear_calibration
};

// calibration objects with default values assigned
struct REFLECTANCE_CAL reflectanceCal = {
    .left_opto_trip = LEFT_SENSOR_TRIP_DEFAULT,
    .middle_opto_trip = MIDDLE_SENSOR_TRIP_DEFAULT,
    .right_opto_trip = RIGHT_SENSOR_TRIP_DEFAULT
};
enum REFLECTANCE_CALIBRATION_STATE calState = IDLE;

/*** Private Function Prototypes **********************************************/

/*** Public Function Definitions **********************************************/

void reflectance_init(void)
{
    // calibration variables
    int left_opto_accumulator, middle_opto_accumulator, right_opto_accumulator;
    int left_opto_white, middle_opto_white, right_opto_white;
    int left_opto_black, middle_opto_black, right_opto_black;
    int sample_counter;
    int num_samples = CALIBRATION_INTERVAL/SAMPLE_INTERVAL;
    unsigned long currentOptoSampleTime;
    unsigned long previousOptoSampleTime = 0;
    sample_counter = 0;
    left_opto_accumulator = 0;
    middle_opto_accumulator = 0;
    right_opto_accumulator = 0;

    // Left, Middle and Right sensor analog input pins already initialized

    // set ADC resolution to 12-bit
    analogReadResolution(12);

    // Perform line detection calibration if EEPROM calibration memory is blank
    EEPROM.begin(1024);
    uint32_t testRead = 0;
    if(EEPROM.get(REFLECTANCE_CAL_EEPROM_ADDRESS_START, testRead) == 0xFFFFFFFF)
    {
        Serial.println("Reflectance Sensor Calibration Routine Triggered");
        // EEPROM is blank, perform calibration procedure
        calState = WAIT_BEGIN;
        board_led_pattern(5);
        while (calState != IDLE)
        {
            board_tasks();
            switch(calState)
            {
                case WAIT_BEGIN:
                    if(board_is_button_pressed())
                    {
                        board_led_pattern(1);   // indicate "CALIBRATE_WHITE" state
                        calState = CALIBRATE_WHITE;
                    }
                    break;
                case CALIBRATE_WHITE:
                    if(sample_counter <= num_samples)
                    {
                        currentOptoSampleTime = millis();
                        if((currentOptoSampleTime - previousOptoSampleTime) >= SAMPLE_INTERVAL)
                        {
		                    previousOptoSampleTime = currentOptoSampleTime;
                            left_opto_accumulator += analogRead(LEFT_SENSOR_PIN);
                            middle_opto_accumulator += analogRead(MIDDLE_SENSOR_PIN);
                            right_opto_accumulator += analogRead(RIGHT_SENSOR_PIN);
                            sample_counter++;     
                        }
                    }
                    else
                    {
                        left_opto_white = left_opto_accumulator/num_samples;
                        middle_opto_white = middle_opto_accumulator/num_samples;
                        right_opto_white = right_opto_accumulator/num_samples;
                        sample_counter = 0;
                        left_opto_accumulator = 0;
                        middle_opto_accumulator = 0;
                        right_opto_accumulator = 0;
                        board_led_pattern(2);   // indicate "CALIBRATE_BLACK" state
                        calState = CALIBRATE_BLACK;
                    }
                    break;
                case CALIBRATE_BLACK:
                    if(sample_counter <= num_samples)
                    {
                        currentOptoSampleTime = millis();
                        if((currentOptoSampleTime - previousOptoSampleTime) >= SAMPLE_INTERVAL)
                        {
		                    previousOptoSampleTime = currentOptoSampleTime;
                            left_opto_accumulator += analogRead(LEFT_SENSOR_PIN);
                            middle_opto_accumulator += analogRead(MIDDLE_SENSOR_PIN);
                            right_opto_accumulator += analogRead(RIGHT_SENSOR_PIN);
                            sample_counter++;     
                        }
                    }
                    else
                    {
                        left_opto_black = left_opto_accumulator/num_samples;
                        middle_opto_black = middle_opto_accumulator/num_samples;
                        right_opto_black = right_opto_accumulator/num_samples;
                        reflectanceCal.left_opto_trip = (float)((left_opto_white + left_opto_black)/2)/MAX_ADC_VALUE;
                        reflectanceCal.middle_opto_trip = (float)((middle_opto_white + middle_opto_black)/2)/MAX_ADC_VALUE;
                        reflectanceCal.right_opto_trip = (float)((right_opto_white + right_opto_black)/2)/MAX_ADC_VALUE;
                        Serial.print("Left Opto Trip: ");
                        Serial.print(reflectanceCal.left_opto_trip, 3);
                        Serial.print(" Middle Opto Trip: ");
                        Serial.print(reflectanceCal.middle_opto_trip, 3);
                        Serial.print(" Right Opto Trip: ");
                        Serial.println(reflectanceCal.right_opto_trip, 3);
                        calState = IDLE;
                        board_led_off();
                    }
                    break;
                default:
                    calState = IDLE;
                    board_led_off();
                    break;
            }
        }
        // Save calibration values to EEPROM memory
        EEPROM.put(REFLECTANCE_CAL_EEPROM_ADDRESS_START, reflectanceCal);
    }
    else
    {
        // EEPROM is programmed with calibration values, so use them
        EEPROM.get(REFLECTANCE_CAL_EEPROM_ADDRESS_START, reflectanceCal);
        Serial.print("Left Opto Trip: ");
        Serial.print(reflectanceCal.left_opto_trip, 3);
        Serial.print(" Middle Opto Trip: ");
        Serial.print(reflectanceCal.middle_opto_trip, 3);
        Serial.print(" Right Opto Trip: ");
        Serial.println(reflectanceCal.right_opto_trip, 3);
    }
    // Write any changes to EEPROM and close interface.
    EEPROM.end();
}

float reflectance_get_left_sensor(void)
{
    return (float)(analogRead(LEFT_SENSOR_PIN)/MAX_ADC_VALUE);
}

float reflectance_get_middle_sensor(void)
{
    return (float)(analogRead(MIDDLE_SENSOR_PIN)/MAX_ADC_VALUE);
}

float reflectance_get_right_sensor(void)
{
    return (float)(analogRead(RIGHT_SENSOR_PIN)/MAX_ADC_VALUE);
}

/*******************************************************************************
 * Function:        int reflectance_line_detect(void)
 *
 * Description:     Sample opto sensors and determine which have detected a line                 
 *
 * Parameters:      None
 *
 * Returns:         Integer (0-7) representing the current status per this table
 * 
 *                  0 (Left: 0, Middle: 0, Right: 0)
 *                  1 (Left: 0, Middle: 0, Right: 1)
 *                  2 (Left: 0, Middle: 1, Right: 0)  // Robot centered over line
 *                  3 (Left: 0, Middle: 1, Right: 1)
 *                  4 (Left: 1, Middle: 0, Right: 0)
 *                  5 (Left: 1, Middle: 0, Right: 1)
 *                  6 (Left: 1, Middle: 1, Right: 0)
 *                  7 (Left: 1, Middle: 1, Right: 1)
 *
 * Side Effects:    None
 *
 * Overview:        Determines which opto sensors are over a line
 *                  0: line not detected
 *                  1: line detected
 *
 * Note:            None
 ******************************************************************************/

int reflectance_get_line_status(void)
{
    int temp = 0;
    // has a line been detected?
    if(reflectance_get_left_sensor() > reflectanceCal.left_opto_trip){
        temp = temp | 4;
    }
    if(reflectance_get_middle_sensor() > reflectanceCal.middle_opto_trip){
        temp = temp | 2;
    }
    if(reflectance_get_right_sensor() > reflectanceCal.right_opto_trip){
        temp = temp | 1;
    }
    return temp;
}

void reflectance_clear_calibration(void)
{
    EEPROM.begin(1024);
    // Erase EEPROM memory to trigger calibration routines during initialization
    for (int i = REFLECTANCE_CAL_EEPROM_ADDRESS_START; i <= REFLECTANCE_CAL_EEPROM_ADDRESS_END; i++) {
      EEPROM.write(i, 255);
    }
    EEPROM.end();
}

