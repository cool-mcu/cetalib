/*
  CETALIB "mqttc" Library Example: "mqttc_pub_sub_simple.ino"

  This example configures the robot to exchange data with another MQTT client using
  the public EMQX broker (broker.emqx.io) on port 1883.

  This application uses the "network" module to provision/connect to a WiFi network.
  Make sure to select "Tools -> Flash Size" and select a 64kB Flash partition for 
  the filesystem.
  
  Please review the documentation for the network module to understand how WiFi
  provisioning works (https://github.com/cool-mcu/cetalib). 

  The robot publishes the Left Opto Sensor value to the broker every second.
  The robot subscribes to receive 2 messages from the broker:
    - an "led control" message ("TOGGLE") which will toggle the state of the led
    - a "status message" which will be displayed on the serial terminal

  Use any MQTT Client app to interact with this demo, for example:
    - MQTTX (https://mqttx.app/)
    - IoT MQTT Panel App (download from Google Play or Apple App Store)
  
  Hardware Configurations Supported:

  CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
  (Select "Board = Raspberry Pi Pico W")
  NOTE: LED_BUILTIN and Serial Monitor are used to display WiFi Provisioning status
 
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select "Board = SparkFun XRP Controller")
  NOTE: LED_BUILTIN and Serial Monitor are used to display WiFi Provisioning status

  Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
  (Select "Board = SparkFun XRP Controller (Beta)")
  NOTE: LED_BUILTIN is used to display WiFi Provisioning Status only during setup()
        Serial Monitor may be used at all times to display WiFi Provisioning status

  updated 07 Jun 2026
  by dBm Signal Dynamics Inc.

*/

#include <stdio.h>    // needed for "sprintf()" function
#include <string.h>   // needed for "strcpy()" function
#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;           

// MQTT Broker URL, Username, Password
const char MQTTbroker[] = "broker.emqx.io";
int MQTTport = 1883;    // EDIT: 1883 for open connection, or 8883 for secure connection
const char MQTTusername[] = "";
const char MQTTpassword[] = "";

// MQTT publish topics and payload buffer
const char leftOptoTopic[] = "CETAIoTRobot/out/leftOptoValue";

// A payload buffer to store the publish payload messages
char pubPayload[32];

// Array of MQTT subscribe topics (maximum of 10 or define "" for none)
const char ledControlTopic[] = "CETAIoTRobot/in/ledControl";
const char statusMessageTopic[] = "CETAIoTRobot/in/statusMessage";
const char *subscribeTopicIDs[] = {ledControlTopic, statusMessageTopic};

// A payload buffer to store the received subscription messages
char subPayload[256];

// Calculate the number of subscribe topics
int num_subscribeTopicIDs = sizeof(subscribeTopicIDs)/sizeof(subscribeTopicIDs[0]);

// Define opto sensor sample interval variables
unsigned long leftOptoSampleCurrentTime, leftOptoSamplePrevTime;
const long leftOptoSampleInterval = 1000; // (sample interval in mS)

// Define a serial terminal output buffer for messages
char serialOutBuffer[256];

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  myRobot->reflectance->initialize();
  myRobot->network->initialize();
  // Provision the WiFi connection
  if (0 == myRobot->board->get_button_level())
  {
    myRobot->network->clear_credentials();
  }
  if (!myRobot->network->connect())
  {
    Serial.println("Failed to connect to AP. WiFi provisioning started...");
    myRobot->network->provision();
  }
  Serial.println("\nConnected to the Network...");
  // Attempt to connect to the MQTT Broker
  if (!myRobot->mqttc->connect(MQTTbroker, MQTTport, MQTTusername, MQTTpassword, subscribeTopicIDs, num_subscribeTopicIDs))
  {
    Serial.println();
    Serial.println("Failed to initialize MQTT Client!");
    myRobot->board->led_blink(10);
    while (1)
    {
      // blink the USER LED to indicate joystick error
      myRobot->board->tasks();
    }
  }
  Serial.println("\nConnected to the Broker...\n");
}

void loop() {
  // Run the background tasks
  myRobot->network->tasks();
  myRobot->mqttc->tasks();
  
  // Exchange data with the Broker if connected
  if(myRobot->mqttc->is_connected())
  {
    // Publish the Left Opto Sensor value every second
    leftOptoSampleCurrentTime = millis();
    if ((leftOptoSampleCurrentTime - leftOptoSamplePrevTime) >= leftOptoSampleInterval)
    {
      leftOptoSamplePrevTime = leftOptoSampleCurrentTime;
      sprintf(pubPayload, "%4.3f", myRobot->reflectance->get_left_sensor());
      myRobot->mqttc->send_message(leftOptoTopic, pubPayload);
    }

    // Poll/Process an led control message
    if (myRobot->mqttc->is_message_available(ledControlTopic))
    {
      strcpy(subPayload, myRobot->mqttc->receive_message());
      if (0 == strcmp(subPayload, "TOGGLE"))
      {
        myRobot->board->led_toggle();
        Serial.println("USER LED TOGGLED");
      }
    }
    // Poll/Process a status message
    if (myRobot->mqttc->is_message_available(statusMessageTopic))
    {
      strcpy(subPayload, myRobot->mqttc->receive_message());
      sprintf(serialOutBuffer, "Received Message: %s", subPayload);
      Serial.println(serialOutBuffer);
    }
  }
}