# cetalib Library for Arduino

cetalib is an Arduino-based programming library that provides functions to make it easy to use all the features of the [CETA IoT Robot (Pico WH)](https://www.cool-mcu.com/pages/robot-kit)

[<img src="cool-mcu/cetalib/blob/main/assets/arduino-ide-cetalib-code-completion.png?raw=true" width="500px">](https://www.cool-mcu.com/pages/robot-kit)

This robot and library support the learning objectives of our [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators). If you are a teacher and are interested in kick-starting your STEM curriculum, [contact us](mailto:info@cool-mcu.com) to enrol in the curriculum.

## Primary Modules of the Library
* [board](https://github.com/cool-mcu/cetalib/blob/master/docs/board.md)
  * Provides functions to interact with the USER LED, USER SWITCH and USER POTENTIOMETER
* [diffDrive](https://github.com/cool-mcu/cetalib/blob/master/docs/diffDrive.md)
  * Provides functions to control the direction and voltage applied to both DC motors
* [imu](https://github.com/cool-mcu/cetalib/blob/master/docs/imu.md)
  * Provides functions to obtain heading and acceleration data from an onboard LSM6DSOX IMU (Inertial Measurement Unit)
* [mqttc](https://github.com/cool-mcu/cetalib/blob/master/docs/mqttc.md)
  * Provides WiFi and MQTT Client network connectivity functions to allow the robot to be monitored and controlled over the internet
* [rangefinder](https://github.com/cool-mcu/cetalib/blob/master/docs/rangefinder.md)
  * Provides basic functions for using an HC-SR04 Ultrasonic Rangefinder
* [reflectance](https://github.com/cool-mcu/cetalib/blob/master/docs/reflectance.md)
  * Provides basic functions for obtaining readings from the 3 opto line-sensors
* [servoarm](https://github.com/cool-mcu/cetalib/blob/master/docs/servoarm.md)
  * Provides basic functions for controlling a SG92R Servo motor

# Dependencies
* [ArduinoMqttClient](https://github.com/arduino-libraries/ArduinoMqttClient)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [Arduino_LSM6DSOX](https://github.com/arduino-libraries/Arduino_LSM6DSOX)
* [Arduino-Pico Core](https://github.com/earlephilhower/arduino-pico)

# Installation

We recommend use of [Arduino IDE v2.x](https://www.arduino.cc/en/software)

1. Install dependent Core and Libraries in Arduino
2. Clone/download a .zip file of the repository
3. [Import the .zip library into Arduino](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/#importing-a-zip-library) 

# Using the Library
To use this library:
```c++
// Include the library
#include <cetalib.h>

// Define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// Access a function in a specific module
float distance = myRobot->rangefinder->get_distance();
```
The autocomplete feature of the Arduino 2.x IDE automatically lists all available functions for a specific module, simplifying the coding process:

[<img src="assets/arduino-ide-cetalib-code-completion.png?raw=true">](https://www.cool-mcu.com/pages/robot-kit)

# Documentation

See the [docs](https://github.com/cool-mcu/cetalib/tree/master/docs) folder for a detailed description of all modules and APIs.

See the [examples](https://github.com/cool-mcu/cetalib/tree/master/examples) folder for complete example sketches using the library.