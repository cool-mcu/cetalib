/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            oled.h
 * Project:         
 * Date:            Aug 31, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * cetalib "oled" text display driver interface functions
 *
 * This library depends on the following Libraries:
 * 
 * Adafruit_BusIO Library:   https://github.com/adafruit/Adafruit_BusIO 
 * Adafruit_SSD1306 Library: https://github.com/adafruit/Adafruit_SSD1306 
 * Adafruit-GFX Library:     https://github.com/adafruit/Adafruit-GFX-Library
 * SSD1306Ascii Library:     https://github.com/greiman/SSD1306Ascii 
 * 
 * OLED initialized with the following settings:
 *  - Configured for Adafruit 128x64 OLED (#938)
 *  - Default Font is used (5x7 pixel)
 *  - 7 row x 21 col (147 characters)
 *  - Auto-scrolling enabled
 * 
 * Hardware Configuration:
 * 
 * CETA IoT Robot (schematic #14-00069B), based on RPI-Pico-WH,
 * 
 * Uses Adafruit SSD1306-based 128x64 OLED display (#938),
 * connected to "Wire1" instance (I2C1 on pins SDA/GP18 & SCL/GP19) 
 *
 */

#ifndef OLED_H_
#define OLED_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "oled_interface.h"

/*** Macros *******************************************************************/
#define OLED_SCL_PIN 19
#define OLED_SDA_PIN 18
#define OLED_I2C_ADDRESS 0x3D
#define OLED_I2C_BAUD 400000
#define OLED_SCREEN_WIDTH 128
#define OLED_SCREEN_HEIGHT 64

/*** Custom Data Types ********************************************************/

/*** Public Function Prototypes ***********************************************/
bool oled_init(void);		    // initialize I2C, display splash screen for 5 seconds, cursor in home position
void oled_print(char *s);	  // print up to 21 characters on the current line (auto-truncated)
void oled_println(char *s);	// print up to 21 characters on the current line with CR/LF (auto-truncated)
void oled_clear(void);		  // clear the screen
void oled_home(void);		    // place the cursor in the home position

#endif /* OLED_H_ */