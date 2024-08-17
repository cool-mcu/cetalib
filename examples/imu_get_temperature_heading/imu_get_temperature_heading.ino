/*
  CETALIB "imu" Library Example: "imu_get_temperature_heading.ino"

  This example performs heading correction calibration, then reads the
  temperature and yaw (heading) values from the LSM6DSOX sensor and continuously
  prints them to the Serial Monitor or Serial Plotter.

  Hardware Configuration:

  CETA IoT Robot (schematic #14-00069B) based on RPI-Pico-WH, with Adafruit 
  LSM6DSOX 6 DoF Accelerometer and Gyroscope board (#4438) connected to I2C1 pins
  (SDA/GP18)/(SCL/GP19)

  created 08 July 2024
  by dBm Signal Dynamics Inc.

*/

#include <stdio.h>
#include <cetalib.h>

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define an output print buffer
char outBuffer[128];

// define sensor sample interval variables
unsigned long imuSensorCurrentTime, imuSensorPrevTime;
const long imuSensorInterval = 1000; // (sample interval in mS)

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println();
  myRobot->board->initialize();
  if(0 == myRobot->board->get_button_level())
  {
    while(0 == myRobot->board->get_button_level());
    myRobot->imu->clear_calibration();
  }
  if (!myRobot->imu->initialize())
  {
    Serial.println("Failed to initialize IMU!. Stopping.");
    while (1);
  }
}

// the loop function runs over and over again forever
void loop() {
  myRobot->board->tasks();
  myRobot->imu->tasks();
  if(myRobot->board->is_button_pressed())
  {
    myRobot->imu->reset_heading();
  }
  imuSensorCurrentTime = millis();
  if ((imuSensorCurrentTime - imuSensorPrevTime) >= imuSensorInterval)
  {
    imuSensorPrevTime = imuSensorCurrentTime;
    sprintf(outBuffer, "Temperature: %3.1f\tHeading: %f\r\n", myRobot->imu->get_temperature(), myRobot->imu->get_heading());
    Serial.print(outBuffer); 
  }
}