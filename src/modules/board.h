/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            board.h
 * Project:         
 * Date:            Mar 29, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "board" driver (USER LED, PUSHBUTTON & POTENTIOMETER interface functions)
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

 #ifndef BOARD_H_
 #define BOARD_H_
 
 /*** Include Files ************************************************************/
 #include <Arduino.h>
 #include "board_interface.h"
 
 /*** Macros *******************************************************************/
 
 #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
   #define LED_PIN 14
   #define BUTTON_PIN 15
   #define POTENTIOMETER_PIN A2
   #define LED_PATTERN_INTERVAL 100
 #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
   #define LED_PIN LED_BUILTIN
   #define BUTTON_PIN 16
   #define LED_PATTERN_INTERVAL 100
 #else
   #error Unsupported board selection
 #endif
 
 /*** Custom Data Types ********************************************************/
 typedef enum {OFF, ON} LED_STATE;
 typedef enum {DEFAULT, BLINK, PATTERN} LED_FUNCTION_STATE;
 
 /*** Public Function Prototypes ***********************************************/
 
 #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
 void board_init(void);                  // Initiallize pins & state variables
 void board_tasks(void);                 // Run all background tasks
 void board_led_on(void);                // Turn on USER LED
 void board_led_off(void);               // Turn off USER LED
 void board_led_toggle(void);            // Toggle USER LED
 void board_led_blink(int frequency);    // Blink the USER LED at a rate of 1-20 Hz
 void board_led_pattern(int pattern);    // Flash a specific pattern on the USER LED (1-5)
 bool board_is_button_pressed(void);     // Did we detect a pushbutton press?
 bool board_is_button_released(void);    // Did we detect a pushbutton release?
 int board_get_button_level(void);       // Get current pushbutton level
 void board_wait_for_button(void);       // Careful - this is a blocking function!!!
 int board_get_potentiometer(void);      // Get current potentiometer reading (0-4095)
 #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
 void board_init(void);                  // Initiallize pins & state variables
 void board_tasks(void);                 // Run all background tasks
 void board_led_on(void);                // Turn on USER LED
 void board_led_off(void);               // Turn off USER LED
 void board_led_toggle(void);            // Toggle USER LED
 void board_led_blink(int frequency);    // Blink the USER LED at a rate of 1-20 Hz
 void board_led_pattern(int pattern);    // Flash a specific pattern on the USER LED (1-5)
 bool board_is_button_pressed(void);     // Did we detect a pushbutton press?
 bool board_is_button_released(void);    // Did we detect a pushbutton release?
 int board_get_button_level(void);       // Get current pushbutton level
 void board_wait_for_button(void);       // Careful - this is a blocking function!!!
 #else
   #error Unsupported board selection
 #endif
 
 #endif /* BOARD_H_ */
 