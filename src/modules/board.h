/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            board.h
 * Project:         
 * Date:            June 24, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * cetalib "board" driver (USER LED, PUSHBUTTON & POTENTIOMETER interface functions)
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH 
 *
 */

#ifndef BOARD_H_
#define BOARD_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "board_interface.h"

/*** Macros *******************************************************************/
#define LED_PIN 14
#define BUTTON_PIN 15
#define POTENTIOMETER_PIN A2
#define LED_PATTERN_INTERVAL 100

/*** Custom Data Types ********************************************************/
typedef enum {OFF, ON} LED_STATE;
typedef enum {DEFAULT, BLINK, PATTERN} LED_FUNCTION_STATE;

/*** Public Function Prototypes ***********************************************/
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

#endif /* BOARD_H_ */
