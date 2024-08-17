/*
  CETALIB "mqttc" Library Example: "mqttc_pub_sub_imu.ino"

  This example performs heading correction calibration, then reads the
  temperature and yaw (heading) values from the LSM6DSOX sensor and continuously
  publishes them to a topic on your MQTT broker instance.
  
  This example also subscribes to receive control messages to remotely control
  the imu. The message "{"imuControl": 1}"" will trigger execution of the imu
  "reset_heading()" function.

  Pressing the user pushbutton connects/disconnects from the broker and WiFi AP to 
  keep data usage low.

  Note:
  For external brokers, you can sign up to the "free" tier for the HiveMQ cloud service or Adafruit IO
  https://www.hivemq.com/products/mqtt-cloud-broker/ 
  https://io.adafruit.com
  
  Once you sign in, you will need to create/obtain MQTT client login credentials for the
  robot, and copy/paste these credentials, along with your broker URL, and port number 
  into the sketch below. MQTT topic IDs will need to be updated for an Adafruit IO connection. 

  You can also configure a local broker, running on your PC, which is connected to
  the same network as your robot (for example, the "Aedes" broker, running in Node-Red).
  Make sure to load the provided "flows.json" file into the Node-Red instance.

  The "mqttc" "connect" function connects to your WiFi access point, then opens
  a TCP/TLS connection to your chosen broker and port. For secure connections over
  port 8883, the library already contains server root CA certificates for Adafruit IO
  and HiveMQ brokers.

  Hardware Configuration:

  CETA IoT Robot (schematic #14-00069B) based on RPI-Pico-WH, with Adafruit 
  LSM6DSOX 6 DoF Accelerometer and Gyroscope board (#4438) connected to I2C1 pins
  (SDA/GP18)/(SCL/GP19). WiFi-based internet access available.

  created 30 July 2024
  by dBm Signal Dynamics Inc.

*/

#include <stdio.h>
#include <string.h>
#include <cetalib.h>
#include <ArduinoJson.h>        

// WiFi Parameters
const char ssid[] = "";               
const char pass[] = "";              

// MQTT Broker URL, Username, Password
const char MQTTbroker[] = "";
int MQTTport = 8883;    // EDIT: 1883 for insecure connection, or 8883 for secure connection
const char MQTTusername[] = "";
const char MQTTpassword[] = "";

// MQTT publish topics
const char imuDataTopic[] = "CETAIoTRobot/out/imu";

// MQTT subscribe topics (maximum of 10 or define "" for none)
const char imuControlTopic[] = "CETAIoTRobot/in/imu";
const char *subscribeTopicIDs[] = {imuControlTopic};
//const char *subscribeTopicIDs[] = {""};
int num_subscribeTopicIDs = sizeof(subscribeTopicIDs)/sizeof(subscribeTopicIDs[0]);

// MQTT data serialization and deserialization functions & remote control variables
char *imuData_Serialize(void);      // serialize imu temerature and heading data
int imuControl_Deserialize(void);   // deserialize "imu_Control" value
int imuControl;                     // stores value from received control payload 

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define an output buffer for use by the sprintf() function
char outBuffer[256];

// define sensor sample interval variables
unsigned long imuSensorCurrentTime, imuSensorPrevTime;
const long imuSensorInterval = 3000; // (sample interval in mS)

// define robot connection states
enum CONNECTION_STATES {CONNECTED, DISCONNECTED} connState;

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
    myRobot->board->led_blink(10);
    while (1)
    {
      myRobot->board->tasks();
    }
  }
  connState = DISCONNECTED;
  myRobot->board->led_pattern(1);
  Serial.println("Robot is disconnected from the network. Press pushbutton to conneect.\r\n");
}

// the loop function runs over and over again forever
void loop() {

  switch(connState)
  {
    case DISCONNECTED:
      myRobot->board->tasks();
      if(myRobot->board->is_button_pressed())
      {
        // Attempt to connect to AP and Broker
        if(!myRobot->mqttc->connect(ssid, pass, MQTTbroker, MQTTport, MQTTusername, MQTTpassword, subscribeTopicIDs, num_subscribeTopicIDs))
        {
          Serial.println("Failed to initialize MQTT Client!. Stopping.");
          myRobot->board->led_blink(10);
          while (1)
          {
            myRobot->board->tasks();
          }
        }
        myRobot->board->led_pattern(2);
        connState = CONNECTED;
      }
      break;
    case CONNECTED:
      myRobot->board->tasks();
      myRobot->imu->tasks();
      myRobot->mqttc->tasks();
      if(myRobot->board->is_button_pressed())
      {
        Serial.println("Disconnecting from AP and Broker");
        myRobot->mqttc->disconnect();
        connState = DISCONNECTED;
        myRobot->board->led_pattern(1);
      }
      else
      {
        // publish imu data periodically
        imuSensorCurrentTime = millis();
        if ((imuSensorCurrentTime - imuSensorPrevTime) >= imuSensorInterval)
        {
          imuSensorPrevTime = imuSensorCurrentTime;
          myRobot->mqttc->send_message(imuDataTopic, imuData_Serialize());
        }
        // process imu control messages
        if(myRobot->mqttc->is_message_available(imuControlTopic))
        {
          if(imuControl_Deserialize())
          {
            switch(imuControl)
            {
              case 1:
                Serial.println("imuControl: Resetting imu heading value");
                myRobot->imu->reset_heading();
                break;
              default:
                break;
            }
          }
        }
      } 
      break;
    default:
      break;
  }  
  
}

/*** mqttc serialization functions ********************************************/

char *imuData_Serialize(void)
{
  sprintf(outBuffer, "{\"imuData\": {\"temperature\": %3.1f, \"heading\": %4.1f}}", myRobot->imu->get_temperature(), myRobot->imu->get_heading());
  return outBuffer;
}

/*** mqttc de-serialization functions *****************************************/

int imuControl_Deserialize(void)
{
  char input[256];
  strcpy(input, myRobot->mqttc->receive_message());
  StaticJsonDocument<16> doc;
  DeserializationError error = deserializeJson(doc, input, sizeof(input));
  if(error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return 0;
  }
  else
  {
    if(doc.containsKey("imuControl"))
    {
      imuControl = doc["imuControl"];         // Extract/save "imuControl" value
      return 1;
    }
    else
    {
      Serial.println("Parsing failed! Invalid Key\r\n"); // No valid keys found in JSON Message
      imuControl = 0;
      return 0;
    }
  }
}