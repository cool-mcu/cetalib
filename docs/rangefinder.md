# rangefinder Module

This module provides functions to interact with an [HC-SR04P Ultrasonic Proximity Sensor](https://abra-electronics.com/sensors/sensors-proximity-en/hc-sr04p.html)  on the [CETA IoT Robot (Kit #18-00019x)](https://www.cool-mcu.com/pages/robot-kit).

<img src="../assets/pico-iot-robot-rangefinder-addon.jpg?raw=true">

Pico WH GPIO Pin Connections:
* GP16: ECHO Signal (WHITE wire)
* GP17: TRIG Signal (GREY wire)

For detailed lessons covering robot components, schematics and step-by-step robot assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#void-initializevoid>)
* [get_distance()](<#float-get_distancevoid>)

## `void initialize(void)`

Initiallize pins, state variables.

### Syntax

```c++
myRobot->rangefinder->initialize();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Initialize the rangefinder module, then do nothing.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->rangefinder->initialize();
}

void loop() {
  // Use the rangefinder functions here
}
```

### See also

* [get_distance()](<#float-get_distancevoid>)

## `float get_distance(void)`

Measure distance to target (in cm).

### Syntax

```c++
float target_distance = myRobot->rangefinder->get_distance();
```
### Parameters

* None.

### Returns

* **float**: current distance (in cm) to a target (up top 45cm max)

### Notes

* Function blocks for up to 5 ms, providing up to ~45cm range measurement

### Example

```c++
// Measure/print the target distance every second.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

float target_distance;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->rangefinder->initialize();
}

void loop() {
  target_distance = myRobot->rangefinder->get_distance();
  Serial.print("Target distance (cm): ");
  Serial.print(target_distance);
  Serial.println();
  delay(1000);
}
```

### See also

* [initialize()](<#void-initializevoid>)