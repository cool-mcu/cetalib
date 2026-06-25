/*
  CETALIB "network" Library Example: "network_provision.ino"

  This application uses the "network" module APIs to provision a WiFi network for the robot.
  
  Make sure to select "Tools -> Flash Size" and select a 64kB Flash partition for 
  the filesystem.

  The robot attempts to connect using stored WiFi credentials, and if it fails, starts an 
  Access Point on 192.168.4.1 and launches a WiFi provisioning web server. The BUILTIN LED on
  the Robot will blink in a heartbeat pattern indicating the server is running.

  Next, using your smart device,
  - Scan for WiFi networks, and connect to the Custom SSID displayed in the serial monitor
  - Enter the passphrase (it's identical to the Custom SSID)
  - Open a browser and navigate to http://192.168.4.1
  - The web home page scans for local WiFi networks when loading
  - Select a network from the pull-down menu, enter the passphrase, them press "Connect Robot"

  NOTE: To re-scan local WiFi networks, simply refresh your browser at any time
  
  Hardware Configurations Supported:

  CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
  (Select "Board = Raspberry Pi Pico W")
  NOTE: LED_BUILTIN and Serial Monitor are used to display WiFi Provisioning status
 
  Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
  (Select "Board = SparkFun XRP Controller")
  NOTE: LED_BUILTIN and Serial Monitor are used to display WiFi Provisioning status

  Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
  (Select "Board = SparkFun XRP Controller (Beta)")
  NOTE: LED_BUILTIN is used to display WiFi Provisioning Status only during setup()
        Serial Monitor may be used at all times to display WiFi Provisioning status
  

  updated 07 Jun 2026
  by dBm Signal Dynamics Inc.

*/

#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

int ledPattern = 1;   // USER LED heartbeat pattern

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  while(!Serial);
  myRobot->board->initialize();
  myRobot->board->led_pattern(ledPattern);
  if (!myRobot->network->connect())
  {
    myRobot->network->provision();
  }
  Serial.printf("Local IP Address: %s\r\n", myRobot->network->get_IPAddr());
}

// the loop function runs over and over again forever
void loop() {
  myRobot->network->tasks(); // Monitor WiFi connection
  myRobot->board->tasks();   // Execute USER LED blink pattern                
}