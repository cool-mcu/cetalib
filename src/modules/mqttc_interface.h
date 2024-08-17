/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            mqttc_interface.h
 * Project:         
 * Date:            July 13, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * "mqttc" driver interface file - defines "MQTTC_INTERFACE" structure
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH 
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