# oled Module

This module provides text display functions for the [Adafruit 128x64 OLED](https://www.adafruit.com/product/938) add-on to the [CETA IoT Robot (Kit #18-00019x)](https://www.cool-mcu.com/pages/robot-kit).

<img src="../assets/pico-iot-robot-oled-addon.jpg?raw=true"><br>

Pico WH GPIO Pin Connections:
* GP18: I2C SDA SIGNAL for ADAFRUIT 128x64 OLED
* GP19: I2C SCL SIGNAL for ADAFRUIT 128x64 OLED

This component uses the same I2C bus instance as the IMU.

For detailed lessons covering oled components, schematics and step-by-step assembly instructions, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#bool-initializevoid>)
* [print()](<#void-printchar-str>)
* [println()](<#void-printlnchar-str>)

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
  delay(2000);
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

* [tasks()](<#void-tasksvoid>)

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
// Initialize the oled module, then print the Potentiometer value in the home position

#include <cetalib.h>
#include <stdio.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

char oledOutBuffer[64];
int potValue;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  if (!myRobot->oled->initialize())
  {
    Serial.println("Failed to initialize OLED!. Stopping.");
    while (1);
  }
}

void loop() {
  potValue = myRobot->board->get_potentiometer();
  sprintf(oledOutBuffer, "Potentiometer: %4d", potValue);
  myRobot->oled->print(oledOutBuffer);
  myRobot->oled->home();
  delay(1000);
}
```

### See also

* [tasks()](<#void-tasksvoid>)

## `void println(char *str)`

Prints up to 21 characters of the passed string on the current OLED line, including a CR/NL character, advancing the cursor to the next line. The remaining characters are truncated.

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
// Initialize the oled module, then print the Potentiometer value on a new line, demonstrating the auto-scroll functionality of the display.

#include <cetalib.h>
#include <stdio.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

char oledOutBuffer[64];
int potValue;

void setup() {
  Serial.begin(115200);
  delay(2000);
  myRobot->board->initialize();
  if (!myRobot->oled->initialize())
  {
    Serial.println("Failed to initialize OLED!. Stopping.");
    while (1);
  }
}

void loop() {
  potValue = myRobot->board->get_potentiometer();
  sprintf(oledOutBuffer, "Potentiometer: %4d", potValue);
  myRobot->oled->println(oledOutBuffer);
  delay(1000);
}
```

### See also

* [tasks()](<#void-tasksvoid>)