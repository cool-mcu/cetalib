/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            oled_interface.h
 * Project:         
 * Date:            Aug 31, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * "oled" text display driver interface file - defines "OLED_INTERFACE" structure
 *
 * Hardware Configuration:
 * 
 * CETA IoT Robot (schematic #14-00069B), based on RPI-Pico-WH,
 * 
 * Uses Adafruit SSD1306-based 128x64 OLED display (#938),
 * connected to "Wire1" instance (I2C1 on pins SDA/GP18 & SCL/GP19) 
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