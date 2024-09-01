# oled Module

This module provides text display functions for the [Adafruit 128x64 OLED](https://www.adafruit.com/product/938) add-on to the [CETA IoT Robot (Kit #18-00019x)](https://www.cool-mcu.com/pages/robot-kit).

<img src="../assets/pico-iot-robot-oled-addon.jpg?raw=true"><br>

Pico WH GPIO Pin Connections:
* GP18: I2C SDA SIGNAL for ADAFRUIT 128x64 OLED
* GP19: I2C SCL SIGNAL for ADAFRUIT 128x64 OLED

This component uses the same I2C bus instance as the IMU.

For detailed lessons covering oled components, schematics and step-by-step assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#bool-initializevoid>)

## `bool initialize(void)`

Initialize I2C bus, display splash screen for 5 seconds, clear screen and place cursor in home position. Must be called once in setup() before use.

### Syntax

```c++
myRobot->oled->initialize();
```
### Parameters

* None.

### Returns

* **boolean**: TRUE if OLED is connected, FALSE if OLED is not connected.

### Notes

* None.

### Example

```c++
// Initialize the oled module, display the splash screen, then do nothing.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  if (!myRobot->oled->initialize())
  {
    Serial.println("Failed to initialize OLED!. Stopping.");
    while (1);
  }
}

void loop() {
  // Use the imu functions here
}
```

### See also

* [tasks()](<#void-tasksvoid>)