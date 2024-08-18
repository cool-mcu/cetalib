# cetalib Library for Arduino

**cetalib** is an Arduino-based programming library that provides functions to make it easy to use all the features of the [CETA IoT Robot (Pico WH)](https://www.cool-mcu.com/pages/robot-kit)

[<img src="./assets/pico-iot-robot-complete-hi-res.JPG?raw=true">](https://www.cool-mcu.com/pages/robot-kit)

This robot and library support the learning objectives of our [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators). If you are a teacher and are interested in kick-starting your STEM curriculum, [contact us](mailto:info@cool-mcu.com) to enrol in the curriculum.

## Primary Modules in the Library
* [board](https://github.com/cool-mcu/cetalib/blob/main/docs/board.md)
  * Provides functions to interact with the USER LED, USER SWITCH and USER POTENTIOMETER
* [diffDrive](https://github.com/cool-mcu/cetalib/blob/main/docs/diffDrive.md)
  * Provides functions to control the direction and voltage applied to both DC motors
* [imu](https://github.com/cool-mcu/cetalib/blob/main/docs/imu.md)
  * Provides functions to obtain heading and acceleration data from an onboard LSM6DSOX IMU (Inertial Measurement Unit)
* [mqttc](https://github.com/cool-mcu/cetalib/blob/main/docs/mqttc.md)
  * Provides WiFi and MQTT Client network connectivity functions to allow the robot to be monitored and controlled over the internet
* [rangefinder](https://github.com/cool-mcu/cetalib/blob/main/docs/rangefinder.md)
  * Provides basic functions for using an HC-SR04 Ultrasonic Rangefinder
* [reflectance](https://github.com/cool-mcu/cetalib/blob/main/docs/reflectance.md)
  * Provides basic functions for obtaining readings from the 3 opto line-sensors
* [servoarm](https://github.com/cool-mcu/cetalib/blob/main/docs/servoarm.md)
  * Provides basic functions for controlling a SG92R Servo motor

# Dependencies

The following libraries and cores need to be installed into your Arduino environment:

* [ArduinoMqttClient](https://github.com/arduino-libraries/ArduinoMqttClient)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [Arduino_LSM6DSOX](https://github.com/arduino-libraries/Arduino_LSM6DSOX)
* [Arduino-Pico Core](https://github.com/earlephilhower/arduino-pico)

# Installation

We recommend using [Arduino IDE v2.x](https://www.arduino.cc/en/software) with this library.

1. Install dependent Core and Libraries into your Arduino environment
2. Clone/download a .zip file of this repository
3. [Import the .zip library into Arduino](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/#importing-a-zip-library) 

# Using the Library
To use this library:
```c++
// Include the library
#include <cetalib.h>

// Define & initialize a pointer to the CETALIB INTERFACE functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// Access a function in a specific module
float distance = myRobot->rangefinder->get_distance();
```
The autocomplete feature of the Arduino 2.x IDE automatically lists all available modules, then functions for a specific module, simplifying the coding process:

[<video src="./assets/arduino-ide-autocomplete.mp4?raw=true" width="640" height="480" controls>]

# Documentation

See the [docs](https://github.com/cool-mcu/cetalib/tree/main/docs) folder for a detailed description of all modules and APIs.

See the [examples](https://github.com/cool-mcu/cetalib/tree/main/examples) folder for complete example sketches using the library.