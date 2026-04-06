/*
 * Copyright (C) 2026 dBm Signal Dynamics Inc.
 *
 * File:            joystick_interface.h
 * Project:         
 * Date:            Mar 28, 2026
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "joystick" driver interface file - defines "JOYSTICK_INTERFACE" structure
 *
 * Hardware Configurations Supported:
 * 
 * CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
 * (Select "Board = Raspberry Pi Pico W")
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select "Board = SparkFun XRP Controller")
 *
 * Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
 * (Select "Board = SparkFun XRP Controller (Beta)")
 *
 */

 #ifndef JOYSTICK_INTERFACE_H_
 #define JOYSTICK_INTERFACE_H_
 
 /*** Include Files ************************************************************/
 #include <Arduino.h>
 //#include "joystick.h"
 
 /*** Macros *******************************************************************/
 
 /*** Custom Data Types ********************************************************/
 
typedef struct
{
  char    clientIP[32];
  int     clientPort;
  uint8_t gamepadRaw[8];
  uint8_t leftStickX;
  uint8_t leftStickY;
  uint8_t rightStickX;
  uint8_t rightStickY;
  struct
  {
    bool isNeutral;
    bool isNorthPressed;
    bool isEastPressed;
    bool isSouthPressed;
    bool isWestPressed;
  } dPad;
  struct
  {
    bool isAPressed;
    bool isBPressed;
    bool isXPressed;
    bool isYPressed;
  } actionButtons;
  struct 
  {
    bool isLeftTriggerPressed;
    bool isRightTriggerPressed;
  } triggerButtons;
  struct
  {
    bool isLeftBumperPressed;
    bool isRightBumperPressed;
  } bumperButtons;
  bool isBackPressed;
  bool isStartPressed;
} GAMEPAD; 

 struct JOYSTICK_INTERFACE
 {
  bool (*initialize)(void);                   // Initialize/Start joystick AP and UDP server
  void (*tasks)(void);                        // Listen/capture incoming joystick packets
  int (*is_active)(void);			                // Has a joystick packet been received
  GAMEPAD* (*get_data)(void);				          // Returns a pointer to the latest raw gamepad switch data
  float (*get_left_tank_effort)(void);		    // Get left "tank drive" effort setting from Left Stick Y
  float (*get_right_tank_effort)(void);		    // Get right "tank drive" effort setting from Right Stick Y
  float (*get_arcade_throttle_effort)(void);	// Get "throttle" effort for Split-Stick Arcade motor drive from LeftStickY
  float	(*get_arcade_turn_effort)(void);		  // Get "turn" effort for Split-Stick Arcade motor drive from RightStickX 
 };

 #endif /* JOYSTICK_INTERFACE_H_ */