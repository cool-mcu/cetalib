/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            imu.cpp
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

/** Include Files *************************************************************/
#include <Arduino.h>                // Required for Arduino functions
#include <stdio.h>                  // Required for sprintf()
#include <EEPROM.h>                 // Required for EEPROM emulation functions
#include <Arduino_LSM6DSOX.h>       // Required for Arduino LSM6DSOX access functions
#include "imu.h"                    // "imu" API declarations
#include "board.h"                  // "board" functions

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/
LSM6DSOXClass CETA_IMU(Wire1, IMU_I2C_ADDRESS);
float temperature, heading;
unsigned long imuTaskCurrentTime, imuTaskPrevTime;
const long imuTaskInterval = (long)IMU_SAMPLE_INTERVAL_MS;

// define an output buffer for sprintf()/Serial.print()
char imuOutBuffer[256];

// initiallize function pointers
extern const struct CETA_IMU_INTERFACE CETA_IMU_IF = {
    .initialize             = &imu_init,
    .tasks                  = &imu_tasks,
    .get_temperature        = &imu_get_temperature,
    .get_heading            = &imu_get_heading,
    .reset_heading          = &imu_reset_heading,
    .clear_calibration      = &imu_clear_calibration
};

// calibration object with default values assigned
struct IMU_CAL imuCal = {
    .yaw_offset_error = IMU_YAW_OFFSET_ERROR_DEFAULT,
    .yaw_gain_coefficient = IMU_YAW_GAIN_COEFFICIENT_DEFAULT
};

enum IMU_CALIBRATION_STATE imuCalState = IMU_CAL_IDLE;

/*** Private Function Prototypes **********************************************/

/*** Public Function Definitions **********************************************/

bool imu_init(void)
{
    // calibration variables
    float calYawAccumulator = 0.0f;
    float calHeading = 0.0f;
    float x, y, z;
    int sample_counter = 0;

    // initialize I2C connection to the IMU
    Wire1.setSDA(IMU_SDA_PIN);
    Wire1.setSCL(IMU_SCL_PIN);
    Wire1.setClock(IMU_I2C_BAUD);
    if (!CETA_IMU.begin())
    {
        return false;
    }
    heading = 0.0f;

    // Perform imu calibration if EEPROM calibration memory is blank
    EEPROM.begin(1024);
    uint32_t testRead = 0;
    if(EEPROM.get(IMU_CAL_EEPROM_ADDRESS_START, testRead) == 0xFFFFFFFF)
    {
        Serial.println("IMU Calibration Routine Triggered");
        // EEPROM is blank, perform calibration procedure
        imuCalState = IMU_CAL_WAIT_BEGIN;
        board_led_pattern(5);
        while (imuCalState != IMU_CAL_IDLE)
        {
            board_tasks();
            switch(imuCalState)
            {
                case IMU_CAL_WAIT_BEGIN:
                  if(board_is_button_pressed())
                  {
                    board_led_pattern(1);                               // indicate "calibrate yaw offset error" state
                    imuCalState = IMU_CAL_HEADING_OFFSET_ERROR;         // compute the yaw offset error
                    calYawAccumulator = 0.0f;
                    sample_counter = 0;
                  }
                  break;
                case IMU_CAL_HEADING_OFFSET_ERROR:
                  if(board_is_button_pressed())
                  {
                    imuCal.yaw_offset_error = calYawAccumulator / sample_counter;
                    //board_led_off();
                    board_led_pattern(2);
                    //imuCalState = IMU_CAL_IDLE;
                    imuCalState = IMU_CAL_HEADING_GAIN;
                    calHeading = 0.0f;
                  }
                  else
                  {
                    imuTaskCurrentTime = millis();
                    if ((imuTaskCurrentTime - imuTaskPrevTime) >= imuTaskInterval)
                    {
                      imuTaskPrevTime = imuTaskCurrentTime;
                      if(CETA_IMU.gyroscopeAvailable())
                      {
                        CETA_IMU.readGyroscope(x, y, z);
                        calYawAccumulator += z;
                        sample_counter++;
                      }
                    }    
                  }
                  break;
                case IMU_CAL_HEADING_GAIN:
                  if(board_is_button_pressed())
                  {
                    imuCal.yaw_gain_coefficient = 90.0 / calHeading;
                    board_led_off();
                    imuCalState = IMU_CAL_IDLE;
                  }
                  else
                  {
                    imuTaskCurrentTime = millis();
                    if ((imuTaskCurrentTime - imuTaskPrevTime) >= imuTaskInterval)
                    {
                      imuTaskPrevTime = imuTaskCurrentTime;
                      if(CETA_IMU.gyroscopeAvailable())
                      {
                        CETA_IMU.readGyroscope(x, y, z);
                        z -= imuCal.yaw_offset_error;
                        calHeading += (z*IMU_SAMPLE_INTERVAL_S);
                      }
                    }    
                  }
                  break;  
                default:
                  imuCalState = IMU_CAL_IDLE;
                  board_led_off();
                  break;
            }
        }
        // Save calibration values to EEPROM memory
        EEPROM.put(IMU_CAL_EEPROM_ADDRESS_START, imuCal);
        sprintf(imuOutBuffer, "\r\nIMU Heading Offset Error: %f\tIMU Heading Gain Error: %f\r\n\r\n", imuCal.yaw_offset_error, imuCal.yaw_gain_coefficient);
        Serial.print(imuOutBuffer);
    }
    else
    {
      // EEPROM is programmed with calibration values, so use them
        EEPROM.get(IMU_CAL_EEPROM_ADDRESS_START, imuCal);
        sprintf(imuOutBuffer, "\r\nIMU Heading Offset Error: %f\tIMU Heading Gain Error: %f\r\n\r\n", imuCal.yaw_offset_error, imuCal.yaw_gain_coefficient);
        Serial.print(imuOutBuffer);
    }

    // Write any changes to EEPROM and close interface.
    EEPROM.end();

    return true;
}

void imu_tasks(void)
{
  float x, y, z;
  imuTaskCurrentTime = millis();
  if ((imuTaskCurrentTime - imuTaskPrevTime) >= imuTaskInterval)
  {
    imuTaskPrevTime = imuTaskCurrentTime;
    
    if(CETA_IMU.temperatureAvailable())
    {
      CETA_IMU.readTemperatureFloat(temperature);
    }
    
    if(CETA_IMU.gyroscopeAvailable())
    {
      CETA_IMU.readGyroscope(x, y, z);
      z -= imuCal.yaw_offset_error;
      if(fabsf(z) > (imuCal.yaw_offset_error*10))
      {
        heading += (z*IMU_SAMPLE_INTERVAL_S);
      }
      //sprintf(imuOutBuffer, "pitch: %f\troll: %f\tyaw: %f\r\n", x, y, z);
      //Serial.print(imuOutBuffer);
    }

  }

}

float imu_get_temperature(void)
{
  return temperature;
}

float imu_get_heading(void)
{
  return (heading*imuCal.yaw_gain_coefficient);
}

void imu_reset_heading(void)
{
  heading = 0.0f;
}

void imu_clear_calibration(void)
{
    EEPROM.begin(1024);
    // Erase EEPROM memory to trigger calibration routines during initialization
    for (int i = IMU_CAL_EEPROM_ADDRESS_START; i <= IMU_CAL_EEPROM_ADDRESS_END; i++) {
      EEPROM.write(i, 255);
    }
    EEPROM.end();
}