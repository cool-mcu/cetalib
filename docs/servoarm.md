# servoarm Module

This module provides functions to interact with an [SG92R](https://abra-electronics.com/electromechanical/motors/servo-motors/sg92r-micro-servo-9g-2-5kg-torque-90-degrees.html) servo motor to implement basic LIFT/DROP functions for the [CETA IoT Robot (Kit #18-00019x)](https://www.cool-mcu.com/pages/robot-kit).

<img src="../assets/pico-iot-robot-servoarm-addon.jpg?raw=true">

Pico WH GPIO Pin Connections:
* ORANGE wire:  PWM Signal - Connect to GP22
* RED wire:     Connect to VPack voltage
* BROWN wire:   Connect to GND

For detailed lessons covering robot components, schematics and step-by-step robot assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#void-initializevoid>)
* [get_left_sensor()](<#float-get_left_sensorvoid>)
* [get_middle_sensor()](<#float-get_middle_sensorvoid>)
* [get_right_sensor()](<#float-get_right_sensorvoid>)
* [get_line_status()](<#int-get_line_statusvoid>)
* [clear_calibration()](<#void-clear_calibrationvoid>)