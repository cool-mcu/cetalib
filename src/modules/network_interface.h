/*
 * Copyright (C) 2026 dBm Signal Dynamics Inc.
 *
 * File:            network_interface.h
 * Project:         
 * Date:            June 07, 2026
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "network" driver interface file - defines "NETWORK_INTERFACE" structure
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

#ifndef NETWORK_INTERFACE_H_
#define NETWORK_INTERFACE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>

/*** Macros *******************************************************************/

/*** Custom Data Types ********************************************************/
struct NETWORK_INTERFACE
{
  bool (*connect)(void);                      // Attempt to connect using saved WiFi credentials from file
  void (*provision)(void);                    // (Blocking) Execute WiFI Provisioning service on 192.168.4.1
  void (*tasks)(void);                        // Network Manager: Monitors/updates WiFi connection state, and attempts to re-connect using stored parameters if needed
                                              // Monitors USER SWITCH - if pressed for 3 seconds, delete wifi_cred.txt and reboot system
  bool (*is_ready)(void);                     // Is the WiFi STA connected to the AP?
  char* (*get_IPAddr)(void);                  // Returns a pointer to the WiFi STA IP Address as a C string
  void (*reset_connection)(void);             // Deletes "wifi_cred.txt" and reboots the system
};

/*** Public Function Prototypes ***********************************************/


#endif /* NETWORK_INTERFACE_H_ */