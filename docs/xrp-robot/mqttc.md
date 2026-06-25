# mqttc Module

This module provides functions to create and manage a **MQTT Client** connection on the [SparkFun XRP Robot](https://www.sparkfun.com/experiential-robotics-platform-xrp-kit.html)..

<center><img src="../../assets/xrp-mqttc.png?raw=true"></center><br>

**NOTE:** This module requires the [Network Module APIs](https://github.com/cool-mcu/cetalib/blob/main/docs/xrp-robot/network.md) to manage the WiFi connection to the network. Before compilation, you will need to allocate 64kB of Flash memory to support a Filesystem as shown here:

<center><img src="../../assets/xrp_filesystem_allocate.jpg?raw=true"></center><br>

For detailed lessons covering networking concepts, the MQTT protocol, schematics and step-by-step robot assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [CETA Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/ceta-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:

* [connect()](#bool-connectconst-char-mqbroker-int-mqport-const-char-mqusername-const-char-mqpassword-const-char-subtopicids-int-size_subtopicids)
* [tasks()](<#void-tasksvoid>)
* [is_connected()](#bool-is_connectedvoid)
* [send_message()](#void-send_messageconst-char-pubtopic-char-pubpayload)
* [is_message_available()](#bool-is_message_availableconst-char-subtopic)
* [receive_message()](#char-receive_messagevoid)

## `bool connect(const char *MQbroker, int MQport, const char *MQusername, const char *MQpassword, const char *subTopicIDs[], int size_subTopicIDs)`

Open an MQTT Client network connection to an MQTT Broker, then subscribe to all required Topics. <br></br>Secure connections (port 8883) are currently supported with **Adafruit IO** (io.adafruit.com), **EMQX Broker** (broker.emqx.io), **HiveMQ** (broker.hivemq.com), and **Mosquitto** (test.mosquitto.org) brokers.

### Syntax

```c
myRobot->mqttc->connect(...);
```
### Parameters

* **const char \*MQbroker**: MQTT Broker name or IP address
* **int MQport**: MQTT Broker port identifier (1883 or 8883)
* **const char \*MQusername**: MQTT Broker Username (if required)
* **const char \*MQpassword**: MQTT Broker Password (if required)
* **const char \*subTopicIDs[]**: Array of subscriber topics to subscribe to when connected
* **int size_subTopicIDs**: Number of subscriber topics in the array

### Returns

* **bool**: TRUE if succesfully connected, FALSE if an error is detected in the parameter list.

### Notes

* This function requires the [Network Module APIs](https://github.com/cool-mcu/cetalib/blob/main/docs/xrp-robot/network.md) to provision/connect to the WiFi network before use.
  * Make sure you allocate 64kB of Flash Memory for a Filesystem in Arduino before compiling!
* The Arduino "loop()" function must run as fast as possible, so the mqttc->tasks() routine can successfully maintain the network connection to the broker.
* Download the [MQTTX MQTT Client](https://mqttx.app/) or the [IoT MQTT Panel App](https://play.google.com/store/apps/details?id=snr.lab.iotmqttpanel.prod&hl=en_CA) to interact with the example below.

### Example

```c
// Provision a WiFi connection (if needed), then
// Connect to the public Mosquitto Broker (test.mosquitto.org), then simply maintain the connection.
// Publish and subscribe topic variables are defined but not used.
// Look for messages on the Serial port to confirm connection success.

// THIS BROKER IS A PUBLIC SERVICE. DO NOT SHARE SENSITIVE DATA

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;            

// MQTT Broker URL, Username, Password
const char MQTTbroker[] = "test.mosquitto.org";
int MQTTport = 1883;    // EDIT: 1883 for open connection, or 8883 for secure connection
const char MQTTusername[] = "";
const char MQTTpassword[] = "";

// MQTT publish topics
const char potentiometerTopic[] = "";

// Array of MQTT subscribe topics (maximum of 10 or define "" for none)
const char *subscribeTopicIDs[] = {""};

// Calculate the number of subscribe topics
int num_subscribeTopicIDs = sizeof(subscribeTopicIDs)/sizeof(subscribeTopicIDs[0]);

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  // Attempt to connect to WiFi AP using existing credentials, or provision a new WiFi Connection
  if (!myRobot->network->connect())
  {
    myRobot->network->provision();
  }
  // Attempt to connect to the MQTT Broker
  if (!myRobot->mqttc->connect(MQTTbroker, MQTTport, MQTTusername, MQTTpassword, subscribeTopicIDs, num_subscribeTopicIDs))
  {
    Serial.println("Failed to initialize MQTT Client!");
    myRobot->board->led_blink(10);
    while (1)
    {
      // blink the USER LED to indicate error
      myRobot->board->tasks();
      // sample the USER SWITCH to reset WiFi credentials and reboot 
      if(myRobot->board->is_button_pressed())
      {
        myRobot->network->reset_connection();
      }
    }
  }
}

void loop() {
  myRobot->network->tasks();  // manage WiFi connection
  myRobot->mqttc->tasks();    // manage MQTT client connection
}
```

### See also

* [connect()](#bool-connectconst-char-mqbroker-int-mqport-const-char-mqusername-const-char-mqpassword-const-char-subtopicids-int-size_subtopicids)
* [tasks()](<#void-tasksvoid>)
* [is_connected()](#bool-is_connectedvoid)
* [send_message()](#void-send_messageconst-char-pubtopic-char-pubpayload)
* [is_message_available()](#bool-is_message_availableconst-char-subtopic)
* [receive_message()](#char-receive_messagevoid)


## `void tasks(void)`

Run background tasks to maintain the MQTT Client connection and check for messages. This function will attempt to reconnect with the broker if the connection is lost.

### Syntax

```c
myRobot->mqttc->tasks();
```
### Parameters

* None.

### Returns

* None.

### Notes

* Download the [MQTTX MQTT Client](https://mqttx.app/) or the [IoT MQTT Panel App](https://play.google.com/store/apps/details?id=snr.lab.iotmqttpanel.prod&hl=en_CA) to interact with the example below.

### Example

```c
// Connect to the Public Mosquitto Broker (test.mosquitto.org), then simply maintain the connection by calling the "tasks()" function

// THIS BROKER IS A PUBLIC SERVICE. DO NOT SHARE SENSITIVE DATA

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;            

// MQTT Broker URL, Username, Password
const char MQTTbroker[] = "test.mosquitto.org";
int MQTTport = 1883;    // EDIT: 1883 for open connection, or 8883 for secure connection
const char MQTTusername[] = "";
const char MQTTpassword[] = "";

// MQTT publish topics
const char potentiometerTopic[] = "";

// Array of MQTT subscribe topics (maximum of 10 or define "" for none)
const char *subscribeTopicIDs[] = {""};

// Calculate the number of subscribe topics
int num_subscribeTopicIDs = sizeof(subscribeTopicIDs)/sizeof(subscribeTopicIDs[0]);

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  // Attempt to connect to WiFi AP using existing credentials, or provision a new WiFi Connection
  if (!myRobot->network->connect())
  {
    myRobot->network->provision();
  }
  // Attempt to connect to the MQTT Broker
  if (!myRobot->mqttc->connect(MQTTbroker, MQTTport, MQTTusername, MQTTpassword, subscribeTopicIDs, num_subscribeTopicIDs))
  {
    Serial.println("Failed to initialize MQTT Client!. Stopping.");
    myRobot->board->led_blink(10);
    while (1)
    {
      // blink the USER LED to indicate error
      myRobot->board->tasks();
      // sample the USER SWITCH to reset WiFi credentials and reboot 
      if(myRobot->board->is_button_pressed())
      {
        myRobot->network->reset_connection();
      }
    }
  }
}

void loop() {
  myRobot->network->tasks();  // manage WiFi connection
  myRobot->mqttc->tasks();    // manage MQTT client connection
}
```

### See also

* [connect()](#bool-connectconst-char-mqbroker-int-mqport-const-char-mqusername-const-char-mqpassword-const-char-subtopicids-int-size_subtopicids)
* [tasks()](<#void-tasksvoid>)
* [is_connected()](#bool-is_connectedvoid)
* [send_message()](#void-send_messageconst-char-pubtopic-char-pubpayload)
* [is_message_available()](#bool-is_message_availableconst-char-subtopic)
* [receive_message()](#char-receive_messagevoid)

## `bool is_connected(void)`

Use to check whether the MQTT connection is active before sending/receiving data.       

### Syntax

```c
if (myRobot->mqttc->is_connected())
{
  myRobot->mqttc->send_message("/message", "Hello, World!");
}
```
### Parameters

* None.

### Returns

* **bool**: TRUE if the connection is active, FALSE if disconnected/trying to re-connect.

### Notes

* Download the [MQTTX MQTT Client](https://mqttx.app/) or the [IoT MQTT Panel App](https://play.google.com/store/apps/details?id=snr.lab.iotmqttpanel.prod&hl=en_CA) to interact with the example below.

### Example

```c
// Connect to the Public Mosquitto Broker (test.mosquitto.org), then publish a counter value every time the button is pressed, as long as the connection is valid.
// Run the MQTTX MQTT Client App or the IoT MQTT Panel App to subscribe/view the data.

// THIS BROKER IS A PUBLIC SERVICE. DO NOT SHARE SENSITIVE DATA

#include <cetalib.h>
#include <stdio.h>    // needed for the sprintf() function

const struct CETALIB_INTERFACE *myRobot = &CETALIB;            

// MQTT Broker URL, Username, Password
const char MQTTbroker[] = "test.mosquitto.org";
int MQTTport = 1883;    // EDIT: 1883 for open connection, or 8883 for secure connection
const char MQTTusername[] = "";
const char MQTTpassword[] = "";

// MQTT publish topics and payload buffer
const char counterTopic[] = "CETAIoTRobot/out/count";
char pubPayload[32];

// Array of MQTT subscribe topics (maximum of 10 or define "" for none)
const char *subscribeTopicIDs[] = {""};

// Calculate the number of subscribe topics
int num_subscribeTopicIDs = sizeof(subscribeTopicIDs)/sizeof(subscribeTopicIDs[0]);

// counter variable value to publish
int robotCount = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  // Attempt to connect to WiFi AP using existing credentials, or provision a new WiFi Connection
  if (!myRobot->network->connect())
  {
    myRobot->network->provision();
  }
  // Attempt to connect to the MQTT Broker
  if (!myRobot->mqttc->connect(MQTTbroker, MQTTport, MQTTusername, MQTTpassword, subscribeTopicIDs, num_subscribeTopicIDs))
  {
    Serial.println("Failed to initialize MQTT Client!. Stopping.");
    myRobot->board->led_blink(10);
    while (1)
    {
      // blink the USER LED to indicate error
      myRobot->board->tasks();
      // sample the USER SWITCH to reset WiFi credentials and reboot 
      if(myRobot->board->is_button_pressed())
      {
        myRobot->network->reset_connection();
      }
    }
  }
}

void loop() {
  myRobot->network->tasks();  // manage WiFi connection
  myRobot->mqttc->tasks();    // manage MQTT client connection
  myRobot->board->tasks();    // manage USER LED and USER SWITCH
  // publish data to the broker if connected
  if(myRobot->mqttc->is_connected())
  {
    if(myRobot->board->is_button_pressed())
    {
      sprintf(pubPayload, "%d", robotCount++);
      myRobot->mqttc->send_message(counterTopic, pubPayload);
    }
  }
}
```

### See also

* [connect()](#bool-connectconst-char-mqbroker-int-mqport-const-char-mqusername-const-char-mqpassword-const-char-subtopicids-int-size_subtopicids)
* [tasks()](<#void-tasksvoid>)
* [is_connected()](#bool-is_connectedvoid)
* [send_message()](#void-send_messageconst-char-pubtopic-char-pubpayload)
* [is_message_available()](#bool-is_message_availableconst-char-subtopic)
* [receive_message()](#char-receive_messagevoid)

## `void send_message(const char *pubTopic, char *pubPayload)`

Publish a message payload (formatted as a C character string) to a topic.

### Syntax

```c
myRobot->mqttc->send_message(pubTopic, pubPayload);
```
### Parameters

* **const char \*pubTopic**: MQTT Publish Topic Identifier
* **char \*pubPayload**: MQTT Message Payload, formatted as a C string

### Returns

* None.

### Notes

* Use stdio function "sprintf()" to create formatted JSON messages.
* Download the [MQTTX MQTT Client](https://mqttx.app/) or the [IoT MQTT Panel App](https://play.google.com/store/apps/details?id=snr.lab.iotmqttpanel.prod&hl=en_CA) to interact with the example below.

### Example

```c
// Connect to the Public Mosquitto Broker (test.mosquitto.org),
// Then publish a JSON formatted C string every time the button is pressed
// Run the MQTTX MQTT Client App or the IoT MQTT Panel App to subscribe/view the data.

// THIS BROKER IS A PUBLIC SERVICE. DO NOT SHARE SENSITIVE DATA

#include <cetalib.h>
#include <stdio.h>    // needed for the sprintf() function

const struct CETALIB_INTERFACE *myRobot = &CETALIB;            

// MQTT Broker URL, Username, Password
const char MQTTbroker[] = "test.mosquitto.org";
int MQTTport = 1883;    // EDIT: 1883 for open connection, or 8883 for secure connection
const char MQTTusername[] = "";
const char MQTTpassword[] = "";

// MQTT publish topics and payload buffer
const char counterTopic[] = "CETAIoTRobot/out/count";
char pubPayload[64];

// Array of MQTT subscribe topics (maximum of 10 or define "" for none)
const char *subscribeTopicIDs[] = {""};

// Calculate the number of subscribe topics
int num_subscribeTopicIDs = sizeof(subscribeTopicIDs)/sizeof(subscribeTopicIDs[0]);

// counter variable value to publish
int robotCount = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  // Attempt to connect to WiFi AP using existing credentials, or provision a new WiFi Connection
  if (!myRobot->network->connect())
  {
    myRobot->network->provision();
  }
  // Attempt to connect to the MQTT Broker
  if (!myRobot->mqttc->connect(MQTTbroker, MQTTport, MQTTusername, MQTTpassword, subscribeTopicIDs, num_subscribeTopicIDs))
  {
    Serial.println("Failed to initialize MQTT Client!. Stopping.");
    myRobot->board->led_blink(10);
    while (1)
    {
      // blink the USER LED to indicate error
      myRobot->board->tasks();
      // sample the USER SWITCH to reset WiFi credentials and reboot 
      if(myRobot->board->is_button_pressed())
      {
        myRobot->network->reset_connection();
      }
    }
  }
}

void loop() {
  myRobot->network->tasks();  // manage WiFi connection
  myRobot->mqttc->tasks();    // manage MQTT client connection
  myRobot->board->tasks();    // manage USER LED and USER SWITCH
  // publish data to the broker if connected
  if(myRobot->mqttc->is_connected())
  {
    if(myRobot->board->is_button_pressed())
    {
      sprintf(pubPayload, "{\"robotCount\": %d}", robotCount++);
      myRobot->mqttc->send_message(counterTopic, pubPayload);
    }
  }
}
```

### See also

* [connect()](#bool-connectconst-char-mqbroker-int-mqport-const-char-mqusername-const-char-mqpassword-const-char-subtopicids-int-size_subtopicids)
* [tasks()](<#void-tasksvoid>)
* [is_connected()](#bool-is_connectedvoid)
* [send_message()](#void-send_messageconst-char-pubtopic-char-pubpayload)
* [is_message_available()](#bool-is_message_availableconst-char-subtopic)
* [receive_message()](#char-receive_messagevoid)

## `bool is_message_available(const char *subTopic)`

Check if a message has been received for a specific subscription topic.        

### Syntax

```c
char subPayload[];
if (myRobot->mqttc->is_message_available("CETAIoTRobot/in/ledControl"))
{
  strcpy(subPayload, myRobot->mqttc->receive_message());
}
```
### Parameters

* **const char \*subTopic**: MQTT Subscribe Topic Identifier to evaluate

### Returns

* **bool**: Result of the inquiry.
  * **true**: Message was received on the topic
  * **false**: Message was not received on the topic 

### Notes

* Messages are not buffered. Call the **mqttc->tasks()** function in the main loop as fast as possible so no messages are missed.
* If a message is detected, you must read the message using the **mqttc->receive_message()** function as shown below
* You must add **#include <string.h>** to access the strcpy function needed to transfer the message contents into your own buffer.
* The mqttc logic can receive a maximum message size of **128** bytes
* Download the [MQTTX MQTT Client](https://mqttx.app/) or the [IoT MQTT Panel App](https://play.google.com/store/apps/details?id=snr.lab.iotmqttpanel.prod&hl=en_CA) to interact with the example below.

### Example

```c
// Connect to the Public Mosquitto Broker (test.mosquitto.org)
// Define an "led Control" subscription topic and payload buffer
// Print received messages to the Serial port
// Publish a counter value when the USER SWITCH is pressed

// THIS BROKER IS A PUBLIC SERVICE. DO NOT SHARE SENSITIVE DATA

#include <cetalib.h>
#include <stdio.h>    // needed for the "sprintf()"" function
#include <string.h>   // needed for the "strcpy()" function

const struct CETALIB_INTERFACE *myRobot = &CETALIB;            

// MQTT Broker URL, Username, Password
const char MQTTbroker[] = "test.mosquitto.org";
int MQTTport = 1883;    // EDIT: 1883 for open connection, or 8883 for secure connection
const char MQTTusername[] = "";
const char MQTTpassword[] = "";

// MQTT publish topics and payload buffer
const char counterTopic[] = "CETAIoTRobot/out/count";
char pubPayload[64];

// Array of MQTT subscribe topics (maximum of 10 or define "" for none) and payload buffer
const char ledControlTopic[] = "CETAIoTRobot/in/ledControl";
const char *subscribeTopicIDs[] = {ledControlTopic};
char subPayload[64];

// Calculate the number of subscribe topics
int num_subscribeTopicIDs = sizeof(subscribeTopicIDs)/sizeof(subscribeTopicIDs[0]);

// counter variable value to publish
int robotCount = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  // Attempt to connect to WiFi AP using existing credentials, or provision a new WiFi Connection
  if (!myRobot->network->connect())
  {
    myRobot->network->provision();
  }
  // Attempt to connect to the MQTT Broker
  if (!myRobot->mqttc->connect(MQTTbroker, MQTTport, MQTTusername, MQTTpassword, subscribeTopicIDs, num_subscribeTopicIDs))
  {
    Serial.println("Failed to initialize MQTT Client!. Stopping.");
    myRobot->board->led_blink(10);
    while (1)
    {
      // blink the USER LED to indicate error
      myRobot->board->tasks();
      // sample the USER SWITCH to reset WiFi credentials and reboot 
      if(myRobot->board->is_button_pressed())
      {
        myRobot->network->reset_connection();
      }
    }
  }
}

void loop() {
  myRobot->network->tasks();  // manage WiFi connection
  myRobot->mqttc->tasks();    // manage MQTT client connection
  myRobot->board->tasks();    // manage USER LED and USER SWITCH
  // exchange data with the broker if connected
  if(myRobot->mqttc->is_connected())
  {
    // send data if ready
    if(myRobot->board->is_button_pressed())
    {
      sprintf(pubPayload, "{\"robotCount\": %d}", robotCount++);
      myRobot->mqttc->send_message(counterTopic, pubPayload);
    }
    // receive data if available
    if (myRobot->mqttc->is_message_available(ledControlTopic))
    {
      strcpy(subPayload, myRobot->mqttc->receive_message());
      Serial.print("Received LED Control Message: ");
      Serial.println(subPayload);
    }
  }
}
```

### See also

* [connect()](#bool-connectconst-char-mqbroker-int-mqport-const-char-mqusername-const-char-mqpassword-const-char-subtopicids-int-size_subtopicids)
* [tasks()](<#void-tasksvoid>)
* [is_connected()](#bool-is_connectedvoid)
* [send_message()](#void-send_messageconst-char-pubtopic-char-pubpayload)
* [is_message_available()](#bool-is_message_availableconst-char-subtopic)
* [receive_message()](#char-receive_messagevoid)

## `char* receive_message(void)`

Obtain a pointer to the latest received subscription message.        

### Syntax

```c
char subPayload[64];
if (myRobot->mqttc->is_message_available("CETAIoTRobot/in/ledControl"))
{
  strcpy(subPayload, myRobot->mqttc->receive_message());
}
```
### Parameters

* None.

### Returns

* **char***: A pointer to the latest received message.
  * use C string library function **strcpy()** to save the contents to a local buffer

### Notes

* Messages are not buffered. Call the **mqttc->tasks()** function in the main loop as fast as possible so no messages are missed.
* You must add **#include <string.h>** to access the **strcpy()** function needed to transfer the message contents into your own buffer.
* mqttc logic will receive a maximum message size of **128** bytes
* Download the [MQTTX MQTT Client](https://mqttx.app/) or the [IoT MQTT Panel App](https://play.google.com/store/apps/details?id=snr.lab.iotmqttpanel.prod&hl=en_CA) to interact with the example below.

### Example

```c
// Connect to the Public Mosquitto Broker (test.mosquitto.org)
// Define an "led Control" subscription topic and payload buffer
// Print received messages to the Serial port
// Publish a counter value when the USER SWITCH is pressed

// THIS BROKER IS A PUBLIC SERVICE. DO NOT SHARE SENSITIVE DATA

#include <cetalib.h>
#include <stdio.h>    // needed for the "sprintf()"" function
#include <string.h>   // needed for the "strcpy()" function

const struct CETALIB_INTERFACE *myRobot = &CETALIB;            

// MQTT Broker URL, Username, Password
const char MQTTbroker[] = "test.mosquitto.org";
int MQTTport = 1883;    // EDIT: 1883 for open connection, or 8883 for secure connection
const char MQTTusername[] = "";
const char MQTTpassword[] = "";

// MQTT publish topics and payload buffer
const char counterTopic[] = "CETAIoTRobot/out/count";
char pubPayload[64];

// Array of MQTT subscribe topics (maximum of 10 or define "" for none) and payload buffer
const char ledControlTopic[] = "CETAIoTRobot/in/ledControl";
const char *subscribeTopicIDs[] = {ledControlTopic};
char subPayload[64];

// Calculate the number of subscribe topics
int num_subscribeTopicIDs = sizeof(subscribeTopicIDs)/sizeof(subscribeTopicIDs[0]);

// counter variable value to publish
int robotCount = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  // Attempt to connect to WiFi AP using existing credentials, or provision a new WiFi Connection
  if (!myRobot->network->connect())
  {
    myRobot->network->provision();
  }
  // Attempt to connect to the MQTT Broker
  if (!myRobot->mqttc->connect(MQTTbroker, MQTTport, MQTTusername, MQTTpassword, subscribeTopicIDs, num_subscribeTopicIDs))
  {
    Serial.println("Failed to initialize MQTT Client!. Stopping.");
    myRobot->board->led_blink(10);
    while (1)
    {
      // blink the USER LED to indicate error
      myRobot->board->tasks();
      // sample the USER SWITCH to reset WiFi credentials and reboot 
      if(myRobot->board->is_button_pressed())
      {
        myRobot->network->reset_connection();
      }
    }
  }
}

void loop() {
  myRobot->network->tasks();  // manage WiFi connection
  myRobot->mqttc->tasks();    // manage MQTT client connection
  myRobot->board->tasks();    // manage USER LED and USER SWITCH
  // exchange data with the broker if connected
  if(myRobot->mqttc->is_connected())
  {
    // send data if ready
    if(myRobot->board->is_button_pressed())
    {
      sprintf(pubPayload, "{\"robotCount\": %d}", robotCount++);
      myRobot->mqttc->send_message(counterTopic, pubPayload);
    }
    // receive data if available
    if (myRobot->mqttc->is_message_available(ledControlTopic))
    {
      strcpy(subPayload, myRobot->mqttc->receive_message());
      Serial.print("Received LED Control Message: ");
      Serial.println(subPayload);
    }
  }
}
```

### See also

* [connect()](#bool-connectconst-char-mqbroker-int-mqport-const-char-mqusername-const-char-mqpassword-const-char-subtopicids-int-size_subtopicids)
* [tasks()](<#void-tasksvoid>)
* [is_connected()](#bool-is_connectedvoid)
* [send_message()](#void-send_messageconst-char-pubtopic-char-pubpayload)
* [is_message_available()](#bool-is_message_availableconst-char-subtopic)
* [receive_message()](#char-receive_messagevoid)