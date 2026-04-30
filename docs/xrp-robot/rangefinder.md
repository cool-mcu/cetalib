# rangefinder Module

This module provides functions to interact with an [HCSR04 Ultrasonic Proximity Sensor](https://www.sparkfun.com/ultrasonic-distance-sensor-3-3v-hc-sr04-180-red.html) on the [SparkFun XRP Robot](https://www.sparkfun.com/experiential-robotics-platform-xrp-kit.html).

<center><img src="../../assets/xrp-rangefinder.jpg?raw=true"></center><br>

XRP Controller RP2350 MCU GPIO Pin Connections:
* GP1: ECHO Signal (YELLOW wire)
* GP0: TRIG Signal (BLUE wire)

For detailed lessons covering robot components, schematics and step-by-step robot assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [CETA Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/ceta-robotics-and-iot-curriculum-for-pre-university-educators).

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
  while(!Serial);
  myRobot->rangefinder->initialize();
}

void loop() {
  target_distance = myRobot->rangefinder->get_distance();
  Serial.printf("Target distance (cm): %.2f\r\n", target_distance);
  delay(1000);
}
```

### See also

* [initialize()](<#void-initializevoid>)