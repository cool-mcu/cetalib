# encoder Module

This module provides functions to read the Motor Shaft Encoder outputs on the [SparkFun XRP Robot](https://www.sparkfun.com/experiential-robotics-platform-xrp-kit.html). This information can be used to calculate the speed of the robot, how far it's travelled, or to control precisely the speed of the robot.

<center>
<img src="../../assets/xrp-encoder.jpg?raw=true">
</center><br>

Encoders are sensors which measure how far each motor (and thus each wheel) has rotated. Motors aren't perfect, so when we tell them to go a certain effort we don’t know how fast it is actually rotating. Encoders measure exactly what the motor is doing and report this information back to the XRP.

Each shaft encoder produces 2  square-wave output signals which are processed by the XRP. The frequency of positive pulses indicates the speed of the shaft, while the phase difference between the signals indicates the motor spin direction.

<center>
<img src="../../assets/xrp-encoder-pulses.png?raw=true">
</center><br>

The XRP motors will produce 585 pulses for every wheel revolution.
Given that the wheel diameter is 6 cm, the distance travelled may be easily computed:

<center>distance (cm) = (circumference) * (number of revolutions) = (Pi * 6) * (count/585)</center><br>
 
For detailed lessons covering encoder components, assembly, and functionality, [contact us](mailto:info@cool-mcu.com) to enrol in the [CETA Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/ceta-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#void-initializevoid>)
* [get_left_position()](<#float-get_left_positionvoid>)
* [get_right_position()](<#float-get_right_positionvoid>)
* [get_left_position_counts()](<#int-get_left_position_countsvoid>)
* [get_right_position_counts()](<#int-get_right_position_countsvoid>)
* [reset_left_position()](<#void-reset_left_positionvoid>)
* [reset_right_position()](<#void-reset_right_positionvoid>)

## `void initialize(void)`

Initialize encoder pin settings and PIO decode engine. Must be called once in setup() before use.

### Syntax

```c++
myRobot->encoder->initialize();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Initialize the encoder module, then do nothing.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->encoder->initialize();
}

void loop() {
  // Use the encoder functions here
}
```

### See also

* [initialize()](<#void-initializevoid>)
* [get_left_position()](<#float-get_left_positionvoid>)
* [get_right_position()](<#float-get_right_positionvoid>)
* [get_left_position_counts()](<#int-get_left_position_countsvoid>)
* [get_right_position_counts()](<#int-get_right_position_countsvoid>)
* [reset_left_position()](<#void-reset_left_positionvoid>)
* [reset_right_position()](<#void-reset_right_positionvoid>)

## `float get_left_position(void)`

Return the position of the left motor wheel (in number of revolutions) since last reset.

### Syntax

```c++
float leftPosition = myRobot->encoder->get_left_position();
```
### Parameters

* None.

### Returns

* **float**: The number of revolutions of the left wheel since the last encoder reset

### Notes

* None.

### Example

```c++
// Initialize the encoder module, then measure & display the left motor position

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->encoder->initialize();
}

void loop() {
  // Use the encoder functions here
  float leftPosition = myRobot->encoder->get_left_position();
  Serial.printf("Left Motor Position: %.2f\r\n", leftPosition);
  delay(100);
}
```

### See also

* [initialize()](<#void-initializevoid>)
* [get_left_position()](<#float-get_left_positionvoid>)
* [get_right_position()](<#float-get_right_positionvoid>)
* [get_left_position_counts()](<#int-get_left_position_countsvoid>)
* [get_right_position_counts()](<#int-get_right_position_countsvoid>)
* [reset_left_position()](<#void-reset_left_positionvoid>)
* [reset_right_position()](<#void-reset_right_positionvoid>)

## `float get_right_position(void)`

Return the position of the right motor wheel (in number of revolutions) since last reset.

### Syntax

```c++
float rightPosition = myRobot->encoder->get_right_position();
```
### Parameters

* None.

### Returns

* **float**: The number of revolutions of the right wheel since the last encoder reset

### Notes

* None.

### Example

```c++
// Initialize the encoder module, then measure & display the right motor position

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->encoder->initialize();
}

void loop() {
  // Use the encoder functions here
  float rightPosition = myRobot->encoder->get_right_position();
  Serial.printf("Right Motor Position: %.2f\r\n", rightPosition);
  delay(100);
}
```

### See also

* [initialize()](<#void-initializevoid>)
* [get_left_position()](<#float-get_left_positionvoid>)
* [get_right_position()](<#float-get_right_positionvoid>)
* [get_left_position_counts()](<#int-get_left_position_countsvoid>)
* [get_right_position_counts()](<#int-get_right_position_countsvoid>)
* [reset_left_position()](<#void-reset_left_positionvoid>)
* [reset_right_position()](<#void-reset_right_positionvoid>)

## `int get_left_position_counts(void)`

Return the raw encoder count of the left motor encoder since last reset

### Syntax

```c++
int leftPositionCount = myRobot->encoder->get_left_position_counts();
```
### Parameters

* None.

### Returns

* **int**: The raw encoder count of the left motor encoder since last reset

### Notes

* None.

### Example

```c++
// Initialize the encoder module, then measure & display the left motor encoder counts

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->encoder->initialize();
}

void loop() {
  // Use the encoder functions here
  int leftPositionCount = myRobot->encoder->get_left_position_counts();
  Serial.printf("Left Motor Raw Count: %d\r\n", leftPositionCount);
  delay(100);
}
```

### See also

* [initialize()](<#void-initializevoid>)
* [get_left_position()](<#float-get_left_positionvoid>)
* [get_right_position()](<#float-get_right_positionvoid>)
* [get_left_position_counts()](<#int-get_left_position_countsvoid>)
* [get_right_position_counts()](<#int-get_right_position_countsvoid>)
* [reset_left_position()](<#void-reset_left_positionvoid>)
* [reset_right_position()](<#void-reset_right_positionvoid>)

## `int get_right_position_counts(void)`

Return the raw encoder count of the right motor encoder since last reset

### Syntax

```c++
int rightPositionCount = myRobot->encoder->get_right_position_counts();
```
### Parameters

* None.

### Returns

* **int**: The raw encoder count of the right motor encoder since last reset

### Notes

* None.

### Example

```c++
// Initialize the encoder module, then measure & display the right motor encoder counts

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->encoder->initialize();
}

void loop() {
  // Use the encoder functions here
  int rightPositionCount = myRobot->encoder->get_right_position_counts();
  Serial.printf("Right Motor Raw Count: %d\r\n", rightPositionCount);
  delay(100);
}
```

### See also

* [initialize()](<#void-initializevoid>)
* [get_left_position()](<#float-get_left_positionvoid>)
* [get_right_position()](<#float-get_right_positionvoid>)
* [get_left_position_counts()](<#int-get_left_position_countsvoid>)
* [get_right_position_counts()](<#int-get_right_position_countsvoid>)
* [reset_left_position()](<#void-reset_left_positionvoid>)
* [reset_right_position()](<#void-reset_right_positionvoid>)

## `void reset_left_position(void)`

Reset left encoder count.

### Syntax

```c++
myRobot->encoder->reset_left_position();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Measure & display the left motor position. Reset if button is pressed

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->board->initialize();
  myRobot->encoder->initialize();
}

void loop() {
  // Use the encoder functions here
  myRobot->board->tasks();
  if(myRobot->board->is_button_pressed())
  {
    myRobot->encoder->reset_left_position();
  }
  float leftPosition = myRobot->encoder->get_left_position();
  Serial.printf("Left Motor Position: %.2f\r\n", leftPosition);
  delay(100);
}
```

### See also

* [initialize()](<#void-initializevoid>)
* [get_left_position()](<#float-get_left_positionvoid>)
* [get_right_position()](<#float-get_right_positionvoid>)
* [get_left_position_counts()](<#int-get_left_position_countsvoid>)
* [get_right_position_counts()](<#int-get_right_position_countsvoid>)
* [reset_left_position()](<#void-reset_left_positionvoid>)
* [reset_right_position()](<#void-reset_right_positionvoid>)

## `void reset_right_position(void)`

Reset right encoder count.

### Syntax

```c++
myRobot->encoder->reset_right_position();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Measure & display the right motor position. Reset if button is pressed

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->board->initialize();
  myRobot->encoder->initialize();
}

void loop() {
  // Use the encoder functions here
  myRobot->board->tasks();
  if(myRobot->board->is_button_pressed())
  {
    myRobot->encoder->reset_right_position();
  }
  float rightPosition = myRobot->encoder->get_right_position();
  Serial.printf("Right Motor Position: %.2f\r\n", rightPosition);
  delay(100);
}
```

### See also

* [initialize()](<#void-initializevoid>)
* [get_left_position()](<#float-get_left_positionvoid>)
* [get_right_position()](<#float-get_right_positionvoid>)
* [get_left_position_counts()](<#int-get_left_position_countsvoid>)
* [get_right_position_counts()](<#int-get_right_position_countsvoid>)
* [reset_left_position()](<#void-reset_left_positionvoid>)
* [reset_right_position()](<#void-reset_right_positionvoid>)