/*
 * Copyright (C) 2026 dBm Signal Dynamics Inc.
 *
 * File:            joystick.h
 * Project:         
 * Date:            Mar 28, 2026
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "joystick" driver
 * 
 * Starts a local WiFi network and UDP server to process Logitech F310 Switch
 * Data in "D" mode. Requires python UDP client "cetalib-joystick-client.py"
 * 
 * Left and Right Joystick data processing functions are provided to enable
 * "Tank" or "Split-Arcade" Motor Drive Schemes using diffDrive->set_efforts()
 * 
 * See "examples" for example use sketches 
 *
 * Hardware Configurations Supported:
 * 
 * CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
 * (Select Board: "Raspberry Pi Pico W")
 * BUILTIN LED used to indicate AP/Server Status
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select Board: "SparkFun XRP Controller")
 * BUILTIN LED used to indicate AP/Server Status
 *
 * Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
 * (Select "Board = SparkFun XRP Controller (Beta)")
 * NO dedicated LED is available to indicate AP/Server Status.
 * User must use USER LED in their main sketch.
 *
 */

 #ifndef JOYSTICK_H_
 #define JOYSTICK_H_
 
 /*** Include Files ************************************************************/
 #include <Arduino.h>
 #include <stdint.h>
 #include "joystick_interface.h"
 
 /*** Macros *******************************************************************/
#if defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
#define JOYSTICK_STAT_LED_PIN LED_BUILTIN
                                  // Indicates joystick service connection status
                                  // ON:        joystick service is UP
                                  // OFF:       joystick service is disconnected
                                  // Flashing:  joystick service trying to re-start

#endif

/*** Custom Data Types ********************************************************/

/*** Public Function Prototypes ***********************************************/

bool		joystick_init(void);			                  // Initialize/Start joystick AP and UDP server if WiFi is available
void 		joystick_tasks(void);				                // Listen/capture incoming joystick packets
int 		joystick_is_active(void);			              // Has a joystick packet been received
GAMEPAD* 	joystick_get_data(void);				          // Returns a pointer to the latest raw gamepad switch data
float		joystick_get_left_tank_effort(void);		    // Get left "tank drive" effort setting from Left Stick Y
float   joystick_get_right_tank_effort(void);		    // Get right "tank drive" effort setting from Right Stick Y
float 	joystick_get_left_arcade_effort(void);	    // Get left effort "arcade drive" effort setting motor from LeftStickY, RightStickX
float		joystick_get_right_arcade_effort(void);		  // Get right effort "arcade drive" effort setting motor from LeftStickY, RightStickX

#endif /* JOYSTICK_H_ */