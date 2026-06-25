/*
 * Copyright (C) 2026 dBm Signal Dynamics Inc.
 *
 * File:            network.h
 * Project:         
 * Date:            June 07, 2026
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "network" driver interface functions
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
 */

#ifndef NETWORK_H_
#define NETWORK_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "network_interface.h"

/*** Macros *******************************************************************/

// WiFi connection status LED
// NOTE: On XRP Beta, this LED is only used during "connect()"" and "provision()" 
// after which it is used as the USER LED (i.e not used in "tasks()")
#define NETWORK_STAT_LED_PIN LED_BUILTIN
                                  // Indicates network connection status
                                  // ON:        connected to AP & IP Addr assigned
                                  // OFF:       disconnected
                                  // Flashing:  trying to re/connect


// Hardware Reset Button Configuration
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  #define RESET_BUTTON_PIN 15
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
  #define RESET_BUTTON_PIN 36
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
  #define RESET_BUTTON_PIN 22
#else
  #error Unsupported board selection
#endif

/*** Custom Data Types ********************************************************/

// Enums for tracking network state
typedef enum {
    NETWORK_STATE_DISCONNECTED,
    NETWORK_STATE_ATTEMPTING_CONNECTION,
    NETWORK_STATE_CONNECTED
} NetworkState_t;

/*** Public Function Prototypes ***********************************************/
bool network_connect(void);           // Attempt to connect using saved WiFi credentials from file
void network_provision(void);         // (Blocking) Execute WiFI Provisioning service on 192.168.4.1
void network_tasks(void);             // Network Manager: Monitors/updates WiFi connection state, and attempts to re-connect using stored parameters if needed
                                      // Monitors USER SWITCH - if pressed for 3 seconds, delete wifi_cred.txt and reboot system
bool network_is_ready(void);          // Is the WiFi STA connected to the AP?
char* network_get_IPAddr(void);       // Returns a pointer to the WiFi STA IP Address as a C string
void network_reset_connection(void);  // Deletes "wifi_cred.txt" and reboots the system

#endif /* NETWORK_H_ */