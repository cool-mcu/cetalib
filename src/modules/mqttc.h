/*
 * Copyright (C) 2026 dBm Signal Dynamics Inc.
 *
 * File:            mqttc.h
 * Project:         
 * Date:            June 12, 2026
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "mqttc" (MQTT Client) driver interface functions
 * 
 * This library uses the "WiFi" library implementation in Arduino-Pico:
 * https://github.com/earlephilhower/arduino-pico/tree/master/libraries/WiFi/src
 * 
 * This library uses the ArduinoMqttClient library:
 * https://github.com/arduino-libraries/ArduinoMqttClient 
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

#ifndef MQTTC_H_
#define MQTTC_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "mqttc_interface.h"

/*** Macros *******************************************************************/

#define MAX_SUBSCRIBE_TOPIC_IDS 10 // Max number of subscribe topics to be defined
#define MQTTC_CONN_STATUS_SAMPLE_INTERVAL 5000 // Connection status sampling interval (in mS)
#define MQTTC_RECONNECT_INTERVAL 5000 //  Broker Reconnect interval

/*** Custom Data Types ********************************************************/

struct MQTTC_RECEIVE_MSG
{
  char inTopic[128];   // topic ID for most recently received subscription message
  char inPayload[128]; // message payload for most recently received subscription message
};

// Enums for tracking mqtt client state
typedef enum {
    MQTTC_STATE_DISCONNECTED,
    MQTTC_STATE_ATTEMPTING_CONNECTION,
    MQTTC_STATE_CONNECTED
} MqttcState_t;


/*** Public Function Prototypes ***********************************************/
bool  mqttc_connect(const char *MQbroker, int MQport,
                    const char *MQusername, const char *MQpassword, const char *subTopicIDs[],
                    int size_subTopicIDs);   // Connect to WiFi AP & Broker.
void  mqttc_tasks(void);                                                // Run mqttc background tasks
void  mqttc_send_message(const char *pubTopic, char *pubPayload);   // Publish a character string payload to a topic
bool  mqttc_is_message_available(const char *subTopic);                 // Check if message string has been received for a specific subscription topic            
char* mqttc_receive_message(void);                                      // Retrieve message string payload for processing
bool  mqttc_is_connected(void);                                         // Check if MQTT Session is active

#endif /* MQTTC_H_ */