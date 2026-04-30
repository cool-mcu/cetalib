# joystick Module

This module provides functions to interact with a [Logitech F310](https://www.logitechg.com/en-ca/shop/p/f310-gamepad) gamepad connected to your PC.

<img src="../../assets/joystick-system.png?raw=true">

After uploading your joystick enabled sketch to the robot, it will start a dedicated WiFi Access Point having the following Network SSID, depending on robot type:

* "XRP_abcd", or
* "XRPBeta_abcd", or
* "CETAIoT_abcd"

Where "abcd" are the lower 2 bytes of your robot's unique MAC address.

Scan for wireless networks, and connect to your robot. The WiFi password is identical to the SSID.

Next, ensure your F310 is plugged in to your PC, then execute the [CETALIB Joystick UDP Client](https://github.com/cool-mcu/cetalib/blob/main/utilities/joystick/README.md) python script in your terminal/command window.

For detailed lessons covering the use of this module, [contact us](mailto:info@cool-mcu.com) to enrol in the [CETA Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/ceta-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#bool-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [is_active()](<#bool-is_activevoid>)
* [get_data()](<#gamepad-get_datavoid>)
* [get_left_tank_effort()](<#float-get_left_tank_effortvoid>)
* [get_right_tank_effort()](<#float-get_right_tank_effortvoid>)
* [get_arcade_throttle_effort()](<#float-get_arcade_throttle_effortvoid>)
* [get_arcade_turn_effort()](<#float-get_arcade_turn_effortvoid>)

## `bool initialize(void)`

Initialize pin settings and start an Access Point. Must be called once in setup() before use. 

### Syntax

```c++
myRobot->joystick->initialize();
```
### Parameters

* None.

### Returns

* **boolean**: *true* if the AP starts successfully, *false* if the WiFi resource is already in use.

### Notes

* Robots that have an additional LED (CETA IoT & XRP Robots) will illuminate the WiFi status LED when the AP is running.
* Sketches should enable the Serial console to display the AP messages on startup.
* Sketches should also use the USER LED to indicate successful Joystick module initialization.
* On successful initialization, scan WiFi networks on your PC and confirm your robot's AP SSID is found.

### Example

```c++
// Initialize the joystick module, then do nothing.
// Examine Serial messages and scan for your AP SSID on the PC.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  delay(5000);
  if(!myRobot->joystick->initialize())
  {
    Serial.println("Joystick Initialization failed. Stopping.");
    while(1);
  }
  Serial.println("Joystick Initialization success!");
}

void loop() {
  // Use the joystick functions here
}
```

### See also

* [initialize()](<#bool-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [is_active()](<#bool-is_activevoid>)
* [get_data()](<#gamepad-get_datavoid>)
* [get_left_tank_effort()](<#float-get_left_tank_effortvoid>)
* [get_right_tank_effort()](<#float-get_right_tank_effortvoid>)
* [get_arcade_throttle_effort()](<#float-get_arcade_throttle_effortvoid>)
* [get_arcade_turn_effort()](<#float-get_arcade_turn_effortvoid>)

## `void tasks(void)`

Run all background tasks to scan for gamepad packets, and save them locally. Run this method at the top of your main program loop.

### Syntax

```c++
myRobot->joystick->tasks();
```
### Parameters

* None.

### Returns

* None.

### Notes

* Make sure there are no blocking tasks in your main loop.

### Example

```c++
// Prints a status message whenever a packet is received.
// Run "cetalib-joystick-client.py" on the host PC to test this sketch.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  delay(5000);
  if(!myRobot->joystick->initialize())
  {
    Serial.println("Joystick Initialization failed. Stopping.");
    while(1);
  }
  Serial.println("Joystick Initialization success!");
}

void loop() {
  myRobot->joystick->tasks();         // run the engine that captures gamepad data
  if(myRobot->joystick->is_active())  // if there's new gamepad data available, process it
  {
    Serial.println("Received joystick data");
  }                    
}
```

### See also

* [initialize()](<#bool-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [is_active()](<#bool-is_activevoid>)
* [get_data()](<#gamepad-get_datavoid>)
* [get_left_tank_effort()](<#float-get_left_tank_effortvoid>)
* [get_right_tank_effort()](<#float-get_right_tank_effortvoid>)
* [get_arcade_throttle_effort()](<#float-get_arcade_throttle_effortvoid>)
* [get_arcade_turn_effort()](<#float-get_arcade_turn_effortvoid>)

## `bool is_active(void)`

Used to poll for new activity on gamepad.

### Syntax

```c++
myRobot->joystick->is_active();
```
### Parameters

* None.

### Returns

* **boolean**: *true* if new gamepad data is available, *false* if no new data is avaiable.

### Notes

* must be used after the tasks() method is called!

### Example

```c++
// Prints a status message whenever a packet is received.
// Run "cetalib-joystick-client.py" on the host PC to test this sketch.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  delay(5000);
  if(!myRobot->joystick->initialize())
  {
    Serial.println("Joystick Initialization failed. Stopping.");
    while(1);
  }
  Serial.println("Joystick Initialization success!");
}

void loop() {
  myRobot->joystick->tasks();         // run the task that captures gamepad data
  if(myRobot->joystick->is_active())  // if there's new gamepad data available, process it
  {
    Serial.println("Received joystick data");
  }                    
}
```

### See also

* [initialize()](<#bool-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [is_active()](<#bool-is_activevoid>)
* [get_data()](<#gamepad-get_datavoid>)
* [get_left_tank_effort()](<#float-get_left_tank_effortvoid>)
* [get_right_tank_effort()](<#float-get_right_tank_effortvoid>)
* [get_arcade_throttle_effort()](<#float-get_arcade_throttle_effortvoid>)
* [get_arcade_turn_effort()](<#float-get_arcade_turn_effortvoid>)

## `GAMEPAD* get_data(void)`

Returns a pointer to the latest gamepad data.

Refer to the [F310 Datasheet](https://www.logitech.com/assets/35017/gamepad-f310-gsw.pdf) for button names and locations.

### Syntax

```c++
GAMEPAD* joystick_data = myRobot->joystick->get_data();
```
### Parameters

* None.

### Returns

* GAMEPAD*: Pointer to a GAMEPAD data structure, containing the current state of the switches

### Notes

Here are the available switch data, using a defined GAMEPAD pointer variable "joystick_data":

```c++
joystick_data->clientIP;            // char*: string containing the client PC's IP address
joystick_data->clientPort;          // char*: string containing the client PC's Port address

joystick_data->gamepadRaw[0];       // uint8_t: first byte of 8-byte raw gamepad data
...
joystick_data->gamepadRaw[7];       // uint8_t: last byte of 8-byte raw gamepad data

joystick_data->leftStickX;          // uint8_t: X-axis reading from the left analog joystick
joystick_data->leftStickY;          // uint8_t: Y-axis reading from the left analog joystick
joystick_data->rightStickX;         // uint8_t: X-axis reading from the right analog joystick
joystick_data->rightStickY;         // uint8_t: Y-axis reading from the right analog joystick

joystick_data->dPad.isNorthPressed; // bool: D-Pad North button status
joystick_data->dPad.isEastPressed;  // bool: D-Pad East button status
joystick_data->dPad.isSouthPressed; // bool: D-Pad South button status
joystick_data->dPad.isWestPressed;  // bool: D-Pad West button status

joystick_data->actionButtons.isAPressed;  // bool: Action button "A" status
joystick_data->actionButtons.isBPressed;  // bool: Action button "B" status
joystick_data->actionButtons.isXPressed;  // bool: Action button "X" status
joystick_data->actionButtons.isYPressed;  // bool: Action button "Y" status

joystick_data->triggerButtons.isLeftTriggerPressed;   // bool: Left Trigger button status
joystick_data->triggerButtons.isRightTriggerPressed;  // bool: Right Trigger button status

joystick_data->bumperButtons.isLeftBumperPressed;   // bool: Left Bumper button status
joystick_data->bumperButtons.isRightBumperPressed;  // bool: Right Bumper button status

joystick_data->isStartPressed;  // bool: Start button status
joystick_data->isBackPressed;   // bool: Back button status
```



### Example

```c++
// Monitors Action buttons "A" and B" to control the USER LED

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

GAMEPAD* joystick_data; // define a pointer to the captured gamepad data

void setup() {
  Serial.begin(115200);
  delay(5000);
  myRobot->board->initialize();
  if(!myRobot->joystick->initialize())
  {
    Serial.println("Joystick Initialization failed. Stopping.");
    while(1);
  }
  Serial.println("Joystick Initialization success!");
}

void loop() {
  myRobot->board->tasks();            // run the task that maintains USER LED state
  myRobot->joystick->tasks();         // run the task that captures gamepad data
  if(myRobot->joystick->is_active())  // if there's new gamepad data available, process it
  {
    joystick_data = myRobot->joystick->get_data(); // point to the latest gamepad data
    if (joystick_data->actionButtons.isAPressed)
    {
      Serial.printf("Action A is Pressed. Turning USER LED ON\r\n");
      myRobot->board->led_on();
    }
    else if (joystick_data->actionButtons.isBPressed)
    {
      Serial.printf("Action B is Pressed. Turning USER LED OFF\r\n");
      myRobot->board->led_off();
    }
  }                    
}
```

### See also

* [initialize()](<#bool-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [is_active()](<#bool-is_activevoid>)
* [get_data()](<#gamepad-get_datavoid>)
* [get_left_tank_effort()](<#float-get_left_tank_effortvoid>)
* [get_right_tank_effort()](<#float-get_right_tank_effortvoid>)

## `float get_left_tank_effort(void)`
## `float get_right_tank_effort(void)`

Convert analog joystick data to implement a robot "tank" drive scheme with the **diffDrive->set_efforts()** function.

* "Left Stick Y" controls effort on Left Motor (-1.0 to +1.0)
* "Right Stick Y" controls effort on Right Motor (-1.0 to +1.0)

### Syntax

```c++
float leftEffort = myRobot->joystick->get_left_tank_effort();
float rightEffort = myRobot->joystick->get_right_tank_effort();
myRobot->diffDrive->set_efforts(leftEffort, rightEffort);
```
### Parameters

* None.

### Returns

* **float**: motor effort parameter (-1.0 to +1.0) to be used in the **diffDrive->set_efforts()** method.

### Notes

* None.

### Example

```c++
// Use the Gamepad analog joysticks to implement a "Tank Drive" motor scheme
// "Left Stick Y" controls effort on Left Motor
// "Right Stick Y" controls effort on Right Motor

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

GAMEPAD* joystick_data; // define a pointer to the captured gamepad data
float leftDriveEffort, rightDriveEffort; // motor effort parameters

void setup() {
  Serial.begin(115200);
  delay(5000);
  myRobot->diffDrive->initialize(false, false); // adjust parameters for forward motion in your robot
  if(!myRobot->joystick->initialize())
  {
    Serial.println("Joystick Initialization failed. Stopping.");
    while(1);
  }
  Serial.println("Joystick Initialization success!");
}

void loop() {
  myRobot->joystick->tasks();         // run the task that captures gamepad data
  if(myRobot->joystick->is_active())  // if there's new gamepad data available, process it
  {
    leftDriveEffort = myRobot->joystick->get_left_tank_effort();    // read/format leftStickY
    rightDriveEffort = myRobot->joystick->get_right_tank_effort();  // read/format rightStickY
    myRobot->diffDrive->set_efforts(leftDriveEffort, rightDriveEffort);
  }                    
}
```

### See also

* [initialize()](<#bool-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [is_active()](<#bool-is_activevoid>)
* [get_data()](<#gamepad-get_datavoid>)
* [get_left_tank_effort()](<#float-get_left_tank_effortvoid>)
* [get_right_tank_effort()](<#float-get_right_tank_effortvoid>)
* [get_arcade_throttle_effort()](<#float-get_arcade_throttle_effortvoid>)
* [get_arcade_turn_effort()](<#float-get_arcade_turn_effortvoid>)

## `float get_arcade_throttle_effort(void)`
## `float get_arcade_turn_effort(void)`

Convert analog joystick data to implement an FRC "Split-Stick Arcade Drive" motor scheme with the **diffDrive->set_efforts()** function.

* "Left Stick Y" controls "throttle" effort on both motors
* "Right Stick X" controls steering ("turn" effort)

### Syntax

```c++
float leftEffort = myRobot->joystick->get_arcade_throttle_effort();
float rightEffort = myRobot->joystick->get_arcade_turn_effort();
myRobot->diffDrive->set_efforts(leftEffort, rightEffort);
```
### Parameters

* None.

### Returns

* **float**: motor effort parameters (-1.0 to +1.0) to be used in the **diffDrive->set_efforts()** method.

### Notes

* None.

### Example

```c++
// Use the Gamepad analog joysticks to implement a "FRC Split-stick Arcade Drive" motor scheme
// "Left Stick Y" controls effort ("throttle") on both motors
// "Right Stick X" controls steering

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

GAMEPAD* joystick_data; // define a pointer to the captured gamepad data
float leftDriveEffort, rightDriveEffort; // motor effort parameters

void setup() {
  Serial.begin(115200);
  delay(5000);
  myRobot->diffDrive->initialize(false, false); // adjust parameters for forward motion in your robot
  if(!myRobot->joystick->initialize())
  {
    Serial.println("Joystick Initialization failed. Stopping.");
    while(1);
  }
  Serial.println("Joystick Initialization success!");
}

void loop() {
  myRobot->joystick->tasks();         // run the task that captures gamepad data
  if(myRobot->joystick->is_active())  // if there's new gamepad data available, process it
  {
    leftDriveEffort = myRobot->joystick->get_arcade_throttle_effort();    // read/format leftStickY
    rightDriveEffort = myRobot->joystick->get_arcade_turn_effort();       // read/format rightStickX
    myRobot->diffDrive->set_efforts(leftDriveEffort, rightDriveEffort);
  }                    
}
```

### See also

* [initialize()](<#bool-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [is_active()](<#bool-is_activevoid>)
* [get_data()](<#gamepad-get_datavoid>)
* [get_left_tank_effort()](<#float-get_left_tank_effortvoid>)
* [get_right_tank_effort()](<#float-get_right_tank_effortvoid>)
* [get_arcade_throttle_effort()](<#float-get_arcade_throttle_effortvoid>)
* [get_arcade_turn_effort()](<#float-get_arcade_turn_effortvoid>)

