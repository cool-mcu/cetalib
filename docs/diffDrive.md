# diffDrive Module

The [CETA IoT Robot](https://www.cool-mcu.com/pages/robot-kit) is a mobile robot platform where driving from one place to another is central to the design of any program. The "differential Drive" module makes driving easy and has functions to:

* **Set the motor efforts**, which is the average voltage suplied to the motors.
* **Make point turns** for a desired number of degrees, either clockwise or countercloskwise

The current robot design does not provide wheel speed feedback, so the robot will settle on an arbitrary speed, given specific motor effort settings. 

<img src="../assets/diffDrive-assembly.jpg?raw=true"><br>
  
Pico WH GPIO Pin Connections:
* GP4: LEFT MOTOR SERVO SIGNAL
* GP5: RIGHT MOTOR SERVO SIGNAL

For a detailed description of differential drive components and functionality, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#void-initializebool-left_flip_dir,-bool-right_flip_dir>)
* [tasks()](<#void-tasksvoid>)
* [led_on()](<#void-led_onvoid>)
* [led_off()](<#void-led_offvoid>)
* [led_toggle()](<#void-led_togglevoid>)
* [led_blink()](<#void-led_blinkint-frequency>)
* [led_pattern()](<#void-led_patternint-pattern>)
* [is_button_pressed()](<#bool-is_button_pressedvoid>)
* [is_button_released()](<#bool-is_button_releasedvoid>)
* [get_button_level()](<#int-get_button_levelvoid>)
* [wait_for_button()](<#void-wait_for_buttonvoid>)
* [get_potentiometer()](<#int-get_potentiometervoid>)

## `void initialize(bool left_flip_dir, bool right_flip_dir)`

Initialize pin settings and module state variables. Must be called once in setup() before use.

### Syntax

```c++
myRobot->diffDrive->initialize(false, false);
```
### Parameters

* left_flip_dir: boolean variable to set left motor spin direction
* right_flip_dir: boolean variable to set right motor spin direction

These parameters can compensate for reversed wiring on the motor terminals in software.

### Returns

* None.

### Example

```c++
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
* [led_on()](<#void-led_onvoid>)
* [led_off()](<#void-led_offvoid>)
* [led_toggle()](<#void-led_togglevoid>)
* [led_blink()](<#void-led_blinkint-frequency>)
* [led_pattern()](<#void-led_patternint-pattern>)
* [is_button_pressed()](<#bool-is_button_pressedvoid>)
* [is_button_released()](<#bool-is_button_releasedvoid>)
* [get_button_level()](<#int-get_button_levelvoid>)
* [wait_for_button()](<#void-wait_for_buttonvoid>)
* [get_potentiometer()](<#int-get_potentiometervoid>)