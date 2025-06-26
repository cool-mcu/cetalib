/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            reflectance.cpp
 * Project:         
 * Date:            May 18, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "reflectance" opto-sensor driver interface functions
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

/** Include Files *************************************************************/
#include <Arduino.h>                // Required for Arduino functions
#include <EEPROM.h>                 // EEPROM emulation routines
#include "reflectance.h"            // "reflectance" API declarations
#include "board.h"                  // "board" functions

/*** Symbolic Constants used in this module ***********************************/
#define SERIAL_PORT Serial  // Default to Serial
#if defined(NO_USB)
    #undef SERIAL_PORT
    #define SERIAL_PORT Serial1     // Use Serial1 if USB is disabled
#endif
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
static struct REFLECTANCE_CAL reflectanceCal = {
    .left_opto_trip = LEFT_SENSOR_TRIP_DEFAULT,
    .middle_opto_trip = MIDDLE_SENSOR_TRIP_DEFAULT,
    .right_opto_trip = RIGHT_SENSOR_TRIP_DEFAULT
};

static enum REFLECTANCE_CALIBRATION_STATE calState = IDLE;

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
        SERIAL_PORT.println("Reflectance Sensor Calibration Routine Triggered");
        SERIAL_PORT.println("Position all sensors behind the starting Tee, then Press the USER Switch to begin");
        // EEPROM is blank, perform calibration procedure
        calState = WAIT_BEGIN_WHITE;
        //board_init();
        board_led_pattern(5);
        while (calState != IDLE)
        {
            board_tasks();
            switch(calState)
            {
                case WAIT_BEGIN_WHITE:
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
                            #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
                                middle_opto_accumulator += analogRead(MIDDLE_SENSOR_PIN);
                            #endif
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
                        board_led_pattern(5);   // signal next instruction
                        SERIAL_PORT.println("Position all sensors over the starting Tee, then Press the USER Switch to continue");
                        calState = WAIT_BEGIN_BLACK;
                    }
                    break;
                case WAIT_BEGIN_BLACK:
                    if(board_is_button_pressed())
                    {
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
                            #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
                                middle_opto_accumulator += analogRead(MIDDLE_SENSOR_PIN);
                            #endif
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
                        SERIAL_PORT.print("Left Opto Trip: ");
                        SERIAL_PORT.print(reflectanceCal.left_opto_trip, 3);
                        SERIAL_PORT.print(" Middle Opto Trip: ");
                        SERIAL_PORT.print(reflectanceCal.middle_opto_trip, 3);
                        SERIAL_PORT.print(" Right Opto Trip: ");
                        SERIAL_PORT.println(reflectanceCal.right_opto_trip, 3);
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
        SERIAL_PORT.print("Left Opto Trip: ");
        SERIAL_PORT.print(reflectanceCal.left_opto_trip, 3);
        SERIAL_PORT.print(" Middle Opto Trip: ");
        SERIAL_PORT.print(reflectanceCal.middle_opto_trip, 3);
        SERIAL_PORT.print(" Right Opto Trip: ");
        SERIAL_PORT.println(reflectanceCal.right_opto_trip, 3);
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
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        return (float)(analogRead(MIDDLE_SENSOR_PIN)/MAX_ADC_VALUE);
    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
        return 0.0f;
    #else
        #error Unsupported board selection
    #endif
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

