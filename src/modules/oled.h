/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            oled.h
 * Project:         
 * Date:            Aug 18, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "oled" text display driver interface functions
 *
 * Tested using Adafruit SSD1306-based 128x64 OLED display (#938)
 * 
 * Dependencies:
 * 
 * Adafruit_BusIO Library:   https://github.com/adafruit/Adafruit_BusIO 
 * Adafruit_SSD1306 Library: https://github.com/adafruit/Adafruit_SSD1306 
 * Adafruit-GFX Library:     https://github.com/adafruit/Adafruit-GFX-Library
 * SSD1306Ascii Library:     https://github.com/greiman/SSD1306Ascii
 * "Cool-MCU.com" Bitmap generated using this website: https://javl.github.io/image2cpp
 * 
 * OLED initialized with the following settings:
 *  - Configured for Adafruit 128x64 OLED (#938)
 *  - Default Font is used (5x7 pixel)
 *  - 7 row x 21 col (147 characters)
 *  - Auto-scrolling enabled
 * 
 * Hardware Configurations Supported:
 * 
 * CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
 * (Select "Board = Raspberry Pi Pico W")
 * (Uses "Wire1" instance (I2C1 on pins SDA/GP18 & SCL/GP19))
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select "Board = SparkFun XRP Controller")
 * (Uses "Wire" instance (I2C0 on pins SDA/GP04 & SCL/GP05))
 *
 * Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
 * (Select "Board = SparkFun XRP Controller (Beta)")
 * (Uses "Wire1" instance (I2C1 on pins SDA/GP18 & SCL/GP19))
 *
 */

 #ifndef OLED_H_
 #define OLED_H_
 
 /*** Include Files ************************************************************/
 #include <Arduino.h>
 #include "oled_interface.h"
 
 /*** Macros *******************************************************************/
 
 #if defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
   #define OLED_SCL_PIN 19
   #define OLED_SDA_PIN 18
 #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
   #define OLED_SCL_PIN 5
   #define OLED_SDA_PIN 4
 #else
   #error Unsupported board selection
 #endif
 
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