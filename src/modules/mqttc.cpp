/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            mqttc.cpp
 * Project:         
 * Date:            July 13, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * cetalib "mqttc" (MQTT Client) driver interface functions
 *
 * This library uses the "WiFi" library implementation in Arduino-Pico:
 * https://github.com/earlephilhower/arduino-pico/blob/master/libraries/Servo/src/Servo.h
 * 
 * This library also uses the ArduinoMqttClient library:
 * https://github.com/arduino-libraries/ArduinoMqttClient 
 * 
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH  
 *
 */

/** Include Files *************************************************************/
#include <Arduino.h>                // Required for Arduino functions
#include <stdio.h>                  // Required for sprintf()
#include <stddef.h>                 // Defines NULL pointer
#include <string.h>                 // C string functions
#include <ArduinoMqttClient.h>      // Required for MQTT client apis
#include <WiFi.h>                   // Required for WiFi library APIs
#include <WiFiClientSecure.h>       // Required for BearSSL APIs
#include <StackThunk.h>             // Required for BearSSL APIs
#include <time.h>                   // Required for BearSSL APIs
#include "mqttc_certs.h"            // broker root CA certificate
#include "mqttc.h"                  // "mqttc" API declarations

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/

static char mqttcOutBuffer[256];

// mqttc paramters

// WiFi Parameters
static char ssid[64];                                  // WiFi SSID of AP - defined in application layer, passed via "connect()" API                      
static char passPhrase[64];                            // WiFi Passphrase of AP - defined in application layer, passed via "connect()" API 
static byte macAddr[6];                                // WiFi radio IEEE MAC address used to generate ClientID

// TCP Client Connection Parameters 
static char broker[128];                               // IP address or hostname - defined in application layer, passed via "connect()" API
static int port;                                       // Server port number - defined in application layer, passed via "connect()" API
static bool useTLS;                                    // Enable/Disable TLS connection - based on port selection (1883: disable, 8883: enable)
                                                // Note: Server root CA certs defined below

// MQTT Client Session Parameters
static char clientID[64];                              // Dynamically generated during "connect()" function
static char userName[64];                              // MQTT User Name - defined in application layer, passed via "connect()" API
static char userPass[64];                              // MQTT User Password - defined in application layer, passed via "connect()" API

// MQTT Client Publish Parameters
// pub topics defined in application layer, passed via "send_message()" API
static int pubQoS = 0;                                 // Set Publish QoS to level 0 (fire-and-forget)
static bool retained = false;                          // Disable retained message
static bool dup = false;                               // Duplicates not issued with QoS level 0

// MQTT Client Publish Parameters
// sub topics defined in application layer, passed via "connect()" API
static char subTopic[MAX_SUBSCRIBE_TOPIC_IDS][256];    // Array of subscribe topics to be defined
static int subQoS = 0;                                 // Set Subscribe QoS to level 0 (fire-and-forget)
static int subTopicSize;                               // use to save the size of the supplied subscription topic array

// define the function interface
extern const struct MQTTC_INTERFACE MQTTC = {
    .connect                = &mqttc_connect,
    .disconnect             = &mqttc_disconnect,
    .tasks                  = &mqttc_tasks,
    .send_message           = &mqttc_send_message,
    .is_message_available   = &mqttc_is_message_available,
    .receive_message        = &mqttc_receive_message,
};

// create a structure for reception of messages (topic & payload)
static struct MQTTC_RECEIVE_MSG mqttcRxMessage;

// Initialize Socket classes - MQTT Client (for unsecure connections)
WiFiClient wifiClient;                            // Used for TCP Socket connection
MqttClient mqttClient(wifiClient);                // Instantiate an MQTT client having WiFiClient methods

// Create BearSSL client library objects (for secure connections)
BearSSL::WiFiClientSecure secureWifiClient;
MqttClient mqttsClient(secureWifiClient);         // Instantiate an MQTT client having WiFiClientSecure methods
BearSSL::X509List aiocert(adafruitio_root_CA_cert);
BearSSL::X509List hivemqcert(hivemq_root_CA_cert);
BearSSL::X509List mosquittocert(mosquitto_root_CA_cert);


// WiFi & TCP Connection Monitoring Variables ("connectionTasks()" function)
static unsigned long connStatusCurrentSampleTime, connStatusPrevSampleTime;
static const long connStatusSampleInterval = CONN_STATUS_SAMPLE_INTERVAL;    // Network Connection testing interval

/*** Private Function Prototypes **********************************************/
static void wifiConnect(void);                          // Connect to WiFi access network
static void mqttClientConnect(void);                    // Connect to the MQTT broker using an unsecure (TCP) connection
static void mqttsClientConnect(void);                   // Connect to the MQTT broker using a secure (TLS) connection
static void mqttClientDisconnect(void);                 // Disconnect from an insecure MQTT broker connection (TCP) and WiFi AP
static void mqttsClientDisconnect(void);                // Disconnect from a secure MQTT broker connection (TLS) and WiFi AP
static void mqttClientOnMessage(int messageSize);       // Call-back function, processes all subscribed messages from unsecure broker connection
static void mqttsClientOnMessage(int messageSize);      // Call-back function, processes all subscribed messages from secure broker connection
static void connectionTasks(void);                      // Monitor WiFi and TCP connection and reconnect if required (unsecure connection)
static void connectionTasksSecure(void);                // Monitor WiFi and TCP connection and reconnect if required (secure connection)
static void setClock(void);                             // Set time via NTP, as required for x.509 certificate validation

/*** Public Function Definitions **********************************************/

bool mqttc_connect(const char *MySSID, const char *MyPass, const char *MQbroker, int MQport,
                    const char *MQusername, const char *MQpassword, const char *subTopicIDs[],
                    int size_subTopicIDs)
{
  
  // Save SSID & Passphrase
  strcpy(ssid, MySSID);
  strcpy(passPhrase, MyPass);

  // Save the MQTT broker URL, port, MQTT Username and MQTT Password
  strcpy(broker, MQbroker);
  port = MQport;
  strcpy(userName, MQusername);
  strcpy(userPass, MQpassword);

  // Enable/Disable TLS connection based on port selection
  switch(port)
  {
    case 1883:
      useTLS = false;
      break;
    case 8883:
      useTLS = true;
      break;
    default:
      useTLS = true;
      break;
  }
  
  // Initialize MQTTC CONNECTION STATUS LED
  pinMode(MQTTC_STAT_LED_PIN, OUTPUT);     // set digital pin as output
  digitalWrite(MQTTC_STAT_LED_PIN, 0);     // initialize LED state

  // Attempt to connect to Wifi network (blocking code):
  wifiConnect();

  // Create a unique ClientID/Topic Prefix
  sprintf(clientID, "cetaiotrobot-%02x%02x%02x%02x%02x%02x", macAddr[5], macAddr[4], macAddr[3], macAddr[2], macAddr[1], macAddr[0]);
  sprintf(mqttcOutBuffer, "clientID: %s\r\n", clientID);
  Serial.print(mqttcOutBuffer);
  
  // Create/Save the subscription topic list if supplied
  if(!strcmp(subTopicIDs[0], ""))
  {
    // NULL string detected, no subscription topics
    subTopicSize = 0;
  }
  else
  {
    if(size_subTopicIDs > MAX_SUBSCRIBE_TOPIC_IDS)
    {
      Serial.println("Subscription topic list exceeds the limit");
      return false;
    }
    subTopicSize = size_subTopicIDs;
    for(int i=0; i<subTopicSize; i++)
    {
      sprintf(subTopic[i], "%s", subTopicIDs[i]);
    }
  }
  
  // Set the ClientID
  if(useTLS)
  {
    mqttsClient.setId(clientID);
  }
  else
  {
    mqttClient.setId(clientID);
  }
  
  // Set the MQTT Username & Password if defined
  if((userName[0] != '\0') && (userPass[0] != '\0'))
  {
    if(useTLS)
    {
      mqttsClient.setUsernamePassword(userName, userPass);
    }
    else
    {
      mqttClient.setUsernamePassword(userName, userPass);
    }
  }

  // Attempt to connect to Broker (blocking code)
  if(useTLS)
  {
    mqttsClientConnect();
  }
  else
  {
    mqttClientConnect();
  }

  // Set the MQTT subscription receive callback
  if(useTLS)
  {
    mqttsClient.onMessage(mqttsClientOnMessage);
  }
  else
  {
    mqttClient.onMessage(mqttClientOnMessage);
  }

  // Subscribe to the topicID for all defined IN messages
  if(subTopicSize)
  {
    for(int i=0; i < subTopicSize; i++)
    {
      Serial.print("Subscribing to topic: ");
      Serial.println(subTopic[i]);
      if(useTLS)
      {
        mqttsClient.subscribe(subTopic[i], subQoS);
      }
      else
      {
        mqttClient.subscribe(subTopic[i], subQoS);
      }
      Serial.print("Waiting for messages on topic: ");
      Serial.println(subTopic[i]);
      Serial.println();
    }
  }

  // if you get here, you are connected and ready to go!
  digitalWrite(MQTTC_STAT_LED_PIN, 1);

  // Initialize timeout for connectTasks()
  connStatusPrevSampleTime = 0;

  return true;

}

void mqttc_disconnect(void)
{
  if(useTLS)
  {
    mqttsClientDisconnect();
  }
  else
  {
    mqttClientDisconnect();
  }
}

void mqttc_tasks(void)
{
  // Check WiFi & TCP connection status and reconnect if neccesary
  if(useTLS)
  {
    connectionTasksSecure();
  }
  else
  {
    connectionTasks();
  }

  // Call poll() regularly to allow the MqttClientLibrary to receive MQTT messages
  // and sens MQTT keep alive messages which avoids being disconnected by the broker
  if(useTLS)
  {
    mqttsClient.poll();
  }
  else
  {
    mqttClient.poll();
  }
}

void mqttc_send_message(const char *pubTopic, char *jsonPubPayload)
{
  if(useTLS)
  {
    mqttsClient.beginMessage(pubTopic, strlen(jsonPubPayload), retained, pubQoS, dup);
    mqttsClient.print(jsonPubPayload);
    mqttsClient.endMessage();
    //sprintf(mqttcOutBuffer, "pub topic: %s\tpayload: %s\r\n", outTopic, jsonPubPayload);
    //Serial.print(mqttcOutBuffer);
  }
  else
  {
    mqttClient.beginMessage(pubTopic, strlen(jsonPubPayload), retained, pubQoS, dup);
    mqttClient.print(jsonPubPayload);
    mqttClient.endMessage();
    //sprintf(mqttcOutBuffer, "pub topic: %s\tpayload: %s\r\n", outTopic, jsonPubPayload);
    //Serial.print(mqttcOutBuffer);
  }
}

int mqttc_is_message_available(const char *subTopic)
{
  if(0 == strcmp(mqttcRxMessage.inTopic, subTopic))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

char* mqttc_receive_message(void)
{
  static char temp[sizeof(mqttcRxMessage.inPayload)];
  strcpy(temp, mqttcRxMessage.inPayload);
  memset(mqttcRxMessage.inTopic, '\0', sizeof(mqttcRxMessage.inTopic));
  memset(mqttcRxMessage.inPayload, '\0', sizeof(mqttcRxMessage.inPayload));
  return temp;
}

/*** Private Function Definitions *********************************************/

void wifiConnect(void){
    // attempt to connect to Wifi network:
    digitalWrite(MQTTC_STAT_LED_PIN, 0);
    Serial.print("\nAttempting to connect to WPA SSID: ");
    Serial.println(ssid);
    while (WiFi.begin(ssid, passPhrase) != WL_CONNECTED) {
        // failed, retry
        Serial.print(".");
        delay(1000);
        digitalWrite(MQTTC_STAT_LED_PIN, 1);
        delay(100);
        digitalWrite(MQTTC_STAT_LED_PIN, 0);
        delay(100);
    }

    // once you are connected :
    WiFi.macAddress(macAddr);     // read/save the mac address of the radio
    Serial.println("You're connected to the network");
}

void mqttClientConnect(void){
    // attempt insecure connection to MQTT Broker
    digitalWrite(MQTTC_STAT_LED_PIN, 0);
    Serial.print("\nAttempting to connect to the MQTT broker: ");
    Serial.println(broker);
    while(!mqttClient.connect(broker, port)){
        // failed, retry
        Serial.print(".");
        digitalWrite(MQTTC_STAT_LED_PIN, 1);
        delay(100);
        digitalWrite(MQTTC_STAT_LED_PIN, 0);
        delay(100);
        digitalWrite(MQTTC_STAT_LED_PIN, 1);
        delay(100);
        digitalWrite(MQTTC_STAT_LED_PIN, 0);
        delay(100);
    }

    // once you are connected :
    Serial.println("You're connected to the MQTT broker!");
    Serial.println();

}

void mqttClientDisconnect(void)
{
  // disconnect from the broker
  mqttClient.flush();
  mqttClient.stop();
  // disconnect from WiFi AP
  WiFi.end();
  // turn off CONNECTION led
  digitalWrite(MQTTC_STAT_LED_PIN, 0);
}

void mqttsClientConnect(void){
    // attempt secure connection to MQTT Broker
    digitalWrite(MQTTC_STAT_LED_PIN, 0);
    Serial.print("\nAttempting to connect to the MQTT broker: ");
    Serial.println(broker);
    // Select the correct server root CA certificate to use for the TLS connection
    if(strstr(broker, "adafruit"))
    {
      secureWifiClient.setTrustAnchors(&aiocert);
    }
    else if(strstr(broker, "hivemq"))
    {
      secureWifiClient.setTrustAnchors(&hivemqcert);
    }
    else
    {
      // unsupported broker, use mosquitto certificate (connection will fail)
      secureWifiClient.setTrustAnchors(&mosquittocert);
    }
    //secureWifiClient.setTrustAnchors(&cert);
    setClock();
    while(!mqttsClient.connect(broker, port)){
        // failed, retry
        Serial.print(".");
        digitalWrite(MQTTC_STAT_LED_PIN, 1);
        delay(100);
        digitalWrite(MQTTC_STAT_LED_PIN, 0);
        delay(100);
        digitalWrite(MQTTC_STAT_LED_PIN, 1);
        delay(100);
        digitalWrite(MQTTC_STAT_LED_PIN, 0);
        delay(100);
    }

    // once you are connected :
    Serial.println("You're connected to the MQTT broker!");
    Serial.println();

}

void mqttsClientDisconnect(void)
{
  // disconnect from the broker
  mqttsClient.flush();
  mqttsClient.stop();
  // disconnect from WiFi AP
  WiFi.end();
  // turn off CONNECTION led
  digitalWrite(MQTTC_STAT_LED_PIN, 0);
}

// Set time via NTP, as required for x.509 validation
void setClock() {
  NTP.begin("pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  NTP.waitSet([]() {
    Serial.print(".");
  });
  Serial.println("");
  time_t now = time(nullptr);
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void mqttClientOnMessage(int messageSize) {
  
    int i;
    String inTopic;

    inTopic = mqttClient.messageTopic();
    inTopic.toCharArray(mqttcRxMessage.inTopic, sizeof(mqttcRxMessage.inTopic));
    
    // use the Stream interface to save the contents to a char buffer
    i = 0;
    while (mqttClient.available()) {
        mqttcRxMessage.inPayload[i++] = (char)mqttClient.read();
    }
    mqttcRxMessage.inPayload[i] = '\0';

    //sprintf(mqttcOutBuffer, "sub topic: %s\tpayload: %s\r\n", mqttcRxMessage.inTopic, mqttcRxMessage.inPayload);
    //Serial.print(mqttcOutBuffer);
}

void mqttsClientOnMessage(int messageSize) {
  
    int i;
    String inTopic;

    inTopic = mqttsClient.messageTopic();
    inTopic.toCharArray(mqttcRxMessage.inTopic, sizeof(mqttcRxMessage.inTopic));
    
    // use the Stream interface to save the contents to a char buffer
    i = 0;
    while (mqttsClient.available()) {
        mqttcRxMessage.inPayload[i++] = (char)mqttsClient.read();
    }
    mqttcRxMessage.inPayload[i] = '\0';

    //sprintf(mqttcOutBuffer, "sub topic: %s\tpayload: %s\r\n", mqttcRxMessage.inTopic, mqttcRxMessage.inPayload);
    //Serial.print(mqttcOutBuffer);
}

void connectionTasks(void)
{
  connStatusCurrentSampleTime = millis();               // get the current time
  // if 10s has elapsed, check the status of both WiFi and TCP connection and reconnect if required
  if ((connStatusCurrentSampleTime - connStatusPrevSampleTime) >= connStatusSampleInterval)
  {
    connStatusPrevSampleTime = connStatusCurrentSampleTime;
    if(WiFi.status() == WL_CONNECTED)
    {
      Serial.println("WiFi Status: connected");
      if(!mqttClient.connected())
      {
        digitalWrite(MQTTC_STAT_LED_PIN, 0);       // turn off CONNECT status LED
        mqttClient.flush();
        mqttClient.stop();
        Serial.println("TCP Status: disconnected..attempting to reconnect");
        // reconnect to the broker, using the same MQTT Client initialization as in mqttc_connect()..
        mqttClient.setId(clientID);
        mqttClientConnect();
        mqttClient.onMessage(mqttClientOnMessage);
        // Subscribe to the topicID for all defined IN messages
        if(subTopicSize)
        {
          for(int i=0; i < subTopicSize; i++)
          {
            Serial.print("Subscribing to topic: ");
            Serial.println(subTopic[i]);
            mqttClient.subscribe(subTopic[i], subQoS);
            Serial.print("Waiting for messages on topic: ");
            Serial.println(subTopic[i]);
            Serial.println();
          }
        }
        
        connStatusPrevSampleTime = 0;
        digitalWrite(MQTTC_STAT_LED_PIN, 1);       // turn on CONNECT status LED
      }
      else
      {
        Serial.println("TCP Status: connected");
      }
    }
    else
    {
      Serial.println("WiFi Status: disconnected..attempting to reconnect WiFi annd TCP");
      mqttClient.flush();
      mqttClient.stop();
      wifiConnect();
      mqttClient.setId(clientID);
      mqttClientConnect();
      mqttClient.onMessage(mqttClientOnMessage);
      if(subTopicSize)
      {
        for(int i=0; i < subTopicSize; i++)
        {
          Serial.print("Subscribing to topic: ");
          Serial.println(subTopic[i]);
          mqttClient.subscribe(subTopic[i], subQoS);
          Serial.print("Waiting for messages on topic: ");
          Serial.println(subTopic[i]);
          Serial.println();
        }
      }
      
      connStatusPrevSampleTime = 0;
      digitalWrite(MQTTC_STAT_LED_PIN, 1);
    }
  }
}

void connectionTasksSecure(void)
{
  connStatusCurrentSampleTime = millis();               // get the current time
  // if 10s has elapsed, check the status of both WiFi and TCP connection and reconnect if required
  if ((connStatusCurrentSampleTime - connStatusPrevSampleTime) >= connStatusSampleInterval)
  {
    connStatusPrevSampleTime = connStatusCurrentSampleTime;
    if(WiFi.status() == WL_CONNECTED)
    {
      Serial.println("WiFi Status: connected");
      if(!mqttsClient.connected())
      {
        digitalWrite(MQTTC_STAT_LED_PIN, 0);       // turn off CONNECT status LED
        mqttsClient.flush();
        mqttsClient.stop();
        Serial.println("TCP Status: disconnected..attempting to reconnect");
        // reconnect to the broker, using the same MQTT Client initialization as in mqttc_connect()..
        mqttsClient.setId(clientID);
        mqttsClientConnect();
        mqttsClient.onMessage(mqttsClientOnMessage);
        // Subscribe to the topicID for all defined IN messages
        if(subTopicSize)
        {
          for(int i=0; i < subTopicSize; i++)
          {
            Serial.print("Subscribing to topic: ");
            Serial.println(subTopic[i]);
            mqttsClient.subscribe(subTopic[i], subQoS);
            Serial.print("Waiting for messages on topic: ");
            Serial.println(subTopic[i]);
            Serial.println();
          }
        }
        
        connStatusPrevSampleTime = 0;
        digitalWrite(MQTTC_STAT_LED_PIN, 1);       // turn on CONNECT status LED
      }
      else
      {
        Serial.println("TCP Status: connected");
      }
    }
    else
    {
      Serial.println("WiFi Status: disconnected..attempting to reconnect WiFi annd TCP");
      mqttsClient.flush();
      mqttsClient.stop();
      wifiConnect();
      mqttsClient.setId(clientID);
      mqttsClientConnect();
      mqttsClient.onMessage(mqttsClientOnMessage);
      if(subTopicSize)
      {
        for(int i=0; i < subTopicSize; i++)
        {
          Serial.print("Subscribing to topic: ");
          Serial.println(subTopic[i]);
          mqttsClient.subscribe(subTopic[i], subQoS);
          Serial.print("Waiting for messages on topic: ");
          Serial.println(subTopic[i]);
          Serial.println();
        }
      }
      
      connStatusPrevSampleTime = 0;
      digitalWrite(MQTTC_STAT_LED_PIN, 1);
    }
  }
}