# board Module

This module provides functions to interact with the USER LED and USER SWITCH on the [SparkFun XRP Robot](https://www.sparkfun.com/experiential-robotics-platform-xrp-kit.html).

<center>
<img src="../../assets/xrp-board.jpg?raw=true">
</center><br>

XRP Controller RP2350 MCU GPIO Pin Connections:
* GP37: USER LED (Uses the WS2812 RGB LED).
* GP36: USER SWITCH

Note that the RGB led is configured to use a fixed color (RED).

For detailed lessons covering robot components, schematics and step-by-step robot assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [CETA Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/ceta-robotics-and-iot-curriculum-for-pre-university-educators).

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

### Notes

* None.

### Example

```c++
// Initialize the board module, then do nothing.

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

### Notes

* None.

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
* [led_on()](<#void-led_onvoid>)
* [led_off()](<#void-led_offvoid>)
* [led_toggle()](<#void-led_togglevoid>)
* [led_blink()](<#void-led_blinkint-frequency>)
* [led_pattern()](<#void-led_patternint-pattern>)
* [is_button_pressed()](<#bool-is_button_pressedvoid>)
* [is_button_released()](<#bool-is_button_releasedvoid>)
* [get_button_level()](<#int-get_button_levelvoid>)
* [wait_for_button()](<#void-wait_for_buttonvoid>)

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

### Notes

* None.

### Example

```c++
// Blink the USER LED once every second.

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

### Notes

* None.

### Example

```c++
// Blink the USER LED once every second.

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

### Notes

* None.

### Example

```c++
// Toggle the USER LED every 100mS.

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
// Set USER LED state to match current USER SWITCH state (pressed/released).

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
// Wait for USER SWITCH to be pressed, then enable USER LED flashing pattern #3.

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