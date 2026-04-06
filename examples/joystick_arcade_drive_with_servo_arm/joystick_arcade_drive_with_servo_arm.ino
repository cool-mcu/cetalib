/*
  CETALIB "joystick" Library Example: "joystick_arcade_drive_with_servo_arm.ino"

  Use the Joystick to implement an FRC "Split-Stick Arcade Drive" motor scheme with servo arm mamipulation

  "Left Stick Y" controls throttle on both motors
  "Right Stick X" controls steering only
  "Left Trigger" lifts the servo arm (increasing servo angle)
  "Right Trigger" lowers the servo arm (decreasing servo angle)

  Hardware Configuration:

  Windows/MacOS PC with Logitech F310 Gamepad connected in "D" mode.
  Follow the provided instructions for running the gamepad python script on your PC.

  CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
  (Select Board: "Raspberry Pi Pico W")
  
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select Board: "SparkFun XRP Controller")

  Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
  (Select "Board = SparkFun XRP Controller (Beta)")

  created 29 Mar 2026
  by dBm Signal Dynamics Inc.

*/

#include <cetalib.h>
#include <Servo.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define a pointer to the captured gamepad data
GAMEPAD* joystick_data;

// create a Servo object
Servo myServoArm;
// define & initialize a variable to hold servo angle (0 to 180 degrees);
int servo_angle = 90;
// define & initialize the servo pin based on which robot you are using
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
int servo_pin = 22;
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
int servo_pin = 6;
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
int servo_pin = 16;
#else
  #error Unsupported board selection
#endif

// define USER LED blink patterns for joystick initialization status
int ledPatternSuccess = 1;    // 1 blink per second on successful init
int ledPatternFailure = 5;    // 5 blinks per second on failed init

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(5000);
  myRobot->board->initialize();
  myServoArm.attach(servo_pin);
  myServoArm.write(servo_angle);
  myRobot->diffDrive->initialize(false, false); // adjust parameters for forward motion in your robot
  if(!myRobot->joystick->initialize())
  {
    Serial.println("Joystick Initialization failed. Stopping.");
    myRobot->board->led_pattern(ledPatternFailure);
    while(1)
    {
      // blink the USER LED to indicate joystick error
      myRobot->board->tasks();
    }
  }
  myRobot->board->led_pattern(ledPatternSuccess);
}

// the loop function runs over and over again forever
void loop() {
   // output an led blink pattern to indicate that the AP is ready for joystick connection
   myRobot->board->tasks();
   // run the engine that captures gamepad data
   myRobot->joystick->tasks();
   // if there's new gamepad data available, process it
   if(myRobot->joystick->is_active())
   {
      // update arcade drive values
      float leftDriveEffort = myRobot->joystick->get_arcade_throttle_effort();
      float rightDriveEffort = myRobot->joystick->get_arcade_turn_effort();
      myRobot->diffDrive->set_efforts(leftDriveEffort, rightDriveEffort);
      // point to the captured joystick data
      joystick_data = myRobot->joystick->get_data();
      // evaluate left trigger and right trigger switches to adjust the servo arm
      if(joystick_data->triggerButtons.isLeftTriggerPressed)
      {
        if (servo_angle >=180)
        {
          servo_angle = 180;
          myServoArm.write(servo_angle);
        }
        else
        {
          servo_angle++;
          myServoArm.write(servo_angle);
        }
      }

      if(joystick_data->triggerButtons.isRightTriggerPressed)
      {
        if (servo_angle <=0)
        {
          servo_angle = 0;
          myServoArm.write(servo_angle);
        }
        else
        {
          servo_angle--;
          myServoArm.write(servo_angle);
        }
      }

      Serial.printf("Left Stick Y Drive Throttle Control: %.2f\tRight Stick X Drive Steering Control: %.2f\tServo Angle: %d\r\n", leftDriveEffort, rightDriveEffort, servo_angle);
   }                    
}
