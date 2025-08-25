/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            mqttc_interface.h
 * Project:         
 * Date:            Aug 18, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * "mqttc" driver interface file - defines "MQTTC_INTERFACE" structure
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

#ifndef MQTTC_INTERFACE_H_
#define MQTTC_INTERFACE_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "mqttc.h"

/*** Macros *******************************************************************/

/*** Custom Data Types ********************************************************/

struct MQTTC_INTERFACE
{
  // Connect to the broker and subscribe for all notifications
  bool (*connect)(const char *MySSID, const char *MyPass, const char *MQbroker, int MQport,
                  const char *MQusername, const char *MQpassword, const char *subTopicIDs[],
                  int size_subTopicIDs);    // Connect to WiFi AP & Broker.
  void (*disconnect)(void);                                         // Disconnect from the Broker & AP
  void (*tasks)(void);                                              // Run mqttc background tasks
  void (*send_message)(const char *pubTopic, char *jsonPubPayload); // Publish serialized JSON payload to a topic
  int (*is_message_available)(const char *subTopic);                // Check if JSON message has been received for a specific subscription topic        
  char* (*receive_message)(void);                                   // Retrieve JSON payload for deserialization
};

/*** Public Function Prototypes ***********************************************/


#endif /* MQTTC_INTERFACE_H_ */