# diffDrive Module

The [CETA IoT Robot](https://www.cool-mcu.com/pages/robot-kit) is a mobile robot platform where driving from one place to another is central to the design of any program. The "differential Drive" module makes driving easy and has functions to:

* **Set the motor efforts**, which is the average voltage suplied to the motors.
* **Make point turns** for a desired number of degrees, either clockwise or countercloskwise

The current robot design does not provide wheel speed feedback, so the robot will settle on an arbitrary speed, given specific motor effort settings. 

<img src="../../assets/diffDrive-assembly.jpg?raw=true"><br>
  
Pico WH GPIO Pin Connections:
* GP4: LEFT MOTOR SERVO SIGNAL
* GP5: RIGHT MOTOR SERVO SIGNAL
* GP18: I2C SDA SIGNAL for ADAFRUIT LSM6DSOX IMU
* GP19: I2C SCL SIGNAL for ADAFRUIT LSM6DSOX IMU

For detailed lessons covering differential drive components, assembly, and functionality, [contact us](mailto:info@cool-mcu.com) to enrol in the [RPi Pico Robotics and IoT Curriculum for Pre-University Educators](https://www.cool-mcu.com/bundles/rpi-pico-robotics-and-iot-curriculum-for-pre-university-educators).

## Methods:
* [initialize()](<#void-initializebool-left_flip_dir-bool-right_flip_dir>)
* [set_efforts()](<#void-set_effortsfloat-lefteffort-float-righteffort>)
* [stop()](<#void-stopvoid>)
* [straight()](<#void-straightfloat-straighteffort>)
* [turn()](<#void-turnfloat-turndegrees-float-turneffort>)
* [clear_calibration()](<#void-clear_calibrationvoid>)
* [save_straight_compensation()](<#void-save_straight_compensationfloat-leftrightcomp>)

## `void initialize(bool left_flip_dir, bool right_flip_dir)`

Initialize pin settings and module state variables. Must be called once in setup() before use.

### Syntax

```c++
myRobot->diffDrive->initialize(false, false);
```
### Parameters

* **left_flip_dir**: boolean variable to set left motor spin direction (true, false)
* **right_flip_dir**: boolean variable to set right motor spin direction (true, false)

These parameters allow software compensation for reversed wiring on the motor terminals.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Initialize the diffDrive module, enabling the default forward direction for the motors.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  // initialize motors to spin in their default forward directions.
  myRobot->diffDrive->initialize(false, false);
}

void loop() {
  // Use the other diffDrive functions here
}
```

### See also

* [set_efforts()](<#void-set_effortsfloat-lefteffort-float-righteffort>)
* [stop()](<#void-stopvoid>)
* [straight()](<#void-straightfloat-straighteffort>)
* [turn()](<#void-turnfloat-turndegrees-float-turneffort>)
* [clear_calibration()](<#void-clear_calibrationvoid>)
* [save_straight_compensation()](<#void-save_straight_compensationfloat-leftrightcomp>)

## `void set_efforts(float leftEffort, float rightEffort)`

Sets the effort (applied voltage) to the motors. The robot will settle to a speed dictated by it's weight, or wheel friction. The range of values can be set from -1 for full effort in reverse, to 0 for no voltage or stopped, to +1 for full effort in the forward direction.

### Syntax

```c++
myRobot->diffDrive->set_effort(0.20, 0.20);
```
### Parameters

* **leftEffort**: float variable to set left motor effort (-1.00 to +1.00)
* **rightEffort**: float variable to set right motor effort (-1.00 to +1.00)

### Returns

* None.

### Notes

* Make sure the battery pack is connected to your robot.
* Place a small box underneath robot when testing these functions, to lift the robot wheels off the ground!

### Example

```c++
// Drive the robot forward until the USER SWITCH is pressed, then stop.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

float left_effort = 0.2f;
float right_effort = 0.2f;

void setup() {
  myRobot->board->initialize();
  myRobot->diffDrive->initialize(false, false);
  myRobot->diffDrive->set_efforts(left_effort, right_effort);
}

void loop() {
  myRobot->board->tasks();
  if (myRobot->board->is_button_pressed()) {
    myRobot->diffDrive->set_efforts(0.0f, 0.0f);
  }
}
```

### See also

* [initialize()](<#void-initializebool-left_flip_dir-bool-right_flip_dir>)
* [stop()](<#void-stopvoid>)
* [straight()](<#void-straightfloat-straighteffort>)
* [turn()](<#void-turnfloat-turndegrees-float-turneffort>)
* [clear_calibration()](<#void-clear_calibrationvoid>)
* [save_straight_compensation()](<#void-save_straight_compensationfloat-leftrightcomp>)

## `void stop(void)`

Stops the motors. Equivalent to "set_efforts(0.0f, 0.0f)".

### Syntax

```c++
myRobot->diffDrive->stop();
```
### Parameters

* None.

### Returns

* None.

### Notes

* None.

### Example

```c++
// Drive the robot forward until the USER SWITCH is pressed, then stop.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

float left_effort = 0.2f;
float right_effort = 0.2f;

void setup() {
  myRobot->board->initialize();
  myRobot->diffDrive->initialize(false, false);
  myRobot->diffDrive->set_efforts(left_effort, right_effort);
}

void loop() {
  myRobot->board->tasks();
  if (myRobot->board->is_button_pressed()) {
    myRobot->diffDrive->stop();
  }
}
```

### See also

* [initialize()](<#void-initializebool-left_flip_dir-bool-right_flip_dir>)
* [set_efforts()](<#void-set_effortsfloat-lefteffort-float-righteffort>)
* [straight()](<#void-straightfloat-straighteffort>)
* [turn()](<#void-turnfloat-turndegrees-float-turneffort>)
* [clear_calibration()](<#void-clear_calibrationvoid>)
* [save_straight_compensation()](<#void-save_straight_compensationfloat-leftrightcomp>)

## `void straight(float straightEffort)`

Applies same effort to both motors, applying motor effort compensation for straight motion.

### Syntax

```c++
myRobot->diffDrive->straight(0.2f);
```
### Parameters

* **straightEffort**: float variable used to set both motor efforts (-1.00 to +1.00)

### Returns

* None.

### Notes

* Due to manufacturing variations, the motors on the robot will not spin at the same speed given identical effort inputs. 
* A "Left/Right" effort compensation procedure must be performed to deliver "straight" motion when calling this function.

Run the [diffDrive_set_straight](../examples/diffDrive_set_straight/diffDrive_set_straight.ino) sketch to calibrate straight motion of the robot. 

### Example

```c++
// Drive the robot straight forward until the USER SWITCH is pressed, then stop.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

float straight_effort = 0.2f;

void setup() {
  myRobot->board->initialize();
  myRobot->diffDrive->initialize(false, false);
  myRobot->diffDrive->straight(straight_effort);
}

void loop() {
  myRobot->board->tasks();
  if (myRobot->board->is_button_pressed()) {
    myRobot->diffDrive->stop();
  }
}
```

### See also

* [initialize()](<#void-initializebool-left_flip_dir-bool-right_flip_dir>)
* [set_efforts()](<#void-set_effortsfloat-lefteffort-float-righteffort>)
* [stop()](<#void-stopvoid>)
* [turn()](<#void-turnfloat-turndegrees-float-turneffort>)
* [clear_calibration()](<#void-clear_calibrationvoid>)
* [save_straight_compensation()](<#void-save_straight_compensationfloat-leftrightcomp>)

## `void turn(float turnDegrees, float turnEffort)`

Point-turn the robot some relative heading given in **turnDegrees**, and exit the function when the heading has been reached. **turnEffort** is bounded from -1.00 (turn counterclockwise the relative heading at full effort) to  1.00 (turn clockwise the relative heading at full effort).

Uses the IMU to determine the heading of the robot.

### Syntax

```c++
myRobot->diffDrive->turn(90, 0.2f); // execute clockwise point-turn with effort of 0.2
```
### Parameters

* **turnDegrees**: float variable used to set the relative target heading in degrees (0 to 360)
* **turnEffort**: float variable used to set the point-turn effort (-1.00 to +1.00)

### Returns

* None.

### Notes

* IMU must be connected and calibrated.
* Function blocks while point-turn is executed.

Run the [imu_get_temperature_heading](../examples/imu_get_temperature_heading/imu_get_temperature_heading.ino) sketch to calibrate the IMU. 

### Example

```c++
// Run a square path when the USER SWITCH is pressed.
// Assumes IMU is already calibrated.
// Motor effort settings are set to produce "straight" motion by experimentation.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

float left_motor_effort = 0.2f;
float right_motor_effort = 0.23f;
float turn_effort = 0.15f;
float turn_degrees = 90.0f;

void setup() {
  myRobot->board->initialize();
  myRobot->diffDrive->initialize(false, false);
  if (!myRobot->imu->initialize())
  {
    myRobot->board->led_pattern(5); // flash the USER LED if IMU not available
    while (1) {
      myRobot->board->tasks();
    }
  }
  myRobot->board->led_pattern(1); // indicate readiness
}

void loop() {
  myRobot->board->tasks();
  myRobot->imu->tasks();
  if (myRobot->board->is_button_pressed()) {
    myRobot->diffDrive->set_efforts(left_motor_effort, right_motor_effort);
    delay(2000);
    myRobot->diffDrive->turn(turn_degrees, turn_effort);
    myRobot->diffDrive->set_efforts(left_motor_effort, right_motor_effort);
    delay(2000);
    myRobot->diffDrive->turn(turn_degrees, turn_effort);
    myRobot->diffDrive->set_efforts(left_motor_effort, right_motor_effort);
    delay(2000);
    myRobot->diffDrive->turn(turn_degrees, turn_effort);
    myRobot->diffDrive->set_efforts(left_motor_effort, right_motor_effort);
    delay(2000);
    myRobot->diffDrive->stop();
  }
}
```

### See also

* [initialize()](<#void-initializebool-left_flip_dir-bool-right_flip_dir>)
* [set_efforts()](<#void-set_effortsfloat-lefteffort-float-righteffort>)
* [stop()](<#void-stopvoid>)
* [straight()](<#void-straightfloat-straighteffort>)
* [clear_calibration()](<#void-clear_calibrationvoid>)
* [save_straight_compensation()](<#void-save_straight_compensationfloat-leftrightcomp>)

## `void clear_calibration(void)`

Deletes all EEPROM calibration data for the diffDrive module.

### Syntax

```c++
myRobot->diffDrive->clear_calibration();
```
### Parameters

* None.

### Returns

* None.

### Notes

* Current EEPROM-stored values
  * "Straight" motion compensation value

### Example

```c++
// Delete/Clear the diffDrive EEPROM memory if the USER SWITCH is pressed on reset.

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
  myRobot->diffDrive->initialize();
  if(0 == myRobot->board->get_button_level())
  {
    while(0 == myRobot->board->get_button_level());
    myRobot->diffDrive->clear_calibration();
  }
}

void loop() {
  // run program re-calibrate and save "straight" motion compensation value to EEPROM
}
```

### See also

* [initialize()](<#void-initializebool-left_flip_dir-bool-right_flip_dir>)
* [set_efforts()](<#void-set_effortsfloat-lefteffort-float-righteffort>)
* [stop()](<#void-stopvoid>)
* [straight()](<#void-straightfloat-straighteffort>)
* [turn()](<#void-turnfloat-turndegrees-float-turneffort>)
* [save_straight_compensation()](<#void-save_straight_compensationfloat-leftrightcomp>)

## `void save_straight_compensation(float leftRightComp)`

Saves a "straight" motion compensation value to EEPROM calibration memory.

### Syntax

```c++
myRobot->diffDrive->save_straight_compensation(1.04f);
```
### Parameters

* **leftRightComp**: float variable used to compensate for Left/Right motor speed mismatch with identical effort input. Used when calling the diffDrive_straight() function. 

### Returns

* None.

### Notes

Run the [diffDrive_set_straight](../examples/diffDrive_set_straight/diffDrive_set_straight.ino) sketch to see example use of these functions to calibrate the straight motion of the robot. 

### Example

```c++
// Run/Save straight motion compensation value to EEPROM

#include <cetalib.h>

const struct CETALIB_INTERFACE *myRobot = &CETALIB;

void setup() {
  myRobot->board->initialize();
  myRobot->diffDrive->initialize();
  if(0 == myRobot->board->get_button_level())
  {
    while(0 == myRobot->board->get_button_level());
    myRobot->diffDrive->clear_calibration();
  }
}

void loop() {
  
  float left_right_compensation
  // run program re-calibrate and save "straight" motion compensation value to EEPROM
  // ...

  // save compensation value
  myRobot->diffDrive->save_straight_compensation(left_right_compensation);
}
```

### See also

* [initialize()](<#void-initializebool-left_flip_dir-bool-right_flip_dir>)
* [set_efforts()](<#void-set_effortsfloat-lefteffort-float-righteffort>)
* [stop()](<#void-stopvoid>)
* [straight()](<#void-straightfloat-straighteffort>)
* [turn()](<#void-turnfloat-turndegrees-float-turneffort>)
* [clear_calibration()](<#void-clear_calibrationvoid>)


