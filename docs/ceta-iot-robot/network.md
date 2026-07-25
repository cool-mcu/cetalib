# network Module

This module provides functions to create and manage a WiFi network connection on the [CETA IoT Robot](https://www.cool-mcu.com/pages/robot-kit).

<center><img src="../../assets/ceta-iot-network.jpg?raw=true"></center><br>

WiFi Network credentials are captured using a **provisioning procedure**, whereby the robot creates an access point, and starts a **provisioning web server** application. After the robot begins this procedure, do the following:

* Scan for WiFi networks, and connect to the Custom SSID displayed in the serial monitor
  * SSID: "CETAIoT_abcd", where "abcd" are the lower 2 bytes of your robot's unique MAC address
* Enter the passphrase to connect to the AP (it's the same as the SSID)
* On your smart device, open a browser and navigate to http://192.168.4.1
* The provisioning home page scans for local WiFi networks when loading
* Select a network from the pull-down menu, enter the passphrase, then press "Connect Robot"

<br><center><img src="../../assets/xrp-network-provisioning-app.jpg?raw=true"></center><br>

**NOTE:** WiFi network credentials are stored in a local filesystem, so before compilation, you will need to allocate 64kB of Flash memory to support a filesystem as shown here:

<center><img src="../../assets/xrp_filesystem_allocate.jpg?raw=true"></center><br>

For detailed lessons covering WiFi networking concepts, schematics, and step-by-step robot assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [CETA Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/ceta-robotics-and-iot-curriculum-for-pre-university-educators).


## Methods:

* [initialize()](#void-initializevoid)
* [connect()](#bool-connectvoid)
* [provision()](#void-provisionvoid)
* [tasks()](#void-tasksvoid)
* [is_ready()](#bool-is_readyvoid)
* [getIPAddr()](#const-char-getipaddrvoid)
* [clear_credentials()](#void-clear_credentialsvoid)
* [reset_connection()](#void-reset_connectionvoid)

## `void initialize(void)`

Initializes the LittleFS and I/O pins needed for the module

### Syntax

```c
myRobot->network->initialize();
```
### Parameters

* None.

### Returns

* None.

### Notes

* Use this method in setup() before calling any other network methods

### Example

```c
// Initialize the "network" module
// Attempt to connect to the saved network
// Launch the provisioning server if connection fails

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

int ledPattern = 1;   // USER LED heartbeat pattern

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->board->initialize();
  myRobot->network->initialize();
  // attempt to connect using stored credentials
  if (!myRobot->network->connect())
  {
    // could not connect to WiFi, so run the provisioning service
    myRobot->network->provision();
  }
  Serial.printf("Connected to WiFi! Local IP Address: %s\r\n", myRobot->network->get_IPAddr());
  myRobot->board->led_pattern(ledPattern);
}

// the loop function runs over and over again forever
void loop() {
  myRobot->network->tasks();  // Monitor WiFi connection and USER SWITCH
                              // Press/hold the USER SWITCH for 3 seconds to trigger the provisioning service again
  myRobot->board->tasks();    // Execute USER LED blink pattern                
}
```

### See also

* [initialize()](#void-initializevoid)
* [connect()](#bool-connectvoid)
* [provision()](#void-provisionvoid)
* [tasks()](#void-tasksvoid)
* [is_ready()](#bool-is_readyvoid)
* [getIPAddr()](#const-char-getipaddrvoid)
* [clear_credentials()](#void-clear_credentialsvoid)
* [reset_connection()](#void-reset_connectionvoid)

## `bool connect(void)`

Attempt to connect as a station with an Access Pointusing using WiFi credentials from a saved file in the filesystem.

### Syntax

```c
myRobot->network->connect();
```
### Parameters

* None.

### Returns

* **bool**:
  * TRUE if succesfully connected to the saved WiFi Access Point,
  * FALSE if unable to connect using the saved WiFi credentials

### Notes

* Use this method in **setup()** to connect to a desired network
  * If connection fails, call **provision()** to connect to another WiFi network
* If you want to change networks in your main loop, press/hold USER SWITCH for 3 seconds while calling the **tasks()** routine in the **loop()** function.

### Example

```c
// Provision a WiFi connection for the robot
// The robot attempts to connect using stored WiFi credentials,
// If it fails, starts an Access Point on 192.168.4.1 and launches a WiFi provisioning web server.
// The BUILTIN LED on the Robot will blink in a heartbeat pattern indicating the server is running.
// Next, using your smart device,
//  - Scan for WiFi networks, and connect to the Custom SSID displayed in the serial monitor
//  - Enter the passphrase (it's identical to the Custom SSID)
//  - Open a browser and navigate to http://192.168.4.1
//  - The web home page scans for local WiFi networks when loading
//  - Select a network from the pull-down menu, enter the passphrase, then press "Connect Robot"

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

int ledPattern = 1;   // USER LED heartbeat pattern

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->board->initialize();
  myRobot->board->led_pattern(ledPattern);
  // attempt to connect using stored credentials
  if (!myRobot->network->connect())
  {
    // could not connect to WiFi, so run the provisioning service
    myRobot->network->provision();
  }
  Serial.printf("Connected to WiFi! Local IP Address: %s\r\n", myRobot->network->get_IPAddr());
}

// the loop function runs over and over again forever
void loop() {
  myRobot->network->tasks();  // Monitor WiFi connection and USER SWITCH
                              // Press/hold the USER SWITCH for 3 seconds to trigger the provisioning service again
  myRobot->board->tasks();    // Execute USER LED blink pattern                
}
```

### See also

* [initialize()](#void-initializevoid)
* [connect()](#bool-connectvoid)
* [provision()](#void-provisionvoid)
* [tasks()](#void-tasksvoid)
* [is_ready()](#bool-is_readyvoid)
* [getIPAddr()](#const-char-getipaddrvoid)
* [clear_credentials()](#void-clear_credentialsvoid)
* [reset_connection()](#void-reset_connectionvoid)

## `void provision(void)`

Execute a provisioning service to capture credentials for a WiFI network.

### Syntax

```c
if (!myRobot->network->connect())
{
  myRobot->network->provision();
}
```
### Parameters

* None.

### Returns

* None. 

### Notes

* Use this method in **setup()** to connect to a desired network
  * If **connect()** fails, call **provision()** to capture credentials for another WiFi network
  * This is a **blocking** function and will reboot the MCU when new credentials are saved.
* If you want to change networks in your main loop, press/hold USER SWITCH for 3 seconds while calling the **tasks()** routine in the **loop()** function.

### Example

```c
// Provision a WiFi connection for the robot
// The robot attempts to connect using stored WiFi credentials,
// If it fails, starts an Access Point on 192.168.4.1 and launches a WiFi provisioning web server.
// The BUILTIN LED on the Robot will blink in a heartbeat pattern indicating the server is running.
// Next, using your smart device,
//  - Scan for WiFi networks, and connect to the Custom SSID displayed in the serial monitor
//  - Enter the passphrase (it's identical to the Custom SSID)
//  - Open a browser and navigate to http://192.168.4.1
//  - The web home page scans for local WiFi networks when loading
//  - Select a network from the pull-down menu, enter the passphrase, then press "Connect Robot"

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

int ledPattern = 1;   // USER LED heartbeat pattern

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->board->initialize();
  myRobot->board->led_pattern(ledPattern);
  // attempt to connect using stored credentials
  if (!myRobot->network->connect())
  {
    // could not connect to WiFi, so run the provisioning service
    myRobot->network->provision();
  }
  Serial.printf("Connected to WiFi! Local IP Address: %s\r\n", myRobot->network->get_IPAddr());
}

// the loop function runs over and over again forever
void loop() {
  myRobot->network->tasks();  // Monitor WiFi connection and USER SWITCH
                              // Press/hold the USER SWITCH for 3 seconds to trigger the provisioning service again
  myRobot->board->tasks();    // Execute USER LED blink pattern                
}
```

### See also

* [initialize()](#void-initializevoid)
* [connect()](#bool-connectvoid)
* [provision()](#void-provisionvoid)
* [tasks()](#void-tasksvoid)
* [is_ready()](#bool-is_readyvoid)
* [getIPAddr()](#const-char-getipaddrvoid)
* [clear_credentials()](#void-clear_credentialsvoid)
* [reset_connection()](#void-reset_connectionvoid)

## `void tasks(void)`

Monitors/updates WiFi connection state, and attempts to re-connect using stored parameters if needed. Also monitors the USER SWITCH - if pressed for 3 seconds, deletes WiFI credentials and reboots system (ensure that **provision()** is called in **setup()** to capture new WiFi credentials).

### Syntax

```c
myRobot->network->tasks();
```
### Parameters

* None.

### Returns

* None.

### Notes

* Use this method in your main loop to monitor the connection and the USER SWITCH
* Make sure there are **no blocking functions** in your main loop!

### Example

```c
// Connect to the saved WiFI network in setup()
// Monitor the USER SWITCH in loop() to erase saved settings, reboot, then run provisioning again

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

int ledPattern = 1;   // USER LED heartbeat pattern

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->board->initialize();
  myRobot->board->led_pattern(ledPattern);
  // attempt to connect using stored credentials
  if (!myRobot->network->connect())
  {
    // could not connect to WiFi, so run the provisioning service
    myRobot->network->provision();
  }
  Serial.printf("Connected to WiFi! Local IP Address: %s\r\n", myRobot->network->get_IPAddr());
}

// the loop function runs over and over again forever
void loop() {
  myRobot->network->tasks();  // Monitor WiFi connection and USER SWITCH
                              // Press/hold the USER SWITCH for 3 seconds to trigger the provisioning service again
  myRobot->board->tasks();    // Execute USER LED blink pattern                
}
```

### See also

* [initialize()](#void-initializevoid)
* [connect()](#bool-connectvoid)
* [provision()](#void-provisionvoid)
* [tasks()](#void-tasksvoid)
* [is_ready()](#bool-is_readyvoid)
* [getIPAddr()](#const-char-getipaddrvoid)
* [clear_credentials()](#void-clear_credentialsvoid)
* [reset_connection()](#void-reset_connectionvoid)

## `bool is_ready(void)`

Capture the current WiFi network connection status. Intended to be used alongside the **getIPAddr()** method to update the IP address displayed on an external display, such as an OLED.

### Syntax

```c
myRobot->network->is_ready();
```
### Parameters

* None.

### Returns

* **bool**:
  * TRUE: the robot is connected to a WiFi network and has a IP address
  * FALSE: the robot is not connected to a WiFi network

### Notes

* Do not use this method to trigger the **connect()** method, since the **tasks()** routine will automatically try to re-connect to the stored network.

### Example

```c
// Monitor the network connection status every second and update the IP Address in the Serial Monitor

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

unsigned long currentSampleTime, previousSampleTime;
unsigned long sampleInterval = 1000;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  while(!Serial);
  // attempt to connect using stored credentials
  if (!myRobot->network->connect())
  {
    // could not connect to WiFi, so run the provisioning service
    myRobot->network->provision();
  }
}

// the loop function runs over and over again forever
void loop() {
  myRobot->network->tasks();  // Monitor WiFi connection and USER SWITCH
  // Sample WiFi connection status and update IP Address
  currentSampleTime = millis();
  if((currentSampleTime - previousSampleTime) >= sampleInterval)
  {
    previousSampleTime = currentSampleTime;
    if(myRobot->network->is_ready())
    {
      Serial.printf("Local IP Address: %s\r\n", myRobot->network->get_IPAddr());
    }
    else
    {
      Serial.printf("Local IP Address: -.-.-.-\r\n");
    }
  }      
}
```

### See also

* [initialize()](#void-initializevoid)
* [connect()](#bool-connectvoid)
* [provision()](#void-provisionvoid)
* [tasks()](#void-tasksvoid)
* [is_ready()](#bool-is_readyvoid)
* [getIPAddr()](#const-char-getipaddrvoid)
* [clear_credentials()](#void-clear_credentialsvoid)
* [reset_connection()](#void-reset_connectionvoid)

## `const char* getIPAddr(void)`

Returns a pointer to the WiFi STA IP Address as a C string.

### Syntax

```c
myRobot->network->is_ready();
```
### Parameters

* None.

### Returns

* **char***:
  * A pointer to a C string array containing the most recent IP Address

### Notes

* Use the **strcpy()** method from **string.h** to copy the IP address into a local char array

### Example

```c
// Monitor the network connection status every second and update the IP Address in the Serial Monitor
// Save/access the IP address using a C string array

#include <cetalib.h>
#include <string.h>   // required for "strcpy()"

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// application variables
unsigned long currentSampleTime, previousSampleTime;
unsigned long sampleInterval = 1000;
char myIPAddress[32];

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  while(!Serial);
  // attempt to connect using stored credentials
  if (!myRobot->network->connect())
  {
    // could not connect to WiFi, so run the provisioning service
    myRobot->network->provision();
  }
}

// the loop function runs over and over again forever
void loop() {
  myRobot->network->tasks();  // Monitor WiFi connection and USER SWITCH
  // Sample WiFi connection status and update IP Address
  currentSampleTime = millis();
  if((currentSampleTime - previousSampleTime) >= sampleInterval)
  {
    previousSampleTime = currentSampleTime;
    if(myRobot->network->is_ready())
    {
      strcpy(myIPAddress, myRobot->network->get_IPAddr());
    }
    else
    {
      strcpy(myIPAddress, "-.-.-.-");
    }
     Serial.printf("Local IP Address: %s\r\n", myIPAddress);
  }                  
}
```

### See also

* [initialize()](#void-initializevoid)
* [connect()](#bool-connectvoid)
* [provision()](#void-provisionvoid)
* [tasks()](#void-tasksvoid)
* [is_ready()](#bool-is_readyvoid)
* [getIPAddr()](#const-char-getipaddrvoid)
* [clear_credentials()](#void-clear_credentialsvoid)
* [reset_connection()](#void-reset_connectionvoid)

## `void clear_credentials(void)`

Deletes existing WiFi credentials. Can be used to trigger the provisioning server
during **setup()**.

### Syntax

```c
myRobot->network->clear_credentials();
```
### Parameters

* None.

### Returns

* None.

### Notes

* Use in **setup()** to erase existing credentials with switch press.

### Example

```c
// Delete existing WiFi credentials if USER SWITCH is pressed upon RESET
// Will trigger provisioning service in setup()

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

int ledPattern = 1;   // USER LED heartbeat pattern

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->board->initialize();
  myRobot->network->initialize();
  if (0 == myRobot->board->get_button_level())
  {
    myRobot->network->clear_credentials();
  }
  if (!myRobot->network->connect())
  {
    myRobot->network->provision();
  }
  Serial.printf("Local IP Address: %s\r\n", myRobot->network->get_IPAddr());
  myRobot->board->led_pattern(ledPattern);
}

// the loop function runs over and over again forever
void loop() {
  myRobot->network->tasks(); // Monitor WiFi connection
  myRobot->board->tasks();   // Execute USER LED blink pattern                
}
```

### See also

* [initialize()](#void-initializevoid)
* [connect()](#bool-connectvoid)
* [provision()](#void-provisionvoid)
* [tasks()](#void-tasksvoid)
* [is_ready()](#bool-is_readyvoid)
* [getIPAddr()](#const-char-getipaddrvoid)
* [clear_credentials()](#void-clear_credentialsvoid)
* [reset_connection()](#void-reset_connectionvoid)

## `void reset_connection(void)`

Deletes existing WiFi credentials and reboots the system. **setup()** should contain the **provision()** method to capture new WiFi credentials.

### Syntax

```c
myRobot->network->reset_connection();
```
### Parameters

* None.

### Returns

* None.

### Notes

* **setup()** should contain the **provision()** method to capture new WiFi credentials.

### Example

```c
// Delete existing WiFi credentials AND reboot if USER SWITCH is pressed
// Will trigger provisioning service in setup()

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->board->initialize();
  // attempt to connect using stored credentials
  if (!myRobot->network->connect())
  {
    // could not connect to WiFi, so run the provisioning service
    myRobot->network->provision();
  }
}

// the loop function runs over and over again forever
void loop() {
  myRobot->network->tasks();  // Monitor WiFi connection and USER SWITCH
  myRobot->board->tasks();    // Monitor USER SWITCH
  if(myRobot->board->is_button_pressed())
  {
    myRobot->network->reset_connection();
  }
}
```

### See also

* [initialize()](#void-initializevoid)
* [connect()](#bool-connectvoid)
* [provision()](#void-provisionvoid)
* [tasks()](#void-tasksvoid)
* [is_ready()](#bool-is_readyvoid)
* [getIPAddr()](#const-char-getipaddrvoid)
* [clear_credentials()](#void-clear_credentialsvoid)
* [reset_connection()](#void-reset_connectionvoid)