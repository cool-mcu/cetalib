/*
 * Copyright (C) 2026 dBm Signal Dynamics Inc.
 *
 * File:            joystick.h
 * Project:         
 * Date:            June 16, 2026
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "joystick" driver
 * 
 * Starts a UDP server to process Logitech F310 Switch Data in "D" mode.
 * Requires a python UDP client script "cetalib-joystick-client.py"
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
 * NO dedicated LED is available to indicate UDP Server Status.
 * Sketches may use the USER LED in their main sketch.
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select Board: "SparkFun XRP Controller")
 * NO dedicated LED is available to indicate UDP Server Status.
 * Sketches may use the USER LED in their main sketch.
 *
 * Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
 * (Select "Board = SparkFun XRP Controller (Beta)")
 * NO dedicated LED is available to indicate UDP Server Status.
 * Sketches may use the USER LED in their main sketch.
 *
 */

 #ifndef JOYSTICK_H_
 #define JOYSTICK_H_
 
 /*** Include Files ************************************************************/
 #include <Arduino.h>
 #include <stdint.h>
 #include "joystick_interface.h"
 
/*** Macros *******************************************************************/

/*** Custom Data Types ********************************************************/

// Enums for tracking joystick UDP server state
// UDP is stateless so no way to monitor client connection state
// Only reset UDP server on WiFi reconnect
typedef enum {
    JOYSTICK_STATE_SERVER_IS_UP,
    JOYSTICK_STATE_SERVER_IS_DOWN
} JoystickState_t;

/*** Public Function Prototypes ***********************************************/

bool		joystick_init(void);			                  // Initialize/Start joystick UDP server if WiFi is available
void 		joystick_tasks(void);				                // Monitor UDP connection & listen/capture incoming joystick packets
bool 		joystick_is_active(void);			              // Has a joystick packet been received
GAMEPAD* 	joystick_get_data(void);				          // Returns a pointer to the latest raw gamepad switch data
float		joystick_get_left_tank_effort(void);		    // Get left "tank drive" effort setting from Left Stick Y
float   joystick_get_right_tank_effort(void);		    // Get right "tank drive" effort setting from Right Stick Y
float 	joystick_get_left_arcade_effort(void);	    // Get left effort "arcade drive" effort setting motor from LeftStickY, RightStickX
float		joystick_get_right_arcade_effort(void);		  // Get right effort "arcade drive" effort setting motor from LeftStickY, RightStickX

#endif /* JOYSTICK_H_ */