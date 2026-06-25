/*
  CETALIB "joystick" Library Example: "joystick_arcade_drive.ino"

  Use the Joystick to implement an FRC "Split-Stick Arcade Drive" motor scheme
  "Left Stick Y" controls throttle on both motors
  "Right Stick X" controls steering only

  This application requires the "network" module to provision/connect to a WiFi network.
  Make sure to select "Tools -> Flash Size" and select a 64kB Flash partition for 
  the filesystem.
  
  Please review the documentation for the network module to understand how WiFi
  provisioning works (https://github.com/cool-mcu/cetalib).

  Hardware Configuration:

  Windows/MacOS PC with Logitech F310 Gamepad connected in "D" mode.
  Follow the provided instructions for running the gamepad python script on your PC.

  CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
  (Select Board: "Raspberry Pi Pico W")
  
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select Board: "SparkFun XRP Controller")

  Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
  (Select "Board = SparkFun XRP Controller (Beta)")

  updated 16 June 2026
  by dBm Signal Dynamics Inc.

*/

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define a pointer to the captured gamepad data
GAMEPAD* joystick_data;

// define motor effort parameters
float leftDriveEffort, rightDriveEffort;

// define USER LED blink patterns for joystick initialization status
int ledPatternSuccess = 1;    // 1 blink per second on successful init
int ledPatternFailure = 5;    // 5 blinks per second on failed init

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(5000);
  myRobot->board->initialize();
  myRobot->diffDrive->initialize(true, true); // adjust parameters for forward motion in your robot
  // Attempt to connect to WiFi AP using existing credentials, or provision a new WiFi Connection
  if (!myRobot->network->connect())
  {
    Serial.println("Failed to connect to AP. WiFi provisioning started...");
    myRobot->network->provision();
  }
  Serial.println("\nConnected to the Network...");
  // Attempt to initialize joystick UDP Server
  if(!myRobot->joystick->initialize())
  {
    Serial.println();
    Serial.println("Failed to initialize Joystick UDP Server!");
    Serial.println("- Press/Hold BUTTON for 3 seconds to switch WiFi Network, or");
    Serial.println("- Update sketch: Call 'network->connect()' and 'network->provision()' before calling 'joystick->initialize()'");
    myRobot->board->led_pattern(ledPatternFailure);
    while(1)
    {
      // blink the USER LED to indicate joystick error
      myRobot->board->tasks();
      // sample USER SWITCH to reset WiFi credentials and reboot
      if(myRobot->board->is_button_pressed())
      {
        myRobot->network->reset_connection();
      }
    }
  }
  myRobot->board->led_pattern(ledPatternSuccess);
  Serial.println("\nJoystick UDP Server is up!...\n");
}

// the loop function runs over and over again forever
void loop() {
   // output an led blink pattern to indicate that the AP is ready for joystick connection
   myRobot->board->tasks();
   // manage the WiFi connection
   myRobot->network->tasks();
   // run the engine that captures gamepad data
   myRobot->joystick->tasks();
   // if there's new gamepad data available, process it
   if(myRobot->joystick->is_active())
   {
      leftDriveEffort = myRobot->joystick->get_arcade_throttle_effort();
      rightDriveEffort = myRobot->joystick->get_arcade_turn_effort();
      myRobot->diffDrive->set_efforts(leftDriveEffort, rightDriveEffort);
      Serial.printf("Left Stick Y Drive Throttle Control: %.2f\tRight Stick X Drive Steering Control: %.2f\r\n", leftDriveEffort, rightDriveEffort);
   }                    
}
