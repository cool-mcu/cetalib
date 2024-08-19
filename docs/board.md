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
* [get_button_level()](<#int-get_button_levelvoid>)
* [wait_for_button()](<#void-wait_for_buttonvoid>)
* [get_potentiometer()](<#int-get_potentiometervoid>)

## `void initialize(void)`

Initialize pin settings and module state variables. Must be called once in setup() before use.

### Syntax

```c++
myRobot->board->initialize();
```
### Parameters

* None.

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

## `void tasks(void)`

Run all background tasks to maintain LED and SWITCH state for certain methods. Must be called regularly in the loop() function.

### Syntax

```c++
myRobot->board->tasks();
```
### Parameters

* None.

### Returns

* None.

### Example

```c++
// Toggle the USER LED whenever the USER SWITCH is pressed

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

* [initialize()](<#void-initializevoid>)
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

## `void led_on(void)`

Turn on the USER LED.

### Syntax

```c++
myRobot->board->led_on();
```
### Parameters

* None.

### Returns

* None.

### Example

```c++
// Blink the USER LED once every second

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

* [initialize()](<#void-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [led_off()](<#void-led_offvoid>)
* [led_toggle()](<#void-led_togglevoid>)
* [led_blink()](<#void-led_blinkint-frequency>)
* [led_pattern()](<#void-led_patternint-pattern>)
* [is_button_pressed()](<#bool-is_button_pressedvoid>)
* [is_button_released()](<#bool-is_button_releasedvoid>)
* [get_button_level()](<#int-get_button_levelvoid>)
* [wait_for_button()](<#void-wait_for_buttonvoid>)
* [get_potentiometer()](<#int-get_potentiometervoid>)

## `void led_off(void)`

Turn off the USER LED.

### Syntax

```c++
myRobot->board->led_off();
```
### Parameters

* None.

### Returns

* None.

### Example

```c++
// Blink the USER LED once every second

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

* [initialize()](<#void-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [led_on()](<#void-led_onvoid>)
* [led_toggle()](<#void-led_togglevoid>)
* [led_blink()](<#void-led_blinkint-frequency>)
* [led_pattern()](<#void-led_patternint-pattern>)
* [is_button_pressed()](<#bool-is_button_pressedvoid>)
* [is_button_released()](<#bool-is_button_releasedvoid>)
* [get_button_level()](<#int-get_button_levelvoid>)
* [wait_for_button()](<#void-wait_for_buttonvoid>)
* [get_potentiometer()](<#int-get_potentiometervoid>)

## `void led_toggle(void)`

Toggle the USER LED state.

### Syntax

```c++
myRobot->board->led_toggle();
```
### Parameters

* None.

### Returns

* None.

### Example

```c++
// Toggle the USER LED every 100mS

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

* [initialize()](<#void-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [led_on()](<#void-led_onvoid>)
* [led_off()](<#void-led_offvoid>)
* [led_blink()](<#void-led_blinkint-frequency>)
* [led_pattern()](<#void-led_patternint-pattern>)
* [is_button_pressed()](<#bool-is_button_pressedvoid>)
* [is_button_released()](<#bool-is_button_releasedvoid>)
* [get_button_level()](<#int-get_button_levelvoid>)
* [wait_for_button()](<#void-wait_for_buttonvoid>)
* [get_potentiometer()](<#int-get_potentiometervoid>)

## `void led_blink(int frequency)`

Blink the USER LED at a certain rate.

### Syntax

```c++
myRobot->board->led_blink(10);  // set the USER LED to blink @ 10 Hz
```
### Parameters

* **frequency**: integer variable defining the blink frequency (range: 1-20 Hz).

### Returns

* None.

### Notes

* Must call tasks() regularly in the main loop().
* Use led_off() to disable flashing.

### Example

```c++
// Set USER LED to blink @ 10Hz. Turn it off when USER SWITCH is pressed.

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

* [initialize()](<#void-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [led_on()](<#void-led_onvoid>)
* [led_off()](<#void-led_offvoid>)
* [led_toggle()](<#void-led_togglevoid>)
* [led_pattern()](<#void-led_patternint-pattern>)
* [is_button_pressed()](<#bool-is_button_pressedvoid>)
* [is_button_released()](<#bool-is_button_releasedvoid>)
* [get_button_level()](<#int-get_button_levelvoid>)
* [wait_for_button()](<#void-wait_for_buttonvoid>)
* [get_potentiometer()](<#int-get_potentiometervoid>)

## `void led_pattern(int pattern)`

Flash a specific "heartbeat" pattern on the USER LED

### Syntax

```c++
myRobot->board->led_pattern(1);  // blink the USER LED using heartbeat pattern 1
```
### Parameters

* **pattern**: integer variable defining the hearbeat pattern (patterns: 1-5).

### Returns

* None.

### Notes

* Must call tasks() regularly in the main loop().
* Use led_off() to disable the flashing.

### Example

```c++
// Enable USER LED heartbeat pattern #2. Disable flashing when USER SWITCH pressed.

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

* [initialize()](<#void-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [led_on()](<#void-led_onvoid>)
* [led_off()](<#void-led_offvoid>)
* [led_toggle()](<#void-led_togglevoid>)
* [led_blink()](<#void-led_blinkint-frequency>)
* [is_button_pressed()](<#bool-is_button_pressedvoid>)
* [is_button_released()](<#bool-is_button_releasedvoid>)
* [get_button_level()](<#int-get_button_levelvoid>)
* [wait_for_button()](<#void-wait_for_buttonvoid>)
* [get_potentiometer()](<#int-get_potentiometervoid>)

## `bool is_button_pressed(void)`

Was the USER SWITCH pressed?

### Syntax

```c++
bool isButtonPressed = myRobot->board->is_button_pressed();
```
### Parameters

* None.

### Returns

* **boolean**: TRUE if button was pressed, FALSE if button was not pressed.

### Notes

* Must call tasks() regularly in the main loop().

### Example

```c++
// Toggle the USER LED whenever the USER SWITCH is pressed.

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

* [initialize()](<#void-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [led_on()](<#void-led_onvoid>)
* [led_off()](<#void-led_offvoid>)
* [led_toggle()](<#void-led_togglevoid>)
* [led_blink()](<#void-led_blinkint-frequency>)
* [led_pattern()](<#void-led_patternint-pattern>)
* [is_button_released()](<#bool-is_button_releasedvoid>)
* [get_button_level()](<#int-get_button_levelvoid>)
* [wait_for_button()](<#void-wait_for_buttonvoid>)
* [get_potentiometer()](<#int-get_potentiometervoid>)

## `bool is_button_released(void)`

Was the USER SWITCH released?

### Syntax

```c++
bool isButtonReleased = myRobot->board->is_button_released();
```
### Parameters

* None.

### Returns

* **boolean**: TRUE if button was released, FALSE if it wasn't.

### Notes

* Must call tasks() regularly in the main loop().

### Example

```c++
// Toggle the USER LED whenever the USER SWITCH is released.

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

* [initialize()](<#void-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [led_on()](<#void-led_onvoid>)
* [led_off()](<#void-led_offvoid>)
* [led_toggle()](<#void-led_togglevoid>)
* [led_blink()](<#void-led_blinkint-frequency>)
* [led_pattern()](<#void-led_patternint-pattern>)
* [is_button_pressed()](<#bool-is_button_pressedvoid>)
* [get_button_level()](<#int-get_button_levelvoid>)
* [wait_for_button()](<#void-wait_for_buttonvoid>)
* [get_potentiometer()](<#int-get_potentiometervoid>)

## `int get_button_level(void)`

Obtain the current pushbutton state.

### Syntax

```c++
int buttonLevel = myRobot->board->get_button_level();
```
### Parameters

* None.

### Returns

* **integer**: 0 if USER SWITCH is pressed, 1 if USER SWITCH is not pressed.

### Notes

* None.

### Example

```c++
// Set USER LED state to match current USER SWITCH state (pressed/released)

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
}

void loop() {
  if (0 == myRobot->board->get_button_level()) {
    myRobot->board->led_on();
  }
  else {
    myRobot->board->led_off();
  }
}
```
### See also

* [initialize()](<#void-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [led_on()](<#void-led_onvoid>)
* [led_off()](<#void-led_offvoid>)
* [led_toggle()](<#void-led_togglevoid>)
* [led_blink()](<#void-led_blinkint-frequency>)
* [led_pattern()](<#void-led_patternint-pattern>)
* [is_button_pressed()](<#bool-is_button_pressedvoid>)
* [is_button_released()](<#bool-is_button_releasedvoid>)
* [wait_for_button()](<#void-wait_for_buttonvoid>)
* [get_potentiometer()](<#int-get_potentiometervoid>)

## `void wait_for_button(void)`

Blocking function. Waits for the USER BUTTON to be pressed.

### Syntax

```c++
myRobot->board->wait_for_button();
```
### Parameters

* None.

### Returns

* None.

### Notes

* Beware - this is a blocking function!
* Any LED flashing or blinking function is preserved while waiting for the pushbutton.

### Example

```c++
// Wait for USER SWITCH to be pressed, then enable USER LED flashing pattern #3

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
  myRobot->board->wait_for_button(); // block here and wait for the pushbutton press
  myRobot->board->led_pattern(3);
}

void loop() {
  myRobot->board->tasks();  // flash the led pattern
}
```
### See also

* [initialize()](<#void-initializevoid>)
* [tasks()](<#void-tasksvoid>)
* [led_on()](<#void-led_onvoid>)
* [led_off()](<#void-led_offvoid>)
* [led_toggle()](<#void-led_togglevoid>)
* [led_blink()](<#void-led_blinkint-frequency>)
* [led_pattern()](<#void-led_patternint-pattern>)
* [is_button_pressed()](<#bool-is_button_pressedvoid>)
* [is_button_released()](<#bool-is_button_releasedvoid>)
* [get_button_level()](<#int-get_button_levelvoid>)
* [get_potentiometer()](<#int-get_potentiometervoid>)

## `int get_potentiometer(void)`

Get current potentiometer reading (0-4095).

### Syntax

```c++
int potValue = myRobot->board->get_potentiometer();
```
### Parameters

* None.

### Returns

* **integer**: Digitized value of current potentiometer voltage (0 - 4095).

### Notes

* Pico input pin GP28/ADC2 is shared with both the potentiometer signal and the LEFT OPTO signal, so ensure that the POTENTIOMETER signal is connected to the ADC2 input before using this function, using a wire jumper or slider switch.

### Example

```c++
// Read/Display the USER POTENTIOMETER value every second

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

int potValue;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
}

void loop() {
  potValue = myRobot->board->get_potentiometer();
  Serial.println(potValue);
  delay(1000);
}
```
### See also

* [initialize()](<#void-initializevoid>)
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