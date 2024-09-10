# reflectance Module

This module provides functions to interact with the LEFT OPTO, MIDDLE OPTO and
RIGHT OPTO reflectance sensors on the [CETA IoT Robot (Kit #18-00019x)](https://www.cool-mcu.com/pages/robot-kit).

<img src="../assets/reflectance-sensors.jpg?raw=true">

Pico WH GPIO Pin Connections:
* GP26/ADC0: LEFT OPTO sensor
* GP27/ADC1: MIDDLE OPTO sensor
* GP28/ADC2: RIGHT OPTO sensor / USER POTENTIOMETER

For detailed lessons covering robot components, schematics and step-by-step robot assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#void-initializevoid>)
* [get_left_sensor()](<#float-get_left_sensorvoid>)
* [get_middle_sensor()](<#float-get_middle_sensorvoid>)
* [get_right_sensor()](<#float-get_right_sensorvoid>)

## `void initialize(void)`

Initiallize pins, state variables, calibrate if needed.

### Syntax

```c++
myRobot->reflectance->initialize();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Initialize the reflectance module, then do nothing.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->reflectance->initialize();
}

void loop() {
  // Use the reflectance functions here
}
```

### See also

* [tasks()](<#void-tasksvoid>)

## `float get_left_sensor(void)`

Sample the LEFT OPTO reflectance sensor reading.

### Syntax

```c++
float left_opto = myRobot->reflectance->get_left_sensor();
```
### Parameters

* None.

### Returns

* **float**: current reflectance sensor reading (0 to 1)

### Notes

* Readings closer to 1 correspond to the sensor being over a black line

### Example

```c++
// Print the LEFT OPTO reflectance sensor value every second.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

float left_opto;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->reflectance->initialize();
}

void loop() {
  left_opto = myRobot->reflectance->get_left_sensor();
  Serial.print("Left Opto: ");
  Serial.print(left_opto);
  Serial.println();
  delay(1000);
}
```

### See also

* [initialize()](<#bool-initializevoid>)

## `float get_middle_sensor(void)`

Sample the MIDDLE OPTO reflectance sensor reading.

### Syntax

```c++
float middle_opto = myRobot->reflectance->get_middle_sensor();
```
### Parameters

* None.

### Returns

* **float**: current reflectance sensor reading (0 to 1)

### Notes

* Readings closer to 1 correspond to the sensor being over a black line

### Example

```c++
// Print the MIDDLE OPTO reflectance sensor value every second.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

float middle_opto;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->reflectance->initialize();
}

void loop() {
  middle_opto = myRobot->reflectance->get_middle_sensor();
  Serial.print("Middle Opto: ");
  Serial.print(middle_opto);
  Serial.println();
  delay(1000);
}
```

### See also

* [initialize()](<#bool-initializevoid>)

## `float get_right_sensor(void)`

Sample the RIGHT OPTO reflectance sensor reading.

### Syntax

```c++
float right_opto = myRobot->reflectance->get_right_sensor();
```
### Parameters

* None.

### Returns

* **float**: current reflectance sensor reading (0 to 1)

### Notes

* Readings closer to 1 correspond to the sensor being over a black line

### Example

```c++
// Print the RIGHT OPTO reflectance sensor value every second.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

float right_opto;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->reflectance->initialize();
}

void loop() {
  right_opto = myRobot->reflectance->get_right_sensor();
  Serial.print("Right Opto: ");
  Serial.print(right_opto);
  Serial.println();
  delay(1000);
}
```

### See also

* [initialize()](<#bool-initializevoid>)