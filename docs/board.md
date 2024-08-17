# board Module

This module provides functions to interact with the USER LED, USER SWITCH and USER POTENTIOMETER on the [CETA IoT Robot (Pico WH)](https://www.cool-mcu.com/pages/robot-kit)

Pico WH GPIO Pin Connections:
* GP14: USER LED
* GP15: USER SWITCH
* GP28/ADC2: USER POTENTIOMETER

For detailed schematic and step-by-step robot assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

Methods:
* [initialize()](<#void initialize(void)>)
* [tasks()](<#void tasks(void)>)

## Methods

### `void initialize(void)`

Initialize pin settings and module state variables. Must be called once in Setup() before use.

#### Syntax

```c++
myRobot->board->initialize();
```
#### Parameters

None.

#### Returns

None.

#### Example

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

#### See also

* [initialize()](<#void initialize(void)>)
* [tasks()](<#void tasks(void)>)
* [readGyroscope()](#readgyroscope)
* [accelerationAvailable()](#accelerationavailable)
* [gyroscopeAvailable()](#gyroscopeavailable)
* [accelerationSampleRate()](#accelerationsamplerate)
* [gyroscopeSampleRate()](#gyroscopesamplerate)

---
### `void tasks(void)`

Run all background tasks to maintain LED and SWITCH state.

#### Syntax

```c++
myRobot->board->tasks();
```
#### Parameters

None.

#### Returns

None.

#### Example

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

#### See also

* [initialize()](<#void initialize(void)>)
* [readAcceleration()](#readacceleration)
* [readGyroscope()](#readgyroscope)
* [accelerationAvailable()](#accelerationavailable)
* [gyroscopeAvailable()](#gyroscopeavailable)
* [accelerationSampleRate()](#accelerationsamplerate)
* [gyroscopeSampleRate()](#gyroscopesamplerate)