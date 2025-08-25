/*
  CETALIB "mqttc" Library Example: "mqttc_pub_sub_simple.ino"

  This example configures the robot to exchange data with another MQTT client using
  the public EMQX broker (broker.emqx.io) on port 1883.

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
 
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select "Board = SparkFun XRP Controller")

  Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
  (Select "Board = SparkFun XRP Controller (Beta)")

  created 18 Aug 2025
  by dBm Signal Dynamics Inc.

*/

#include <stdio.h>    // needed for "sprintf()" function
#include <string.h>   // needed for "strcpy()" function
#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// WiFi Parameters
const char ssid[] = "MY_SSID";        // EDIT              
const char pass[] = "MY_PASSPHRASE";  // EDIT            

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
  // Attempt to connect to AP and Broker
  if (!myRobot->mqttc->connect(ssid, pass, MQTTbroker, MQTTport, MQTTusername, MQTTpassword, subscribeTopicIDs, num_subscribeTopicIDs))
  {
    Serial.println("Failed to initialize MQTT Client!. Stopping.");
    myRobot->board->led_blink(10);
    while (1)
    {
      myRobot->board->tasks();
    }
  }
}

void loop() {
  // Run the background tasks
  myRobot->mqttc->tasks();
  myRobot->board->tasks();
  
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