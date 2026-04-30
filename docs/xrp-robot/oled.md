# oled Module

This module provides text display functions for the [Adafruit 128x64 OLED](https://www.adafruit.com/product/938) add-on to the [SparkFun XRP Robot](https://www.sparkfun.com/experiential-robotics-platform-xrp-kit.html).

<center><img src="../../assets/xrp-oled.jpg?raw=true"></center><br>

XRP Controller RP2350 MCU GPIO Pin Connections (connect to QWIIC 0):
* GP4: I2C SDA SIGNAL for ADAFRUIT 128x64 OLED
* GP5: I2C SCL SIGNAL for ADAFRUIT 128x64 OLED

For detailed lessons covering oled components, schematics and step-by-step assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [CETA Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/ceta-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#bool-initializevoid>)
* [print()](<#void-printchar-str>)
* [println()](<#void-printlnchar-str>)
* [clear()](<#void-clearvoid>)
* [home()](<#void-homevoid>)

## `bool initialize(void)`

Initialize I2C bus, display splash screen for 5 seconds, clear screen and place cursor in home position. Must be called once in setup() before use.

### Syntax

```c++
myRobot->oled->initialize();
```
### Parameters

* None.

### Returns

* **boolean**: TRUE if OLED is connected, FALSE if OLED is not connected.

### Notes

* None.

### Example

```c++
// Initialize the oled module, display the splash screen, then do nothing.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  if (!myRobot->oled->initialize())
  {
    Serial.println("Failed to initialize OLED!. Stopping.");
    while (1);
  }
}

void loop() {
  // Use the oled functions here
}
```

### See also

* [print()](<#void-printchar-str>)
* [println()](<#void-printlnchar-str>)
* [clear()](<#void-clearvoid>)
* [home()](<#void-homevoid>)

## `void print(char *str)`

Prints up to 21 characters of the passed string on the current OLED line. The remaining characters are truncated.

### Syntax

```c++
char *str = "Hello, World!";
myRobot->oled->print(str);
```
### Parameters

* **str**: pointer to char array (C string) containing the desired message

### Returns

* None.

### Notes

* Use the stdio function "sprintf()" to format the text string before printing to the OLED.
* Character '\n' can be used to move the cursor to the next line in the display, which will auto-scroll the display.
  * oled->print("hello\n"); produces the same output as oled->println("hello");

### Example

```c++
// Initialize the oled module, then prints a random number (0-4096) in the home position every second

#include <cetalib.h>
#include <stdio.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

char oledOutBuffer[64];

void setup() {
  Serial.begin(115200);
  while(!Serial);
  if (!myRobot->oled->initialize())
  {
    Serial.println("Failed to initialize OLED!. Stopping.");
    while (1);
  }
}

void loop() {
  sprintf(oledOutBuffer, "Random Number: %4d", random(0, 4096));
  myRobot->oled->print(oledOutBuffer);
  myRobot->oled->home();
  delay(1000);
}
```

### See also

* [initialize()](<#bool-initializevoid>)
* [println()](<#void-printlnchar-str>)
* [clear()](<#void-clearvoid>)
* [home()](<#void-homevoid>)

## `void println(char *str)`

Prints up to 21 characters of the passed string on the current OLED line, adding a New Line character ("\n") which advances the cursor to the next line. The remaining characters are truncated.

Auto-scrolling is enabled on the display.

### Syntax

```c++
char *str = "Hello, World!";
myRobot->oled->println(str);
```
### Parameters

* **str**: pointer to char array (C string) containing the desired message

### Returns

* None.

### Notes

* Use the stdio function "sprintf()" to format the text string before printing to the OLED.
* oled->println("hello"); produces the same output as oled->print("hello\n");

### Example

```c++
// Initialize the oled module, then print a random number (0-4096) on a new line, demonstrating the auto-scroll functionality of the display.

#include <cetalib.h>
#include <stdio.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

char oledOutBuffer[64];

void setup() {
  Serial.begin(115200);
  while(!Serial);
  if (!myRobot->oled->initialize())
  {
    Serial.println("Failed to initialize OLED!. Stopping.");
    while (1);
  }
}

void loop() {
  sprintf(oledOutBuffer, "Random Number: %4d", random(0, 4096));
  myRobot->oled->println(oledOutBuffer);
  delay(1000);
}
```

### See also

* [initialize()](<#bool-initializevoid>)
* [print()](<#void-printchar-str>)
* [clear()](<#void-clearvoid>)
* [home()](<#void-homevoid>)

## `void clear(void)`

Clear the screen.

### Syntax

```c++
myRobot->oled->clear();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Print a random number (0-4096) on a new line, and clear the screen when the user switch is pressed.

#include <cetalib.h>
#include <stdio.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

char oledOutBuffer[64];

void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->board->initialize();
  if (!myRobot->oled->initialize())
  {
    Serial.println("Failed to initialize OLED!. Stopping.");
    while (1);
  }
}

void loop() {
  myRobot->board->tasks();
  sprintf(oledOutBuffer, "Random Number: %4d", random(0, 4096));
  myRobot->oled->println(oledOutBuffer);
  if (myRobot->board->is_button_pressed())
  {
    myRobot->oled->clear();
  }
  delay(250);
}
```

### See also

* [initialize()](<#bool-initializevoid>)
* [print()](<#void-printchar-str>)
* [println()](<#void-printlnchar-str>)
* [home()](<#void-homevoid>)

## `void home(void)`

Place the cursor at position 0, 0.

### Syntax

```c++
myRobot->oled->home();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Print a random number (0-4096) on a new line, and move the cursor to the home position when the user switch is pressed.

#include <cetalib.h>
#include <stdio.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

char oledOutBuffer[64];

void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->board->initialize();
  if (!myRobot->oled->initialize())
  {
    Serial.println("Failed to initialize OLED!. Stopping.");
    while (1);
  }
}

void loop() {
  myRobot->board->tasks();
  sprintf(oledOutBuffer, "Random Number: %4d", random(0, 4096));
  myRobot->oled->println(oledOutBuffer);
  if (myRobot->board->is_button_pressed())
  {
    myRobot->oled->home();
  }
  delay(250);
}
```

### See also

* [initialize()](<#bool-initializevoid>)
* [print()](<#void-printchar-str>)
* [println()](<#void-printlnchar-str>)
* [clear()](<#void-clearvoid>)