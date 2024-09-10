# reflectance Module

This module provides functions to interact with the LEFT OPTO, MIDDLE OPTO and
RIGHT OPTO reflectance sensors on the [CETA IoT Robot (Kit #18-00019x)](https://www.cool-mcu.com/pages/robot-kit).

<img src="../assets/reflectance-sensors.jpg?raw=true">

Pico WH GPIO Pin Connections:
* GP26/ADC0: LEFT OPTO sensor
* GP27/ADC1: MIDDLE OPTO sensor
* GP28/ADC2: RIGHT OPTO sensor / USER POTENTIOMETER

For detailed lessons covering robot components, schematics and step-by-step robot assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#void-initializevoid>)
* [get_left_sensor()](<#float-get_left_sensorvoid>)
* [get_middle_sensor()](<#float-get_middle_sensorvoid>)
* [get_right_sensor()](<#float-get_right_sensorvoid>)
* [get_line_status()](<#int-get_line_statusvoid>)

## `void initialize(void)`

Initiallize pins, state variables, calibrate if needed.

### Syntax

```c++
myRobot->reflectance->initialize();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Initialize the reflectance module, then do nothing.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->reflectance->initialize();
}

void loop() {
  // Use the reflectance functions here
}
```

### See also

* [tasks()](<#void-tasksvoid>)

## `float get_left_sensor(void)`

Sample the LEFT OPTO reflectance sensor reading.

### Syntax

```c++
float left_opto = myRobot->reflectance->get_left_sensor();
```
### Parameters

* None.

### Returns

* **float**: current reflectance sensor reading (0 to 1)

### Notes

* Readings closer to 1 correspond to the sensor being over a black line

### Example

```c++
// Print the LEFT OPTO reflectance sensor value every second.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

float left_opto;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->reflectance->initialize();
}

void loop() {
  left_opto = myRobot->reflectance->get_left_sensor();
  Serial.print("Left Opto: ");
  Serial.print(left_opto);
  Serial.println();
  delay(1000);
}
```

### See also

* [initialize()](<#bool-initializevoid>)

## `float get_middle_sensor(void)`

Sample the MIDDLE OPTO reflectance sensor reading.

### Syntax

```c++
float middle_opto = myRobot->reflectance->get_middle_sensor();
```
### Parameters

* None.

### Returns

* **float**: current reflectance sensor reading (0 to 1)

### Notes

* Readings closer to 1 correspond to the sensor being over a black line

### Example

```c++
// Print the MIDDLE OPTO reflectance sensor value every second.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

float middle_opto;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->reflectance->initialize();
}

void loop() {
  middle_opto = myRobot->reflectance->get_middle_sensor();
  Serial.print("Middle Opto: ");
  Serial.print(middle_opto);
  Serial.println();
  delay(1000);
}
```

### See also

* [initialize()](<#bool-initializevoid>)

## `float get_right_sensor(void)`

Sample the RIGHT OPTO reflectance sensor reading.

### Syntax

```c++
float right_opto = myRobot->reflectance->get_right_sensor();
```
### Parameters

* None.

### Returns

* **float**: current reflectance sensor reading (0 to 1)

### Notes

* Readings closer to 1 correspond to the sensor being over a black line

### Example

```c++
// Print the RIGHT OPTO reflectance sensor value every second.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

float right_opto;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->reflectance->initialize();
}

void loop() {
  right_opto = myRobot->reflectance->get_right_sensor();
  Serial.print("Right Opto: ");
  Serial.print(right_opto);
  Serial.println();
  delay(1000);
}
```

### See also

* [initialize()](<#bool-initializevoid>)

## `int get_line_status(void)`

Sample/Return current line detection status.

### Syntax

```c++
int line_detect = myRobot->reflectance->get_line_status();
```
### Parameters

* None.

### Returns

* **int**: current line detection alarm status (0 to 7)

  * 0: (Left: 0, Middle: 0, Right: 0)
  * 1: (Left: 0, Middle: 0, Right: 1)
  * 2: (Left: 0, Middle: 1, Right: 0)  // Robot centered over line
  * 3: (Left: 0, Middle: 1, Right: 1)
  * 4: (Left: 1, Middle: 0, Right: 0)
  * 5: (Left: 1, Middle: 0, Right: 1)
  * 6: (Left: 1, Middle: 1, Right: 0)
  * 7: (Left: 1, Middle: 1, Right: 1) 

### Notes

* The function requires a calibration procedure, which samples, then calculates/saves optimal
OPTO sensor trip thesholds into EEPROM memory.
* The calibration procedure is triggered by calling the "reflectance->initialize()" function after clearing the calibration memory using the "reflectance->clear_calibration()" function as shown below.
    * See code example below
    * Follow the instructions provided in the serial terminal window to complete the calibration procedure

### Example

```c++
// Sample/display the line detection status value every second.
// If the USER SWITCH is pressed on reset, trigger a OPTO calibration sequence

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

int line_detect;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  if (0 == myRobot->board->get_button_level())
  {
    myRobot->reflectance->clear_calibration();
  }
  myRobot->reflectance->initialize();
}

void loop() {
  line_detect = myRobot->reflectance->get_line_status();
  switch (line_detect)
  {
    case 0:
      Serial.println("Left: 0, Middle: 0, Right: 0");
      break;
    case 1:
      Serial.println("Left: 0, Middle: 0, Right: 1");
      break;
    case 2:
      Serial.println("Left: 0, Middle: 1, Right: 0");
      break;
    case 3:
      Serial.println("Left: 0, Middle: 1, Right: 1");
      break;
    case 4:
      Serial.println("Left: 1, Middle: 0, Right: 0");
      break;
    case 5:
      Serial.println("Left: 1, Middle: 0, Right: 1");
      break;
    case 6:
      Serial.println("Left: 1, Middle: 1, Right: 0");
      break;
    case 7:
      Serial.println("Left: 1, Middle: 1, Right: 1");
      break;
    default:
      break;
  }
  delay(1000);
}
```

### See also

* [initialize()](<#bool-initializevoid>)