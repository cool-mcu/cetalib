# board Module

This module provides functions to interact with the USER LED, USER SWITCH and USER POTENTIOMETER on the [CETA IoT Robot (Kit #18-00019x)](https://www.cool-mcu.com/pages/robot-kit).

<img src="../assets/board-assembly.JPG?raw=true">

Pico WH GPIO Pin Connections:
* GP14: USER LED
* GP15: USER SWITCH
* GP28/ADC2: USER POTENTIOMETER

For detailed description of robot components, schematic and step-by-step robot assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#void-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [led_on()](<#void-led_onvoid>)
* [led_off()](<#void-led_offvoid>)
* [led_toggle()](<#void-led_togglevoid>)
* [led_blink()](<#void-led_blinkint-frequency>)
* [led_pattern()](<#void-led_patternint-pattern>)
* [is_button_pressed()](<#bool-is_button_pressedvoid>)
* [is_button_released()](<#bool-is_button_releasedvoid>)

## `void initialize(void)`

Initialize pin settings and module state variables. Must be called once in setup() before use.

### Syntax

```c++
myRobot->board->initialize();
```
### Parameters

None.

### Returns

None.

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

* [initialize()](<#void-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [led_on()](<#void-led_onvoid>)
* [led_toggle()](<#void-led_togglevoid>)

## `void tasks(void)`

Run all background tasks to maintain LED and SWITCH state for certain methods. Must be called regularly in the loop() function.

### Syntax

```c++
myRobot->board->tasks();
```
### Parameters

None.

### Returns

None.

### Example

```c++
#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
}

void loop() {
  myRobot->board->tasks();
  if (myRobot->board->is_button_pressed()) {
    myRobot->board->led_toggle();
  }
}
```

### See also

* [initialize()](<#void initialize(void)>)
* [readAcceleration()](#readacceleration)
* [readGyroscope()](#readgyroscope)
* [accelerationAvailable()](#accelerationavailable)
* [gyroscopeAvailable()](#gyroscopeavailable)
* [accelerationSampleRate()](#accelerationsamplerate)
* [gyroscopeSampleRate()](#gyroscopesamplerate)

## `void led_on(void)`

Turn on the USER LED.

### Syntax

```c++
myRobot->board->led_on();
```
### Parameters

None.

### Returns

None.

### Example

```c++
#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
}

void loop() {
  myRobot->board->led_on();
  delay(100);
  myRobot->board->led_off();
  delay(1000);
}
```
### See also

* [initialize()](<#void initialize(void)>)
* [readAcceleration()](#readacceleration)
* [readGyroscope()](#readgyroscope)
* [accelerationAvailable()](#accelerationavailable)
* [gyroscopeAvailable()](#gyroscopeavailable)
* [accelerationSampleRate()](#accelerationsamplerate)
* [gyroscopeSampleRate()](#gyroscopesamplerate)

## `void led_off(void)`

Turn off the USER LED.

### Syntax

```c++
myRobot->board->led_of();
```
### Parameters

None.

### Returns

None.

### Example

```c++
#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
}

void loop() {
  myRobot->board->led_on();
  delay(100);
  myRobot->board->led_off();
  delay(1000);
}
```
### See also

* [initialize()](<#void initialize(void)>)
* [readAcceleration()](#readacceleration)
* [readGyroscope()](#readgyroscope)
* [accelerationAvailable()](#accelerationavailable)
* [gyroscopeAvailable()](#gyroscopeavailable)
* [accelerationSampleRate()](#accelerationsamplerate)
* [gyroscopeSampleRate()](#gyroscopesamplerate)

## `void led_toggle(void)`

Toggle the USER LED state.

### Syntax

```c++
myRobot->board->led_toggle();
```
### Parameters

None.

### Returns

None.

### Example

```c++
#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
}

void loop() {
  myRobot->board->led_toggle();
  delay(100);
}
```
### See also

* [initialize()](<#void initialize(void)>)
* [readAcceleration()](#readacceleration)
* [readGyroscope()](#readgyroscope)
* [accelerationAvailable()](#accelerationavailable)
* [gyroscopeAvailable()](#gyroscopeavailable)
* [accelerationSampleRate()](#accelerationsamplerate)
* [gyroscopeSampleRate()](#gyroscopesamplerate)

## `void led_blink(int frequency)`

Blink the USER LED at a certain rate.

### Syntax

```c++
myRobot->board->led_blink(10);  // set the USER LED to blink @ 10 Hz
```
### Parameters

* frequency: integer variable defining the blink frequency (range: 1-20 Hz).

### Returns

None.

### Notes

* Must call tasks() regularly in the main loop().
* Use led_off() to disable flashing.

### Example

```c++
#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
  myRobot->board->led_blink(10); // set the USER LED to blink @ 10 Hz
}

void loop() {
  myRobot->board->tasks();
  if (myRobot->board->is_button_pressed()) {
    myRobot->board->led_off();  // turn off USER LED flashing
  }
}
```
### See also

* [initialize()](<#void initialize(void)>)
* [readAcceleration()](#readacceleration)
* [readGyroscope()](#readgyroscope)
* [accelerationAvailable()](#accelerationavailable)
* [gyroscopeAvailable()](#gyroscopeavailable)
* [accelerationSampleRate()](#accelerationsamplerate)
* [gyroscopeSampleRate()](#gyroscopesamplerate)

## `void led_pattern(int pattern)`

Flash a specific "heartbeat" pattern on the USER LED

### Syntax

```c++
myRobot->board->led_pattern(1);  // blink the USER LED using heartbeat pattern 1
```
### Parameters

* pattern: integer variable defining the hearbeat pattern (patterns: 1-5).

### Returns

None.

### Notes

* Must call tasks() regularly in the main loop().
* Use led_off() to disable the flashing.

### Example

```c++
#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
  myRobot->board->led_pattern(2); // blink the USER LED using heartbeat pattern 2
}

void loop() {
  myRobot->board->tasks();
  if (myRobot->board->is_button_pressed()) {
    myRobot->board->led_off();  // turn off USER LED flashing
  }
}
```
### See also

* [initialize()](<#void initialize(void)>)
* [readAcceleration()](#readacceleration)
* [readGyroscope()](#readgyroscope)
* [accelerationAvailable()](#accelerationavailable)
* [gyroscopeAvailable()](#gyroscopeavailable)
* [accelerationSampleRate()](#accelerationsamplerate)
* [gyroscopeSampleRate()](#gyroscopesamplerate)

## `bool is_button_pressed(void)`

Was the USER SWITCH pressed?

### Syntax

```c++
bool isButtonPressed = myRobot->board->is_button_pressed();
```
### Parameters

None.

### Returns

* boolean: TRUE if button was pressed, FALSE if button was not pressed.

### Notes

* Must call tasks() regularly in the main loop().

### Example

```c++
#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
}

void loop() {
  myRobot->board->tasks();
  if (myRobot->board->is_button_pressed()) {
    myRobot->board->led_toggle();
  }
}
```
### See also

* [initialize()](<#void initialize(void)>)
* [readAcceleration()](#readacceleration)
* [readGyroscope()](#readgyroscope)
* [accelerationAvailable()](#accelerationavailable)
* [gyroscopeAvailable()](#gyroscopeavailable)
* [accelerationSampleRate()](#accelerationsamplerate)
* [gyroscopeSampleRate()](#gyroscopesamplerate)

## `bool is_button_released(void)`

Was the USER SWITCH released?

### Syntax

```c++
bool isButtonReleased = myRobot->board->is_button_released();
```
### Parameters

None.

### Returns

* boolean: TRUE if button was released, FALSE if it wasn't.

### Notes

* Must call tasks() regularly in the main loop().

### Example

```c++
#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
}

void loop() {
  myRobot->board->tasks();
  if (myRobot->board->is_button_released()) {
    myRobot->board->led_toggle();
  }
}
```
### See also

* [initialize()](<#void initialize(void)>)
* [readAcceleration()](#readacceleration)
* [readGyroscope()](#readgyroscope)
* [accelerationAvailable()](#accelerationavailable)
* [gyroscopeAvailable()](#gyroscopeavailable)
* [accelerationSampleRate()](#accelerationsamplerate)
* [gyroscopeSampleRate()](#gyroscopesamplerate)