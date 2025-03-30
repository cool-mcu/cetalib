/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            board_interface.h
 * Project:         
 * Date:            Mar 29, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "board" driver interface file - defines "BOARD_INTERFACE" structure
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

 #ifndef BOARD_INTERFACE_H_
 #define BOARD_INTERFACE_H_
 
 /*** Include Files ************************************************************/
 #include <Arduino.h>
 
 /*** Macros *******************************************************************/
 
 /*** Custom Data Types ********************************************************/
 
 #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
 struct BOARD_INTERFACE
 {
   void (*initialize)(void);             // Initiallize pins & state variables
   void (*tasks)(void);                  // Run all background tasks
   void (*led_on)(void);                 // Turn on USER LED
   void (*led_off)(void);                // Turn off USER LED
   void (*led_toggle)(void);             // Toggle USER LED
   void (*led_blink)(int frequency);     // Blink the USER LED at a rate of 1-20 Hz
   void (*led_pattern)(int pattern);     // Flash a specific pattern on the USER LED (1-5)
   bool (*is_button_pressed)(void);      // Did we detect a pushbutton press?
   bool (*is_button_released)(void);     // Did we detect a pushbutton release?
   int (*get_button_level)(void);        // Get current pushbutton level
   void (*wait_for_button)(void);        // Careful! This is a blocking function
   int (*get_potentiometer)(void);       // Get current potentiometer reading (0-4095)
 };
 #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
 struct BOARD_INTERFACE
 {
   void (*initialize)(void);             // Initiallize pins & state variables
   void (*tasks)(void);                  // Run all background tasks
   void (*led_on)(void);                 // Turn on USER LED
   void (*led_off)(void);                // Turn off USER LED
   void (*led_toggle)(void);             // Toggle USER LED
   void (*led_blink)(int frequency);     // Blink the USER LED at a rate of 1-20 Hz
   void (*led_pattern)(int pattern);     // Flash a specific pattern on the USER LED (1-5)
   bool (*is_button_pressed)(void);      // Did we detect a pushbutton press?
   bool (*is_button_released)(void);     // Did we detect a pushbutton release?
   int (*get_button_level)(void);        // Get current pushbutton level
   void (*wait_for_button)(void);        // Careful! This is a blocking function
 };
 #else
   #error Unsupported board selection
 #endif
 
 /*** Public Function Prototypes ***********************************************/
 
 
 #endif /* BOARD_INTERFACE_H_ */
 