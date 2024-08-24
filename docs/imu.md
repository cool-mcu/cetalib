# imu Module

This module provides functions to interact with the [Adafruit LSM6DSOX IMU](https://www.adafruit.com/product/4438) on the [CETA IoT Robot (Kit #18-00019x)](https://www.cool-mcu.com/pages/robot-kit). This module measures linear acceleration and angular velocity, and is primarily used to sense the relative heading of the robot. The sensor also provides ambient temperature measurement.

<img src="../assets/adafruit_imu.jpg?raw=true"><br>

Pico WH GPIO Pin Connections:
* GP18: I2C SDA SIGNAL for ADAFRUIT LSM6DSOX IMU
* GP19: I2C SCL SIGNAL for ADAFRUIT LSM6DSOX IMU

For detailed lessons covering imu components, schematics and step-by-step assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#bool-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [get_temperature()](<#float-get_temperaturevoid>)
* [get_heading()](<#float-get_headingvoid>)
* [reset_heading()](<#void-reset_headingvoid>)

## `bool initialize(void)`

Initiallize pins & state variables & calibrate if needed. Must be called once in setup() before use.

### Syntax

```c++
myRobot->imu->initialize();
```
### Parameters

* None.

### Returns

* **boolean**: TRUE if IMU is connected, FALSE if IMU is not connected.

### Notes

* None.

### Example

```c++
// Initialize the imu module, then do nothing.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  delay(2000);
  if (!myRobot->imu->initialize())
  {
    Serial.println("Failed to initialize IMU!. Stopping.");
    while (1);
  }
}

void loop() {
  // Use the imu functions here
}
```

### See also

* [tasks()](<#void-tasksvoid>)

## `void tasks(void)`

Run all background tasks to maintain heading value. Must be called regularly in the loop() function to maintain an accurate heading measurement.

### Syntax

```c++
myRobot->imu->tasks();
```
### Parameters

* None.

### Returns

* None.

### Notes

* No blocking functions allowed in loop()

### Example

```c++
// Print the current robot heading every second.
// Assumes IMU is calibrated.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define sensor sample interval variables
unsigned long imuSensorCurrentTime, imuSensorPrevTime;
const long imuSensorInterval = 1000; // (sample interval in mS)

float heading; // current robot heading in degrees

void setup() {
  Serial.begin(115200);
  delay(2000);
  if (!myRobot->imu->initialize())
  {
    Serial.println("Failed to initialize IMU!. Stopping.");
    while (1);
  }
}

void loop() {
  myRobot->imu->tasks();
  imuSensorCurrentTime = millis();
  if ((imuSensorCurrentTime - imuSensorPrevTime) >= imuSensorInterval)
  {
    imuSensorPrevTime = imuSensorCurrentTime;
    heading = myRobot->imu->get_heading();
    Serial.print("Heading: ");
    Serial.println(heading); 
  }
}
```

### See also

* [initialize()](<#void-initializevoid>)

## `float get_temperature(void)`

Get the ambient temperature in degrees celcius.

### Syntax

```c++
float temperature = myRobot->imu->get_temperature();
```
### Parameters

* None.

### Returns

* **float**: current ambient temperature in degrees celcius

### Notes

* None.

### Example

```c++
// Print the current ambient temperature every second.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define sensor sample interval variables
unsigned long imuSensorCurrentTime, imuSensorPrevTime;
const long imuSensorInterval = 1000; // (sample interval in mS)

float temperature; // current ambient temperature

void setup() {
  Serial.begin(115200);
  delay(2000);
  if (!myRobot->imu->initialize())
  {
    Serial.println("Failed to initialize IMU!. Stopping.");
    while (1);
  }
}

void loop() {
  myRobot->imu->tasks();
  imuSensorCurrentTime = millis();
  if ((imuSensorCurrentTime - imuSensorPrevTime) >= imuSensorInterval)
  {
    imuSensorPrevTime = imuSensorCurrentTime;
    temperature = myRobot->imu->get_temperature();
    Serial.print("Temperature: ");
    Serial.println(temperature); 
  }
}
```

### See also

* [initialize()](<#void-initializevoid>)

## `float get_heading(void)`

Get the current robot heading ("yaw") (0-360 degrees)

### Syntax

```c++
float heading = myRobot->imu->get_heading();
```
### Parameters

* None.

### Returns

* **float**: current robot heading in degrees (0-360)

### Notes

* IMU Calibration is required for accurate readings. Run the [imu_get_teperature_heading](../examples/imu_get_teperature_heading/imu_get_teperature_heading.ino) sketch to calibrate the IMU.
* No blocking functions in loop(). imu_tasks() must be called frequently.

### Example

```c++
// Print the current robot heading every second.
// Assumes IMU is calibrated.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define sensor sample interval variables
unsigned long imuSensorCurrentTime, imuSensorPrevTime;
const long imuSensorInterval = 1000; // (sample interval in mS)

float heading; // current robot heading in degrees

void setup() {
  Serial.begin(115200);
  delay(2000);
  if (!myRobot->imu->initialize())
  {
    Serial.println("Failed to initialize IMU!. Stopping.");
    while (1);
  }
}

void loop() {
  myRobot->imu->tasks();
  imuSensorCurrentTime = millis();
  if ((imuSensorCurrentTime - imuSensorPrevTime) >= imuSensorInterval)
  {
    imuSensorPrevTime = imuSensorCurrentTime;
    heading = myRobot->imu->get_heading();
    Serial.print("Heading: ");
    Serial.println(heading); 
  }
}
```

### See also

* [initialize()](<#void-initializevoid>)

## `void reset_heading(void)`

Reset the robot heading value.

### Syntax

```c++
myRobot->imu->reset_heading();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Print the current robot heading every second.
// Reset the heading when USER SWITCH is pressed

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define sensor sample interval variables
unsigned long imuSensorCurrentTime, imuSensorPrevTime;
const long imuSensorInterval = 1000; // (sample interval in mS)

float heading; // current robot heading in degrees

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  if (!myRobot->imu->initialize())
  {
    Serial.println("Failed to initialize IMU!. Stopping.");
    while (1);
  }
}

void loop() {
  myRobot->board->tasks();
  myRobot->imu->tasks();
  if(myRobot->board->is_button_pressed())
  {
    myRobot->imu->reset_heading();
  }
  imuSensorCurrentTime = millis();
  if ((imuSensorCurrentTime - imuSensorPrevTime) >= imuSensorInterval)
  {
    imuSensorPrevTime = imuSensorCurrentTime;
    heading = myRobot->imu->get_heading();
    Serial.print("Heading: ");
    Serial.println(heading); 
  }
}
```

### See also

* [initialize()](<#void-initializevoid>)