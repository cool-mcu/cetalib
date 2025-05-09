# servoarm Module

This module provides functions to interact with an [SG92R](https://abra-electronics.com/electromechanical/motors/servo-motors/sg92r-micro-servo-9g-2-5kg-torque-90-degrees.html) servo motor to implement basic LIFT/DROP functions for the [CETA IoT Robot (Kit #18-00019x)](https://www.cool-mcu.com/pages/robot-kit).

<img src="../../assets/pico-iot-robot-servoarm-addon.jpg?raw=true">

Pico WH GPIO Pin Connections:
* ORANGE wire:  PWM Signal - Connect to GP22
* RED wire:     Connect to VPack voltage
* BROWN wire:   Connect to GND

For detailed lessons covering robot components, schematics and step-by-step robot assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#void-initializevoid>)
* [set_angle()](<#void-set_angleint-angle>)
* [get_angle()](<#int-get_anglevoid>)
* [clear_calibration()](<#void-clear_calibrationvoid>)
* [home()](<#void-homevoid>)
* [lift()](<#void-liftvoid>)
* [drop()](<#void-dropvoid>)

## `void initialize(void)`

Initiallize pins, state variables, calibrate LIFT/DROP angles if desired.

### Syntax

```c++
myRobot->servoarm->initialize();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Initialize the servoarm module, then do nothing.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->servoarm->initialize();
}

void loop() {
  // Use the servoarm functions here
}
```

### See also

* [set_angle()](<#void-set_angleint-angle>)
* [get_angle()](<#int-get_anglevoid>)
* [clear_calibration()](<#void-clear_calibrationvoid>)
* [home()](<#void-homevoid>)
* [lift()](<#void-liftvoid>)
* [drop()](<#void-dropvoid>)

## `void set_angle(int angle)`

Set the servoarm angle (0 to 180 degrees).

### Syntax

```c++
myRobot->servoarm->set_angle(45);
```
### Parameters

* **angle**: integer containing the current desired servoarm angle (0 to 180 deg)

### Returns

* None.

### Notes

* None.

### Example

```c++
// Set the servoarm angle based on the potentiometer reading.
// The potentiometer reading (0-4095) is linearly mapped to angle (0 to 180 deg)

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
  myRobot->servoarm->initialize();
}

void loop() {
  int angle = (int)map(myRobot->board->get_potentiometer(), 0, 4095, 0, 180);
  myRobot->servoarm->set_angle(angle);
  delay(100);
}
```

### See also

* [initialize()](<#void-initializevoid>)
* [get_angle()](<#int-get_anglevoid>)
* [clear_calibration()](<#void-clear_calibrationvoid>)
* [home()](<#void-homevoid>)
* [lift()](<#void-liftvoid>)
* [drop()](<#void-dropvoid>)

## `int get_angle(void)`

Retrieve the most recent angle setting.

### Syntax

```c++
int angle = myRobot->servoarm->get_angle();
```
### Parameters

* None.

### Returns

* **int**: The most recent servoangle setting (0 to 180 degrees)

### Notes

* None.

### Example

```c++
// Set the servoarm angle based on the potentiometer reading.
// The potentiometer reading (0-4095) is linearly mapped to angle (0 to 180 deg)
// Read/display the most recent angle setting when the USER SWITCH is pressed

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  myRobot->servoarm->initialize();
}

void loop() {
  myRobot->board->tasks();
  if(myRobot->board->is_button_pressed())
  {
    Serial.print("Current angle: ");
    Serial.println(myRobot->servoarm->get_angle());
  }
  int angle = (int)map(myRobot->board->get_potentiometer(), 0, 4095, 0, 180);
  myRobot->servoarm->set_angle(angle);
  delay(100);
}
```

### See also

* [initialize()](<#void-initializevoid>)
* [set_angle()](<#void-set_angleint-angle>)
* [clear_calibration()](<#void-clear_calibrationvoid>)
* [home()](<#void-homevoid>)
* [lift()](<#void-liftvoid>)
* [drop()](<#void-dropvoid>)

## `void clear_calibration(void)`

Erase servoarm calibration data, triggering a calibration sequence when servoarm->initialize() is called.

### Syntax

```c++
myRobot->servoarm->clear_calibration();
```
### Parameters

* None.

### Returns

* None.

### Notes

* This function erases 3 saved position settings for the arm:
  * HOME position
  * LIFT position
  * DROP position
* The next time servoarm->initialize() is called, a calibration sequence will be triggered to set new values for these settings
* Open a serial terminal and follow the provided instructions to complete the calibration sequence

### Example

```c++
// Press/hold USER SWITCH for 3 seconds on reset to clear servoarm calibration memory.
// This will trigger a calibration sequence when servoarm->initialize() is called
// Follow the instructions on the serial terminal to set new servoarm positions for HOME, LIFT, DROP

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  if(0 == myRobot->board->get_button_level())
  {
    myRobot->servoarm->clear_calibration();
  }
  myRobot->servoarm->initialize();
}

void loop() {
  // Use the servoarm functions here
}
```

### See also

* [initialize()](<#void-initializevoid>)
* [set_angle()](<#void-set_angleint-angle>)
* [get_angle()](<#int-get_anglevoid>)
* [home()](<#void-homevoid>)
* [lift()](<#void-liftvoid>)
* [drop()](<#void-dropvoid>)

## `void home(void)`

Set servo to "HOME" position (requires calibration)

## `void lift(void)`

Set servo to "LIFT" position (requires calibration)

## `void drop(void)`

Set servo to "DROP" position (requires calibration)

### Syntax

```c++
myRobot->servoarm->home();
myRobot->servoarm->lift();
myRobot->servoarm->drop();
```
### Parameters

* None.

### Returns

* None.

### Notes

* The HOME, LIFT ad DROP position require executing a calibration sequence. See the [servoarm->clear_calibration()](<#void-clear_calibrationvoid>) example

### Example

```c++
// Press the USER SWITCH to cycle through the HOME, LIFT and DROP servoarm positions
// Requires a previously completed calibration sequence.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define some servoarm states
enum SERVOARM_POSITION {HOME=0, LIFT, DROP} servoPosition = HOME;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  myRobot->servoarm->initialize();
  myRobot->servoarm->home();
  Serial.println("ServoArm Position: HOME");
}

void loop() {
  myRobot->board->tasks();
  if(myRobot->board->is_button_pressed())
  {
    switch(servoPosition)
    {
      case HOME:
        servoPosition = LIFT;
        myRobot->servoarm->lift();
        Serial.println("ServoArm Position: LIFT");
        break;
      case LIFT:
        servoPosition = DROP;
        myRobot->servoarm->drop();
        Serial.println("ServoArm Position: DROP");
        break;
      case DROP:
        servoPosition = HOME;
        myRobot->servoarm->home();
        Serial.println("ServoArm Position: HOME");
        break;
      default:
        break;    
    }
  }
}
```

### See also

* [initialize()](<#void-initializevoid>)
* [set_angle()](<#void-set_angleint-angle>)
* [get_angle()](<#int-get_anglevoid>)
* [clear_calibration()](<#void-clear_calibrationvoid>)