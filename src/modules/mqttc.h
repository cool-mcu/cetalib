/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            mqttc.h
 * Project:         
 * Date:            July 25, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "mqttc" (MQTT Client) driver interface functions
 *
 * This library uses the "WiFi" library implementation in Arduino-Pico:
 * https://github.com/earlephilhower/arduino-pico/blob/master/libraries/Servo/src/Servo.h
 * 
 * This library also uses the ArduinoMqttClient library:
 * https://github.com/arduino-libraries/ArduinoMqttClient 
 * 
 * Hardware Configurations Supported:
 * 
 * CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
 * (Select Board: "Raspberry Pi Pico W")
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select Board: "SparkFun XRP Controller")
 *
 */

#ifndef MQTTC_H_
#define MQTTC_H_

/*** Include Files ************************************************************/
#include <Arduino.h>
#include "mqttc_interface.h"

/*** Macros *******************************************************************/
#define MQTTC_STAT_LED_PIN LED_BUILTIN // DIGITAL OUTPUT - connected to Pico-WH on-board LED
                                  // Indicates mqttc connection status
                                  // ON:        connected to MQTT broker
                                  // OFF:       disconnected
                                  // Flashing:  trying to re/connect

#define MAX_SUBSCRIBE_TOPIC_IDS 10 // Max number of subscribe topics to be defined
#define CONN_STATUS_SAMPLE_INTERVAL 30000 // Connection status sampling nterval (in mS)

/*** Custom Data Types ********************************************************/

struct MQTTC_RECEIVE_MSG
{
  char inTopic[128];   // topic ID for most recently received subscription message
  char inPayload[128]; // message payload for most recently received subscription message
};


/*** Public Function Prototypes ***********************************************/
bool  mqttc_connect(const char *MySSID, const char *MyPass, const char *MQbroker, int MQport,
                    const char *MQusername, const char *MQpassword, const char *subTopicIDs[],
                    int size_subTopicIDs);   // Connect to WiFi AP & Broker.
void  mqttc_disconnect(void);                                           // Disconnect from the Broker & AP
void  mqttc_tasks(void);                                                // Run mqttc background tasks
void  mqttc_send_message(const char *pubTopic, char *jsonPubPayload);   // Publish serialized JSON payload to a topic
int   mqttc_is_message_available(const char *subTopic);                 // Check if JSON message has been received for a specific subscription topic        
char* mqttc_receive_message(void);                                      // Retrieve JSON payload for deserialization

#endif /* MQTTC_H_ */