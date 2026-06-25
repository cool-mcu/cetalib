/*
 * Copyright (C) 2026 dBm Signal Dynamics Inc.
 *
 * File:            mqttc.cpp
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
#include "network.h"                // "network" APIs used

/*** Symbolic Constants used in this module ***********************************/
#define SERIAL_PORT Serial  // Default to Serial
#if defined(NO_USB)
    #undef SERIAL_PORT
    #define SERIAL_PORT Serial1     // Use Serial1 if USB is disabled
#endif
/*** Global Variable Declarations *********************************************/

static char mqttcOutBuffer[256];

// mqttc paramters
MqttcState_t MqttcState = MQTTC_STATE_DISCONNECTED;
// MQTT Connection Monitoring Variables ("connectionTasks()" function)
static unsigned long MqttcConnStatusCurrentSampleTime, MqttcConnStatusPrevSampleTime;
static const long MqttcConnStatusSampleInterval = MQTTC_CONN_STATUS_SAMPLE_INTERVAL;
static unsigned long MqttcReconnectStartTime;


// WiFi Parameters
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
    .tasks                  = &mqttc_tasks,
    .send_message           = &mqttc_send_message,
    .is_message_available   = &mqttc_is_message_available,
    .receive_message        = &mqttc_receive_message,
    .is_connected           = &mqttc_is_connected,
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
BearSSL::X509List emqxcert(emqx_root_CA_cert);

/*** Private Function Prototypes **********************************************/
static bool mqttClientConnect(void);                    // Connect to the MQTT broker using an unsecure (TCP) connection
static bool mqttsClientConnect(void);                   // Connect to the MQTT broker using a secure (TLS) connection
static void mqttClientOnMessage(int messageSize);       // Call-back function, processes all subscribed messages from unsecure broker connection
static void mqttsClientOnMessage(int messageSize);      // Call-back function, processes all subscribed messages from secure broker connection
static void setClock(void);                             // Set time via NTP, as required for x.509 certificate validation

/*** Public Function Definitions **********************************************/

// mqttc_connect() - only call after connected to a network!

bool mqttc_connect(const char *MQbroker, int MQport,
                    const char *MQusername, const char *MQpassword, const char *subTopicIDs[],
                    int size_subTopicIDs)
{ 
  if(!network_is_ready())
  {
    // Network is not connected, return!
    return false;
  }
  
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

  // Create a unique ClientID
  WiFi.macAddress(macAddr);     // read/save the mac address of the radio
  #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  sprintf(clientID, "cetaiotrobot-%02x%02x%02x%02x%02x%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
  #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
  sprintf(clientID, "xrprobot-%02x%02x%02x%02x%02x%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
  #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
  sprintf(clientID, "xrpbetarobot-%02x%02x%02x%02x%02x%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
  #else
   #error Unsupported board selection
  #endif

  sprintf(mqttcOutBuffer, "\r\nMQTT ClientID: %s\r\n", clientID);
  SERIAL_PORT.print(mqttcOutBuffer);
  
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
      SERIAL_PORT.println("Subscription topic list exceeds the limit");
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

  // Attempt to connect to Broker
  if(useTLS)
  {
    if(!mqttsClientConnect())
    {
      return false; // failed to connect to the desired broker
    }
  }
  else
  {
    if(!mqttClientConnect())
    {
      return false; // failed to connect to the desired broker
    }
  }

  // Set the MQTT subscription receive callback function
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
      SERIAL_PORT.print("Subscribing to topic: ");
      SERIAL_PORT.println(subTopic[i]);
      if(useTLS)
      {
        mqttsClient.subscribe(subTopic[i], subQoS);
      }
      else
      {
        mqttClient.subscribe(subTopic[i], subQoS);
      }
      SERIAL_PORT.print("Waiting for messages on topic: ");
      SERIAL_PORT.println(subTopic[i]);
    }
  }

  // Initialize timeout for socket monitoring in mqttc_tasks()
  MqttcConnStatusPrevSampleTime = 0;

  // MQTT Client is ready to send/receive messages!
  return true;
}

void mqttc_tasks(void)
{
  if(network_is_ready())
  {
    switch(MqttcState)
    {
      case MQTTC_STATE_CONNECTED:
        // Call poll() regularly to allow the MqttClientLibrary to receive MQTT messages
        // and send MQTT keep alive messages which avoids being disconnected by the broker
        if(useTLS)
        {
          mqttsClient.poll();
        }
        else
        {
          mqttClient.poll();
        }
        // Check TCP/TLS connection status and reconnect if neccesary
        MqttcConnStatusCurrentSampleTime = millis();
        if ((MqttcConnStatusCurrentSampleTime - MqttcConnStatusPrevSampleTime) >= MqttcConnStatusSampleInterval)
        {
          MqttcConnStatusPrevSampleTime = MqttcConnStatusCurrentSampleTime;
          if(useTLS)
          { 
            if(!mqttsClient.connected())
            {
              mqttsClient.flush();
              mqttsClient.stop();
              SERIAL_PORT.println();
              SERIAL_PORT.println("[MQTTC Manager] TLS Status: Disconnected...attempting to reconnect");
              // reconnect to the broker, using the same MQTT Client initialization as in mqttc_connect()..
              mqttsClient.setId(clientID);
              // Set the MQTT Username & Password if defined
              if((userName[0] != '\0') && (userPass[0] != '\0'))
              {
                mqttsClient.setUsernamePassword(userName, userPass);
              }
              // Select the correct server root CA certificate to use for the TLS connection
              if(strstr(broker, "adafruit"))
              {
                secureWifiClient.setTrustAnchors(&aiocert);
              }
              else if(strstr(broker, "hivemq"))
              {
                secureWifiClient.setTrustAnchors(&hivemqcert);
              }
              else if(strstr(broker, "emqx"))
              {
                secureWifiClient.setTrustAnchors(&emqxcert);
              }
              else
              {
                // unsupported broker, use mosquitto certificate (connection will fail)
                secureWifiClient.setTrustAnchors(&mosquittocert);
              }
              setClock();
              mqttsClient.connect(broker, port);
              if(mqttsClient.connected())
              {
                SERIAL_PORT.println();
                SERIAL_PORT.println("[MQTTC Manager] TLS Status: Connected to the MQTT broker!");
                MqttcState = MQTTC_STATE_CONNECTED;
                // Subscribe to the topicID for all defined IN messages
                if(subTopicSize)
                {       
                  for(int i=0; i < subTopicSize; i++)
                  {
                    SERIAL_PORT.println();
                    SERIAL_PORT.print("[MQTTC Manager] Subscribing to topic: ");
                    SERIAL_PORT.println(subTopic[i]);
                    mqttsClient.subscribe(subTopic[i], subQoS);
                    SERIAL_PORT.println();
                    SERIAL_PORT.print("[MQTTC Manager] Waiting for messages on topic: ");
                    SERIAL_PORT.println(subTopic[i]);
                  }
                }
                // Set the MQTT subscription receive callback function
                mqttsClient.onMessage(mqttsClientOnMessage);
              }
              else
              {
                SERIAL_PORT.println();
                SERIAL_PORT.println("[MQTTC Manager] TLS Status: Connection failed...attempting to reconnect");
                MqttcState = MQTTC_STATE_ATTEMPTING_CONNECTION;
                MqttcReconnectStartTime = millis();
              }
            }
          }
          else
          {
            if(!mqttClient.connected())
            {
              mqttClient.flush();
              mqttClient.stop();
              SERIAL_PORT.println();
              SERIAL_PORT.println("[MQTTC Manager] TCP Status: Disconnected...attempting to reconnect");
              // reconnect to the broker, using the same MQTT Client initialization as in mqttc_connect()..
              mqttClient.setId(clientID);
              // Set the MQTT Username & Password if defined
              if((userName[0] != '\0') && (userPass[0] != '\0'))
              {
                mqttClient.setUsernamePassword(userName, userPass);
              }
              mqttClient.connect(broker, port);
              if(mqttClient.connected())
              {
                SERIAL_PORT.println();
                SERIAL_PORT.println("[MQTTC Manager] TCP Status: Connected to the MQTT broker!");
                MqttcState = MQTTC_STATE_CONNECTED;
                // Subscribe to the topicID for all defined IN messages
                if(subTopicSize)
                {       
                  for(int i=0; i < subTopicSize; i++)
                  {
                    SERIAL_PORT.println();
                    SERIAL_PORT.print("[MQTTC Manager] Subscribing to topic: ");
                    SERIAL_PORT.println(subTopic[i]);
                    mqttClient.subscribe(subTopic[i], subQoS);
                    SERIAL_PORT.println();
                    SERIAL_PORT.print("[MQTTC Manager] Waiting for messages on topic: ");
                    SERIAL_PORT.println(subTopic[i]);
                  }
                }
                // Set the MQTT subscription receive callback function
                mqttClient.onMessage(mqttClientOnMessage);
              }
              else
              {
                SERIAL_PORT.println();
                SERIAL_PORT.println("[MQTTC Manager] TCP Status: Connection failed...attempting to reconnect");
                MqttcState = MQTTC_STATE_ATTEMPTING_CONNECTION;
                MqttcReconnectStartTime = millis();
              }
            }
          }
        }
        break;

        case MQTTC_STATE_ATTEMPTING_CONNECTION:
          // Retry the connection to the broker non-blockingly every xx seconds
          if((millis() - MqttcReconnectStartTime) > MQTTC_RECONNECT_INTERVAL)
          {
            if(useTLS)
            {
              SERIAL_PORT.println();
              SERIAL_PORT.println("[MQTTC Manager] TLS Status: Attempting to reconnect...");
              // reconnect to the broker, using the same MQTT Client initialization as in mqttc_connect()..
              mqttsClient.setId(clientID);
              // Set the MQTT Username & Password if defined
              if((userName[0] != '\0') && (userPass[0] != '\0'))
              {
                mqttsClient.setUsernamePassword(userName, userPass);
              }
              // Select the correct server root CA certificate to use for the TLS connection
              if(strstr(broker, "adafruit"))
              {
                secureWifiClient.setTrustAnchors(&aiocert);
              }
              else if(strstr(broker, "hivemq"))
              {
                secureWifiClient.setTrustAnchors(&hivemqcert);
              }
              else if(strstr(broker, "emqx"))
              {
                secureWifiClient.setTrustAnchors(&emqxcert);
              }
              else
              {
                // unsupported broker, use mosquitto certificate (connection will fail)
                secureWifiClient.setTrustAnchors(&mosquittocert);
              }
              setClock();
              mqttsClient.connect(broker, port);
              if(mqttsClient.connected())
              {
                SERIAL_PORT.println();
                SERIAL_PORT.println("[MQTTC Manager] TLS Status: Connected to the MQTT broker!");
                MqttcState = MQTTC_STATE_CONNECTED;
                // Subscribe to the topicID for all defined IN messages
                if(subTopicSize)
                {       
                  for(int i=0; i < subTopicSize; i++)
                  {
                    SERIAL_PORT.println();
                    SERIAL_PORT.print("[MQTTC Manager] Subscribing to topic: ");
                    SERIAL_PORT.println(subTopic[i]);
                    mqttsClient.subscribe(subTopic[i], subQoS);
                    SERIAL_PORT.println();
                    SERIAL_PORT.print("[MQTTC Manager] Waiting for messages on topic: ");
                    SERIAL_PORT.println(subTopic[i]);
                  }
                }
                // Set the MQTT subscription receive callback function
                mqttsClient.onMessage(mqttsClientOnMessage);
              }
              else
              {
                SERIAL_PORT.println();
                SERIAL_PORT.println("[MQTTC Manager] TLS Status: Connection failed..attempting to reconnect");
                MqttcReconnectStartTime = millis();
              }
            }
            else
            {
              SERIAL_PORT.println();
              SERIAL_PORT.println("[MQTTC Manager] TCP Status: Attempting to reconnect...");
              // reconnect to the broker, using the same MQTT Client initialization as in mqttc_connect()..
              mqttClient.setId(clientID);
              // Set the MQTT Username & Password if defined
              if((userName[0] != '\0') && (userPass[0] != '\0'))
              {
                mqttClient.setUsernamePassword(userName, userPass);
              }
              mqttClient.connect(broker, port);
              if(mqttClient.connected())
              {
                SERIAL_PORT.println();
                SERIAL_PORT.println("[MQTTC Manager] TCP Status: Connected to the MQTT broker!");
                MqttcState = MQTTC_STATE_CONNECTED;
                // Subscribe to the topicID for all defined IN messages
                if(subTopicSize)
                {       
                  for(int i=0; i < subTopicSize; i++)
                  {
                    SERIAL_PORT.println();
                    SERIAL_PORT.print("[MQTTC Manager] Subscribing to topic: ");
                    SERIAL_PORT.println(subTopic[i]);
                    mqttClient.subscribe(subTopic[i], subQoS);
                    SERIAL_PORT.println();
                    SERIAL_PORT.print("[MQTTC Manager] Waiting for messages on topic: ");
                    SERIAL_PORT.println(subTopic[i]);
                  }
                }
                // Set the MQTT subscription receive callback function
                mqttClient.onMessage(mqttClientOnMessage);
              }
              else
              {
                SERIAL_PORT.println();
                SERIAL_PORT.println("[MQTTC Manager] TCP Status: Connection failed..attempting to reconnect");
                MqttcReconnectStartTime = millis();
              }
            }
          }
          break;

        case MQTTC_STATE_DISCONNECTED:
          // First time here after WiFi access is restored...
          // Trigger state change to "ATTEMPTING_CONNECTION" to restart session
          MqttcState = MQTTC_STATE_ATTEMPTING_CONNECTION;
          MqttcReconnectStartTime = millis();
          break;

        default:
          break;
    }
  }
  else
  {
    if(MqttcState == MQTTC_STATE_CONNECTED)
    {
      // WiFi dropped, so kill the dependent socket
      if(useTLS)
      {
        SERIAL_PORT.println();
        SERIAL_PORT.println("[MQTTC Manager] Interface reported DOWN. Dropping active MQTT session.");
        mqttsClient.flush();
        mqttsClient.stop();
        MqttcState = MQTTC_STATE_DISCONNECTED;
      }
      else
      {
        SERIAL_PORT.println();
        SERIAL_PORT.println("[MQTTC Manager] Interface reported DOWN. Dropping active MQTT session.");
        mqttClient.flush();
        mqttClient.stop();
        MqttcState = MQTTC_STATE_DISCONNECTED;
      }
    }
  }
}

void mqttc_send_message(const char *pubTopic, char *pubPayload)
{
  if(useTLS)
  {
    mqttsClient.beginMessage(pubTopic, strlen(pubPayload), retained, pubQoS, dup);
    mqttsClient.print(pubPayload);
    mqttsClient.endMessage();
    //sprintf(mqttcOutBuffer, "pub topic: %s\tpayload: %s\r\n", outTopic, pubPayload);
    //SERIAL_PORT.print(mqttcOutBuffer);
  }
  else
  {
    mqttClient.beginMessage(pubTopic, strlen(pubPayload), retained, pubQoS, dup);
    mqttClient.print(pubPayload);
    mqttClient.endMessage();
    //sprintf(mqttcOutBuffer, "pub topic: %s\tpayload: %s\r\n", outTopic, pubPayload);
    //SERIAL_PORT.print(mqttcOutBuffer);
  }
}

bool mqttc_is_message_available(const char *subTopic)
{
  if(0 == strcmp(mqttcRxMessage.inTopic, subTopic))
  {
    return true;
  }
  else
  {
    return false;
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

bool mqttc_is_connected(void)
{
  if(MqttcState == MQTTC_STATE_CONNECTED)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/*** Private Function Definitions *********************************************/

bool mqttClientConnect(void){
    // attempt insecure connection to MQTT Broker
    SERIAL_PORT.print("\nAttempting to connect to the MQTT broker: ");
    SERIAL_PORT.println(broker);
    int attempts = 0;
    while((!mqttClient.connect(broker, port)) && attempts < 20)
    {
        // failed, retry
        SERIAL_PORT.print(".");
        delay(500);
        attempts++;
    }
    if(mqttClient.connected())
    {
      SERIAL_PORT.println("You're connected to the MQTT broker!");
      SERIAL_PORT.println();
      MqttcState = MQTTC_STATE_CONNECTED;
      return true;
    }
    else
    {
      SERIAL_PORT.println("\nMQTT Client Connection failed or timed out.");
      MqttcState = MQTTC_STATE_DISCONNECTED;
      return false;
    }
}

bool mqttsClientConnect(void){
    // attempt secure connection to MQTT Broker
    SERIAL_PORT.print("\nAttempting to connect to the MQTT broker: ");
    SERIAL_PORT.println(broker);
    // Select the correct server root CA certificate to use for the TLS connection
    if(strstr(broker, "adafruit"))
    {
      secureWifiClient.setTrustAnchors(&aiocert);
    }
    else if(strstr(broker, "hivemq"))
    {
      secureWifiClient.setTrustAnchors(&hivemqcert);
    }
    else if(strstr(broker, "emqx"))
    {
      secureWifiClient.setTrustAnchors(&emqxcert);
    }
    else
    {
      // unsupported broker, use mosquitto certificate (connection will fail)
      secureWifiClient.setTrustAnchors(&mosquittocert);
    }
    //secureWifiClient.setTrustAnchors(&cert);
    setClock();
    int attempts = 0;
    while((!mqttsClient.connect(broker, port)) && attempts < 20)
    {
        // failed, retry
        SERIAL_PORT.print(".");
        delay(500);
        attempts++;
    }
    if(mqttClient.connected())
    {
      SERIAL_PORT.println("You're connected to the MQTT broker!");
      SERIAL_PORT.println();
      MqttcState = MQTTC_STATE_CONNECTED;
      return true;
    }
    else
    {
      SERIAL_PORT.println("\nMQTT Client Connection failed or timed out.");
      MqttcState = MQTTC_STATE_DISCONNECTED;
      return false;
    }

}

// Set time via NTP, as required for x.509 validation
void setClock() {
  NTP.begin("pool.ntp.org", "time.nist.gov");
  SERIAL_PORT.print("Waiting for NTP time sync: ");
  NTP.waitSet([]() {
    SERIAL_PORT.print(".");
  });
  SERIAL_PORT.println("");
  time_t now = time(nullptr);
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  SERIAL_PORT.print("Current time: ");
  SERIAL_PORT.print(asctime(&timeinfo));
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
    //SERIAL_PORT.print(mqttcOutBuffer);
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
    //SERIAL_PORT.print(mqttcOutBuffer);
}

