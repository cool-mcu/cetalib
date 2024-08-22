# imu Module

This module provides functions to interact with the [Adafruit LSM6DSOX IMU](https://www.adafruit.com/product/4438) on the [CETA IoT Robot (Kit #18-00019x)](https://www.cool-mcu.com/pages/robot-kit). This module measures linear acceleration and angular velocity, and is primarily used to sense the relative heading of the robot. The sensor also provides ambient temperature measurement.

<img src="../assets/adafruit_imu.jpg?raw=true"><br>

Pico WH GPIO Pin Connections:
* GP18: I2C SDA SIGNAL for ADAFRUIT LSM6DSOX IMU
* GP19: I2C SCL SIGNAL for ADAFRUIT LSM6DSOX IMU

For detailed lessons covering imu components, schematics and step-by-step assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#void-initializevoid>)

## `void initialize(void)`

Initialize pin settings and module state variables. Must be called once in setup() before use.

### Syntax

```c++
myRobot->imu->initialize();
```
### Parameters

* None.

### Returns

* None.

### Example

```c++
// Initialize the board module, then do nothing.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
}

void loop() {
  // Use the board functions here
}
```

### See also

* [tasks()](<#void-tasksvoid>)