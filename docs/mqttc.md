# mqttc Module

This module provides functions to interact with the WiFi/Bluetooth radio on the [RPi Pico W](https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html#raspberry-pi-pico-w-and-pico-wh) board. Specifically, this module provides an interface to utilize the WiFi interface to create MQTT Client network connections.

<img src="../assets/mqttc.png?raw=true">

For detailed lessons covering networking concepts, the MQTT protocol, schematics and step-by-step robot assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [connect()](#bool-connectconst-char-myssid-const-char-mypass-const-char-mqbroker-int-mqport-const-char-mqusername-const-char-mqpassword-const-char-subtopicids-int-size_subtopicids)
* [disconnect()](<#void-disconnectvoid>)
* [tasks()](<#void-tasksvoid>)

## `bool connect(const char *MySSID, const char *MyPass, const char *MQbroker, int MQport, const char *MQusername, const char *MQpassword, const char *subTopicIDs[], int size_subTopicIDs)`

Connect to a WiFi Access Point, and then open an MQTT Client network connection to an MQTT Broker. <br></br>Secure connections (port 8883) are currently supported with Adafruit IO, HiveMQ, and Public Mosquitto brokers.

### Syntax

```c++
myRobot->mqttc->connect(...);
```
### Parameters

* **const char \*MySSID**: SSID of your WiFi network
* **const char \*MyPass**: WPA2 Passphrase of your WiFi network
* **const char \*MQbroker**: MQTT Broker name or IP address
* **int MQport**: MQTT Broker port identifier (1883 or 8883)
* **const char \*MQusername**: MQTT Broker Username (if required)
* **const char \*MQpassword**: MQTT Broker Password (if required)
* **const char \*subTopicIDs[]**: Array of subscriber topics to subscribe to when connected
* **int size_subTopicIDs**: Number of subscriber topics in the array

### Returns

* **bool**: TRUE if succesfully connected, FALSE if an error is detected in the parameter list.

### Notes

* The Pico W built-in LED will flash during connection attempts, and will be lit solid when connected to the access-point and broker.
* The mqttc->tasks() routine must be called regularly in loop() to maintain the network connection.
* Download the [MQTTX MQTT Client](https://mqttx.app/) to interact with the examples below.

### Example

```c++
// Connect to the Public Mosquitto Broker (test.mosquitto.org), then simply maintain the connection.
// Publish and subscribe topic variables are defined but not used.
// Look for messages on the Serial port to confirm connection success.
// The Pico W GREEN LED will be lit on connection success.

// THIS BROKER IS A PUBLIC SERVICE. DO NOT SHARE SENSITIVE DATA

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// WiFi Parameters
const char ssid[] = "MY_SSID";        // EDIT              
const char pass[] = "MY_PASSPHRASE";  // EDIT            

// MQTT Broker URL, Username, Password
const char MQTTbroker[] = "test.mosquitto.org";
int MQTTport = 1883;    // EDIT: 1883 for insecure connection, or 8883 for secure connection
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
  myRobot->mqttc->tasks();
}
```

### See also

* [initialize()](<#void-initializevoid>)

## `void disconnect(void)`

Disconnect from the Broker & Access Point.

### Syntax

```c++
myRobot->mqttc->disconnect();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Connect to the Public Mosquitto Broker (test.mosquitto.org)
// Disconnect from the Broker (and Access Point) when the USER SWITCH is pressed

// THIS BROKER IS A PUBLIC SERVICE. DO NOT SHARE SENSITIVE DATA

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// WiFi Parameters
const char ssid[] = "MY_SSID";        // EDIT              
const char pass[] = "MY_PASSPHRASE";  // EDIT            

// MQTT Broker URL, Username, Password
const char MQTTbroker[] = "test.mosquitto.org";
int MQTTport = 1883;    // EDIT: 1883 for insecure connection, or 8883 for secure connection
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
  myRobot->mqttc->tasks();
  myRobot->board->tasks();
  if (myRobot->board->is_button_pressed())
  {
    Serial.println("Disconnecting from Broker and Access Point");
    myRobot->mqttc->disconnect();
    Serial.println("Halting.");
    myRobot->board->led_blink(10);
    while (1)
    {
      myRobot->board->tasks();
    }
  }
}
```

### See also

* [get_left_sensor()](<#float-get_left_sensorvoid>)

## `void tasks(void)`

Run mqttc background tasks to maintain connections and check for messages.

### Syntax

```c++
myRobot->mqttc->tasks();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Connect to the Public Mosquitto Broker (test.mosquitto.org), then simply maintain the connection by calling the "tasks()" function

// THIS BROKER IS A PUBLIC SERVICE. DO NOT SHARE SENSITIVE DATA

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// WiFi Parameters
const char ssid[] = "MY_SSID";        // EDIT              
const char pass[] = "MY_PASSPHRASE";  // EDIT            

// MQTT Broker URL, Username, Password
const char MQTTbroker[] = "test.mosquitto.org";
int MQTTport = 1883;    // EDIT: 1883 for insecure connection, or 8883 for secure connection
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
  myRobot->mqttc->tasks();
}
```

### See also

* [get_left_sensor()](<#float-get_left_sensorvoid>)