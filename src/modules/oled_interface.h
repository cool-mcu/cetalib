/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            oled_interface.h
 * Project:         
 * Date:            Aug 18, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "oled" text display driver interface file - defines "OLED_INTERFACE" structure
 *
 * Tested using Adafruit SSD1306-based 128x64 OLED display (#938)
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

 #ifndef OLED_INTERFACE_H_
 #define OLED_INTERFACE_H_
 
 /*** Include Files ************************************************************/
 #include <Arduino.h>
 
 /*** Macros *******************************************************************/
 
 /*** Custom Data Types ********************************************************/
 struct OLED_INTERFACE
 {
   bool (*initialize)(void);               // initialize I2C, display splash screen for 5 seconds, cursor in home position
   void (*print)(char *s);                 // print up to 21 characters on the current line (auto-truncated)
   void (*println)(char *s);               // print up to 21 characters on the current line with CR/LF (auto-truncated)
   void (*clear)(void);                    // clear the screen
   void (*home)(void);                     // place the cursor in the home position
 };
 
 /*** Public Function Prototypes ***********************************************/
 
 
 #endif /* OLED_INTERFACE_H_ */