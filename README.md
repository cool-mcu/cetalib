# cetalib Library for Arduino

**cetalib** is an Arduino-based programming library that provides functions to make it easy to use all the features of the [CETA IoT Robot (Pico WH)](https://www.cool-mcu.com/pages/robot-kit) and the [SparkFun XRP Robot](https://www.sparkfun.com/experiential-robotics-platform-xrp-kit.html).

[<img src="./assets/ceta-and-xrp-robot.png?raw=true">](https://www.cool-mcu.com/pages/robot-kit)

This robot and library support the learning objectives of our [CETA Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators). If you are a teacher and are interested in kick-starting your STEM curriculum, [contact us](mailto:info@cool-mcu.com) to enrol in the curriculum.

## Primary Modules in the Library (CETA IoT Robot (Pico WH))
* [board](https://github.com/cool-mcu/cetalib/blob/main/docs/ceta-iot-robot/board.md)
  * Provides functions to interact with the USER LED, USER SWITCH and USER POTENTIOMETER
* [diffDrive](https://github.com/cool-mcu/cetalib/blob/main/docs/ceta-iot-robot/diffDrive.md)
  * Provides functions to control the direction and voltage applied to both DC motors
* [imu](https://github.com/cool-mcu/cetalib/blob/main/docs/ceta-iot-robot/imu.md)
  * Provides functions to obtain heading and acceleration data from an onboard LSM6DSOX IMU (Inertial Measurement Unit)
* [mqttc](https://github.com/cool-mcu/cetalib/blob/main/docs/ceta-iot-robot/mqttc.md)
  * Provides WiFi and MQTT Client network connectivity functions to allow the robot to be monitored and controlled over the internet
* [rangefinder](https://github.com/cool-mcu/cetalib/blob/main/docs/ceta-iot-robot/rangefinder.md)
  * Provides basic functions for using an HC-SR04 Ultrasonic Rangefinder
* [reflectance](https://github.com/cool-mcu/cetalib/blob/main/docs/ceta-iot-robot/reflectance.md)
  * Provides basic functions for obtaining readings from the 3 opto line-sensors
* [servoarm](https://github.com/cool-mcu/cetalib/blob/main/docs/ceta-iot-robot/servoarm.md)
  * Provides basic functions for controlling a SG92R Servo motor
* [oled](https://github.com/cool-mcu/cetalib/blob/main/docs/ceta-iot-robot/oled.md)
  * Provides basic text display functions for a 128x64 OLED display

# Dependencies

The following libraries need to be installed into your Arduino environment:

* [ArduinoMqttClient (v0.1.5)](https://github.com/arduino-libraries/ArduinoMqttClient/archive/refs/tags/0.1.5.zip)
* [ArduinoJson (v6.17.2)](https://github.com/bblanchon/ArduinoJson/archive/refs/tags/v6.17.2.zip)
* [Arduino_LSM6DSOX (v1.1.2)](https://github.com/arduino-libraries/Arduino_LSM6DSOX/archive/refs/tags/1.1.2.zip)
* [Adafruit_BusIO (v1.16.1)](https://github.com/adafruit/Adafruit_BusIO/archive/refs/tags/1.16.1.zip)
* [Adafruit_SSD1306 (v2.5.11)](https://github.com/adafruit/Adafruit_SSD1306/archive/refs/tags/2.5.11.zip)
* [Adafruit-GFX-Library (v1.11.10)](https://github.com/adafruit/Adafruit-GFX-Library/archive/refs/tags/1.11.10.zip)
* [SSD1306Ascii Library (v1.3.5)](https://github.com/greiman/SSD1306Ascii/archive/refs/tags/1.3.5.zip)
* [rp2040-encoder-library (v0.2.0)](https://github.com/gbr1/rp2040-encoder-library/archive/refs/tags/0.2.0.zip)

The following core needs to be installed into your Arduino environment using the Boards Manager:
* [Raspberry Pi Pico/RP2040 Core by Earl Philhower](https://github.com/earlephilhower/arduino-pico)

# Installation

We recommend using [Arduino IDE v2.x](https://www.arduino.cc/en/software) with this library.

1. Install dependent Core and Libraries into your Arduino environment
2. Clone/download a .zip file of this repository
3. [Import the .zip libraries into Arduino](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/#importing-a-zip-library) 

The following YouTube playlist provides installation instructions and module usage guides :
[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/OTbfWM7dy2E/0.jpg)](https://youtu.be/SSFTYtvs7AA?list=PLoHXwNc42dpcWivhTyOY2jIawscry8m5y)


# Using the Library
After creating a new sketch, the very first thing you need to do is select the appropriate board for the robot you are using.

The SparkFun XRP Robot uses the **SparkFun XRP Controller** board type:

<img src="./assets/xrp_board_type.jpg?raw=true">

While the CETA IoT Robot (Pico WH) uses the **Raspberry Pi Pico W** board type:

<img src="./assets/ceta_board_type.jpg?raw=true">

Next, to access the library modules in a sketch:
* #include the cetalib.h header file at the top of the sketch
* Define and initialize a pointer to the CETALIB INTERFACE functions
* Access a function in a specific module using the arrow operator ("->")

For example:
```c++
// Include the library
#include <cetalib.h>

// Define & initialize a pointer to the CETALIB INTERFACE functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// Access a function in a specific module
float distance = myRobot->rangefinder->get_distance();
```
The autocomplete feature of the Arduino 2.x IDE automatically lists all available modules, then functions for a specific module, simplifying the coding process:

<image src="./assets/arduino-ide-autocomplete.gif?raw=true">

# Documentation

See the [docs](https://github.com/cool-mcu/cetalib/tree/main/docs) folder for a detailed description of all modules and APIs, along with simple example sketches you can try in Arduino.

The [examples](https://github.com/cool-mcu/cetalib/tree/main/examples) folder provides additional example sketches using the library.