/*
 * Copyright (C) 2026 dBm Signal Dynamics Inc.
 *
 * File:            joystick.cpp
 * Project:         
 * Date:            Mar 28, 2026
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "joystick" driver
 * 
 * Starts a local WiFi network and UDP server to process Logitech F310 Switch
 * Data in "D" mode. Requires python UDP client "cetalib-joystick-client.py"
 * 
 * Left and Right Joystick data processing functions are provided to enable
 * "Tank" or "Split-Arcade" Motor Drive Schemes using diffDrive->set_efforts()
 * 
 * See "examples" for example use sketches 
 *
 * Hardware Configurations Supported:
 * 
 * CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
 * (Select Board: "Raspberry Pi Pico W")
 * BUILTIN LED used to indicate AP/Server Status
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select Board: "SparkFun XRP Controller")
 * BUILTIN LED used to indicate AP/Server Status
 *
 * Sparkfun XRP (Beta) Robot Platform (#KIT-22230), based on the RPI Pico W
 * (Select "Board = SparkFun XRP Controller (Beta)")
 * NO dedicated LED is available to indicate AP/Server Status.
 * User must use USER LED in their main sketch.
 *
 */

 /** Include Files *************************************************************/
#include <Arduino.h>                // Required for Arduino functions
#include <WiFi.h>                   // Required for WiFi functions
#include <WiFiUdp.h>                // Required for UDP server interface
#include <stdio.h>                  // Required for sprintf()
#include <string>                   // Required for strcpy(); function
#include "joystick.h"

/*** Symbolic Constants used in this module ***********************************/
#define SERIAL_PORT Serial  // Default to Serial
#if defined(NO_USB)
    #undef SERIAL_PORT
    #define SERIAL_PORT Serial1     // Use Serial1 if USB is disabled
#endif

/*** Global Variable Declarations *********************************************/

// Access Point Settings
char apSSID[64];
char apPassphrase[64];
IPAddress apIP(192, 168, 42, 1);                // WiFi network AP IP address
IPAddress gateway(192, 168, 42, 1);             // WiFi network Gateway IP Address
IPAddress subnet(255, 255, 255, 0);             // WiFi network Subnet Mask
int wifiChannel;                                // WiFi network radio channel to use (1, 6 or 11)
byte macAddr[6];                                // WiFi radio IEEE MAC address used to generate unique SSID and passphrase
                                                // SSID: "XRPBeta_abcd", where "abcd" are the last 2 mac address hex bytes
                                                // Pass: "XRPBeta_abcd"

// UDP Server settings
unsigned int localPort = 8888;                  // local port to listen on
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];  // buffer to hold incoming packet
WiFiUDP Udp;
static int joystickIsActive = 0;                // used to detect a joystick update 

// local gamepad data structure
GAMEPAD joystick_local;

// define the function interface
extern const struct JOYSTICK_INTERFACE JOYSTICK = {
    .initialize                 = &joystick_init,
    .tasks                      = &joystick_tasks,
    .is_active                  = &joystick_is_active,
    .get_data                   = &joystick_get_data,
    .get_left_tank_effort       = &joystick_get_left_tank_effort,
    .get_right_tank_effort      = &joystick_get_right_tank_effort,
    .get_arcade_throttle_effort = &joystick_get_left_arcade_effort,
    .get_arcade_turn_effort     = &joystick_get_right_arcade_effort,
};

/*** Private Function Prototypes **********************************************/

// Perform a site survey during the initialize() function. Look specifically
// for the RSSI (signal strength) of other APs on channels 1, 6, and 11,
// then pick the "quietest" one.
static int selectBestClassroomChannel(void);

/*** Public Function Definitions **********************************************/

bool joystick_init(void)
{
  // check if WiFi already in use by "mqttc" module
  if(WiFi.status() == WL_CONNECTED)
  {
      SERIAL_PORT.println("WiFi already in use. Cannot start AP");
      return false;
  }

  // initialize the Joystick status LED
  #if defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
  // Initialize JOYSTICK SERVICE STATUS LED
  pinMode(JOYSTICK_STAT_LED_PIN, OUTPUT);     // set digital pin as output
  digitalWrite(JOYSTICK_STAT_LED_PIN, 0);     // initialize LED state
  #endif

  // initialize local joystick data structure
  strcpy(joystick_local.clientIP, "0.0.0.0");
  joystick_local.clientPort = 0;
  for (int i=0; i<8; i++)
  {
    joystick_local.gamepadRaw[i] = 0;
  }
  joystick_local.leftStickX = 0;
  joystick_local.leftStickY = 0;
  joystick_local.rightStickX = 0;
  joystick_local.rightStickY = 0;
  joystick_local.dPad.isNeutral = true;
  joystick_local.dPad.isNorthPressed = false;
  joystick_local.dPad.isEastPressed = false;
  joystick_local.dPad.isSouthPressed = false;
  joystick_local.dPad.isWestPressed = false;
  joystick_local.actionButtons.isAPressed = false;
  joystick_local.actionButtons.isBPressed = false;
  joystick_local.actionButtons.isXPressed = false;
  joystick_local.actionButtons.isYPressed = false;
  joystick_local.triggerButtons.isLeftTriggerPressed = false;
  joystick_local.triggerButtons.isRightTriggerPressed = false;
  joystick_local.bumperButtons.isLeftBumperPressed = false;
  joystick_local.bumperButtons.isRightBumperPressed = false;
  joystick_local.isBackPressed = false;
  joystick_local.isStartPressed = false;

  // Tell the WiFi stack we want AP mode
  WiFi.mode(WIFI_AP);
  
  // Configure AP mode
  WiFi.softAPConfig(apIP, gateway, subnet);

  // Select the "quietest non-overlapping" WiFi channel to start the network (1, 6, or 11)
  wifiChannel = selectBestClassroomChannel();
  
  // Create a unique AP SSID, and AP Passphrase using the last 2 macAddr bytes
  // "XRPBeta_abcd", "XRP_abcd", "CETAIoT_abcd"
  // Note: the WiFi passphrase is set to match the SSID
  WiFi.macAddress(macAddr);     // read/save the mac address of the radio
  #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  sprintf(apSSID, "CETAIoT_%02x%02x", macAddr[1], macAddr[0]);
  sprintf(apPassphrase, "CETAIoT_%02x%02x", macAddr[1], macAddr[0]);
  #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
  sprintf(apSSID, "XRP_%02x%02x", macAddr[1], macAddr[0]);
  sprintf(apPassphrase, "XRP_%02x%02x", macAddr[1], macAddr[0]);
  #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER_BETA)
  sprintf(apSSID, "XRPBeta_%02x%02x", macAddr[1], macAddr[0]);
  sprintf(apPassphrase, "XRPBeta_%02x%02x", macAddr[1], macAddr[0]);
  #else
   #error Unsupported board selection
  #endif
  
  // --- Now start the Access Point ---
  SERIAL_PORT.printf("Starting AP \"%s\"\n", apSSID);
  if (WiFi.beginAP(apSSID, apPassphrase, wifiChannel) == WL_CONNECTED)
  {
    SERIAL_PORT.print("AP started! IP: ");
    SERIAL_PORT.print(WiFi.softAPIP());
    SERIAL_PORT.print(" WiFi Channel: ");
    SERIAL_PORT.println(wifiChannel);
  } 
  else 
  {
    SERIAL_PORT.println("AP failed to start!.");
    return false;
  }
  
  // Start the UDP Server
  SERIAL_PORT.printf("UDP server started on port %d\n", localPort);
  Udp.begin(localPort);
  
  // update Joystick status LED
  #if defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
  // if you get here, you are connected and ready to go!
  digitalWrite(JOYSTICK_STAT_LED_PIN, 1);
  #endif

  return true;

}

void joystick_tasks(void)
{
  // if there's joystick data available, read/process a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // signal that we've received a joystick update
    joystickIsActive = 1;

    // save the remote client IP and port
    sprintf(joystick_local.clientIP, "%s", Udp.remoteIP().toString().c_str());
    joystick_local.clientPort = Udp.remotePort();
    
    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    
    // update the local joystick data structure

    // gamepad raw bytes
    joystick_local.gamepadRaw[0] = packetBuffer[0];
    joystick_local.gamepadRaw[1] = packetBuffer[1];
    joystick_local.gamepadRaw[2] = packetBuffer[2];
    joystick_local.gamepadRaw[3] = packetBuffer[3];
    joystick_local.gamepadRaw[4] = packetBuffer[4];
    joystick_local.gamepadRaw[5] = packetBuffer[5];
    joystick_local.gamepadRaw[6] = packetBuffer[6];
    joystick_local.gamepadRaw[7] = packetBuffer[7];
    
    // update joysticks
    joystick_local.leftStickX = packetBuffer[0];
    joystick_local.leftStickY = packetBuffer[1];
    joystick_local.rightStickX = packetBuffer[2];
    joystick_local.rightStickY = packetBuffer[3];
    
    // update dPad buttons
    if(packetBuffer[4] == 8){
      joystick_local.dPad.isNeutral = true;
    }
    else{
      joystick_local.dPad.isNeutral = false;
    }
    
    if(packetBuffer[4] == 0){
      joystick_local.dPad.isNorthPressed = true;
    }
    else{
      joystick_local.dPad.isNorthPressed = false;
    }
    
    if(packetBuffer[4] == 2){
      joystick_local.dPad.isEastPressed = true;
    }
    else{
      joystick_local.dPad.isEastPressed = false;
    }

    if(packetBuffer[4] == 4){
      joystick_local.dPad.isSouthPressed = true;
    }
    else{
      joystick_local.dPad.isSouthPressed = false;
    }

    if(packetBuffer[4] == 6){
      joystick_local.dPad.isWestPressed = true;
    }
    else{
      joystick_local.dPad.isWestPressed = false;
    }

    // update actionbuttons

    if(packetBuffer[4] & 32){
      joystick_local.actionButtons.isAPressed = true;
    }
    else{
      joystick_local.actionButtons.isAPressed = false;
    }

    if(packetBuffer[4] & 64){
      joystick_local.actionButtons.isBPressed = true;
    }
    else{
      joystick_local.actionButtons.isBPressed = false;
    }

    if(packetBuffer[4] & 16){
      joystick_local.actionButtons.isXPressed = true;
    }
    else{
      joystick_local.actionButtons.isXPressed = false;
    }

    if(packetBuffer[4] & 128){
      joystick_local.actionButtons.isYPressed = true;
    }
    else{
      joystick_local.actionButtons.isYPressed = false;
    }

    // update trigger buttons

    if(packetBuffer[5] & 4){
      joystick_local.triggerButtons.isLeftTriggerPressed = true;
    }
    else{
      joystick_local.triggerButtons.isLeftTriggerPressed = false;
    }

     if(packetBuffer[5] & 8){
      joystick_local.triggerButtons.isRightTriggerPressed = true;
    }
    else{
      joystick_local.triggerButtons.isRightTriggerPressed = false;
    }

    // update bumper buttons

    if(packetBuffer[5] & 1){
      joystick_local.bumperButtons.isLeftBumperPressed = true;
    }
    else{
      joystick_local.bumperButtons.isLeftBumperPressed = false;
    }

    if(packetBuffer[5] & 2){
      joystick_local.bumperButtons.isRightBumperPressed = true;
    }
    else{
      joystick_local.bumperButtons.isRightBumperPressed = false;
    }

    // update back and start buttons

    if(packetBuffer[5] & 16){
      joystick_local.isBackPressed = true;
    }
    else{
      joystick_local.isBackPressed = false;
    }

    if(packetBuffer[5] & 32){
      joystick_local.isStartPressed = true;
    }
    else{
      joystick_local.isStartPressed = false;
    }
  }
}

int joystick_is_active(void)
{
  if (joystickIsActive)
  {
    joystickIsActive = 0;
    return 1;
  }
  else
  {
    return 0;
  }
}

GAMEPAD* joystick_get_data(void)
{
  return &joystick_local;
}

float joystick_get_left_tank_effort(void)
{
  // Left Stick Y (gamepadRaw[1]) controls effort on Left Motor
  float leftDrive = (128.0 - (float)joystick_local.gamepadRaw[1]) / 128.0;
  
  // Add a small "deadzone to prevent "drifting"
  if (abs(leftDrive) < 0.05) leftDrive = 0.0;
  
  return leftDrive;
}

float joystick_get_right_tank_effort(void)
{
  // Right Stick Y (gamepadRaw[3]) controls effort on Right Motor
  float rightDrive = (128.0 - (float)joystick_local.gamepadRaw[3]) / 128.0;
  
  // Add a small "deadzone to prevent "drifting"
  if (abs(rightDrive) < 0.05) rightDrive = 0.0;
  
  return rightDrive;
}

float joystick_get_left_arcade_effort(void)
{
  // Use the Joystick to implement a FRC "Split-Stick Arcade Drive" motor scheme
  // Left Stick Y (gamepadRaw[1]) controls effort/throttle on both motors
  // Right Stick X (gamepadRaw[2]) controls steering only
    
  // 1. Convert Raw Bytes to Normalized Floats (-1.0 to 1.0)
  // We subtract from 128 so that 'Up' and 'Right' are positive numbers
  float throttle = (128.0 - (float)joystick_local.gamepadRaw[1]) / 128.0; 
  float turn     = ((float)joystick_local.gamepadRaw[2] - 128.0) / 128.0;

  // 2. Apply a Deadzone (Prevents the robot from humming at rest)
  if (abs(throttle) < 0.05) throttle = 0;
  if (abs(turn) < 0.05)     turn = 0;

  // 3. Arcade Drive Mixing Math
  float leftPower  = throttle + turn;
  float rightPower = throttle - turn;

  // 4. Constrain values to -1.0 to 1.0 (prevents over-driving)
  leftPower  = constrain(leftPower, -1.0, 1.0);
  rightPower = constrain(rightPower, -1.0, 1.0);

  return leftPower;
}

float joystick_get_right_arcade_effort(void)
{
  // Use the Joystick to implement a FRC "Split-Stick Arcade Drive" motor scheme
  // Left Stick Y (gamepadRaw[1]) controls effort/throttle on both motors
  // Right Stick X (gamepadRaw[2]) controls steering only
    
  // 1. Convert Raw Bytes to Normalized Floats (-1.0 to 1.0)
  // We subtract from 128 so that 'Up' and 'Right' are positive numbers
  float throttle = (128.0 - (float)joystick_local.gamepadRaw[1]) / 128.0; 
  float turn     = ((float)joystick_local.gamepadRaw[2] - 128.0) / 128.0;

  // 2. Apply a Deadzone (Prevents the robot from humming at rest)
  if (abs(throttle) < 0.05) throttle = 0;
  if (abs(turn) < 0.05)     turn = 0;

  // 3. Arcade Drive Mixing Math
  float leftPower  = throttle + turn;
  float rightPower = throttle - turn;

  // 4. Constrain values to -1.0 to 1.0 (prevents over-driving)
  leftPower  = constrain(leftPower, -1.0, 1.0);
  rightPower = constrain(rightPower, -1.0, 1.0);

  return rightPower;
}

/*** Private Function Definitions *********************************************/

int selectBestClassroomChannel() {
  int scanResult = WiFi.scanNetworks();
  long rssi1 = -100, rssi6 = -100, rssi11 = -100;

  for (int i = 0; i < scanResult; i++) {
    int ch = WiFi.channel(i);
    long rssi = WiFi.RSSI(i);
    if (ch == 1 && rssi > rssi1) rssi1 = rssi;
    if (ch == 6 && rssi > rssi6) rssi6 = rssi;
    if (ch == 11 && rssi > rssi11) rssi11 = rssi;
  }

  // Pick the channel with the weakest "loudest neighbor"
  if (rssi1 <= rssi6 && rssi1 <= rssi11) return 1;
  if (rssi6 <= rssi1 && rssi6 <= rssi11) return 6;
  return 11;
}






