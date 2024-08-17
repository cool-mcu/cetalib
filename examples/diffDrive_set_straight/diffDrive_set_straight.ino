/*
  CETALIB "mqttc" Library Example: "diffDrive_set_straight.ino"

  This example communicates with Node-Red dashboard widgets via MQTT broker to
  configure/calibrate straight robot motion.

  Messages:
  
  - {"leftMotorEffort": 0.4} sets the desired leftMotor effort
  - {"rightMotorEffort": 0.4} sets the desired rightMotor effort
  - {"straightEffort": 0.4} sets the desired straightEffort
  - {"event": 1} executes the "diffDrive_set_efforts()" function
  - {"event": 2} executes the "diffDrive_stop()" function
  - {"event": 3} executes the "diffDrive_straight()" function using the saved "Left_Right_Compensation" factor for straight robot motion.
  - {"event": 4} executes the "diffDrive_save_compensation()" function

  You will need to install Node-Red on your PC, and your PC needs to be on the 
  same network as the robot. Make sure to load the provided "flows.json" file into the
  Node-Red instance.

  Hardware Configuration:

  CETA IoT Robot (schematic #14-00069A/B) based on RPI-Pico-WH

  created 01 Aug 2024
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
const char MQTTbroker[] = "";     // EDIT: insert IP address of Node-Red PC
int MQTTport = 1883;              
const char MQTTusername[] = "";   // N/A for local Node-Red instance
const char MQTTpassword[] = "";   // N/A for local Node-Red instance

// MQTT publish topics
const char diffDriveDataTopic[] = "CETAIoTRobot/out/diffDrive";

// MQTT subscribe topics (maximum of 10 or define "" for none)
const char leftMotorEffortControlTopic[] = "CETAIoTRobot/in/diffDrive/leftMotorEffort";
const char rightMotorEffortControlTopic[] = "CETAIoTRobot/in/diffDrive/rightMotorEffort";
const char straightMotorEffortControlTopic[] = "CETAIoTRobot/in/diffDrive/straightMotorEffort";
const char eventControlTopic[] = "CETAIoTRobot/in/diffDriveEvent";
const char *subscribeTopicIDs[] = {leftMotorEffortControlTopic, rightMotorEffortControlTopic, straightMotorEffortControlTopic, eventControlTopic};
//const char *subscribeTopicIDs[] = {""};
int num_subscribeTopicIDs = sizeof(subscribeTopicIDs)/sizeof(subscribeTopicIDs[0]);

// MQTT data serialization and deserialization functions & remote control variables
char *diffDriveData_Serialize(void);        // serialize leftMotorEffort, rightMotorEffort, leftRightCompensation data
int leftMotorEffort_Deserialize(void);      // deserialize "leftMotorEffort" value
int rightMotorEffort_Deserialize(void);     // deserialize "rightMotorEffort" value
int straightMotorEffort_Deserialize(void);  // deserialize "straightMotorEffort" value
int event_Deserialize(void);                // deserialize "event" value
float leftMotorEffort = 0.2f;               // stores value from received control payload
float rightMotorEffort = 0.2f;              // stores value from received control payload
float straightMotorEffort = 0.2f;           // stores value from received control payload
int event = 0;                              // stores value from received control payload       

// diffDrive straight motion compensation value
float leftRightCompensation = 1.0f;         // calculated as leftMotorEffort/rightMotorEffort
                                            // if rightMotorEffort = 0, then calculation not updated

// define & initialize a pointer to the CETALIB functions
const struct CETALIB_INTERFACE *myRobot = &CETALIB;

// define an output buffer for use by the sprintf() function
char outBuffer[256];

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
    myRobot->diffDrive->clear_calibration();
  }
  myRobot->diffDrive->initialize(false, false);
  leftRightCompensation = leftMotorEffort/rightMotorEffort;
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
        // process control messages
        if(myRobot->mqttc->is_message_available(leftMotorEffortControlTopic))
        {
          if(leftMotorEffort_Deserialize())
          {
            if(rightMotorEffort != 0.0f)
            {
              leftRightCompensation = leftMotorEffort/rightMotorEffort;
              myRobot->mqttc->send_message(diffDriveDataTopic, diffDriveData_Serialize());
              Serial.println("Received \"leftMotorEffort\" update");
            }
          }
        }
        else if(myRobot->mqttc->is_message_available(rightMotorEffortControlTopic))
        {
          if(rightMotorEffort_Deserialize())
          {
            if(rightMotorEffort != 0.0f)
            {
              leftRightCompensation = leftMotorEffort/rightMotorEffort;
              myRobot->mqttc->send_message(diffDriveDataTopic, diffDriveData_Serialize());
              Serial.println("Received \"rightMotorEffort\" update");
            }
          }
        }
        else if(myRobot->mqttc->is_message_available(straightMotorEffortControlTopic))
        {
          if(straightMotorEffort_Deserialize())
          {
            myRobot->mqttc->send_message(diffDriveDataTopic, diffDriveData_Serialize());
            Serial.println("Received \"straightMotorEffort\" update");
          }
        }
        else if(myRobot->mqttc->is_message_available(eventControlTopic))
        {
          if(event_Deserialize())
          {
            switch(event)
            {
              case 0:
                break;
              case 1:
                myRobot->diffDrive->set_efforts(leftMotorEffort, rightMotorEffort);
                Serial.println("Executed \"diffDrive->set_efforts()\" command");
                break;
              case 2:
                myRobot->diffDrive->stop();
                Serial.println("Executed \"diffDrive->stop()\" command");
                break;
              case 3:
                myRobot->diffDrive->straight(straightMotorEffort);
                Serial.println("Executed \"diffDrive->straight()\" command");
                break;
              case 4:
                myRobot->diffDrive->save_straight_compensation(leftRightCompensation);
                Serial.println("Saved the straight motion compensation value");
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

char *diffDriveData_Serialize(void)
{
  sprintf(outBuffer, "{\"diffDriveData\": {\"leftMotorEffort\": %3.2f, \"rightMotorEffort\": %3.2f, \"straightMotorEffort\": %3.2f, \"leftRightCompensation\": %3.2f}}", leftMotorEffort, rightMotorEffort, straightMotorEffort, leftRightCompensation);
  return outBuffer;
}

/*** mqttc de-serialization functions *****************************************/

int leftMotorEffort_Deserialize(void)
{
  char input[256];
  strcpy(input, myRobot->mqttc->receive_message());
  StaticJsonDocument<48> doc;
  DeserializationError error = deserializeJson(doc, input, sizeof(input));
  if(error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return 0;
  }
  else
  {
    if(doc.containsKey("leftMotorEffort"))
    {
      leftMotorEffort = doc["leftMotorEffort"];         // Extract/save "leftMotorEffort" value
      return 1;
    }
    else
    {
      Serial.println("Parsing failed! Invalid Key\r\n"); // No valid keys found in JSON Message
      return 0;
    }
  }
}

int rightMotorEffort_Deserialize(void)
{
  char input[256];
  strcpy(input, myRobot->mqttc->receive_message());
  StaticJsonDocument<48> doc;
  DeserializationError error = deserializeJson(doc, input, sizeof(input));
  if(error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return 0;
  }
  else
  {
    if(doc.containsKey("rightMotorEffort"))
    {
      rightMotorEffort = doc["rightMotorEffort"];         // Extract/save "rightMotorEffort" value
      return 1;
    }
    else
    {
      Serial.println("Parsing failed! Invalid Key\r\n"); // No valid keys found in JSON Message
      return 0;
    }
  }
}

int straightMotorEffort_Deserialize(void)
{
  char input[256];
  strcpy(input, myRobot->mqttc->receive_message());
  StaticJsonDocument<48> doc;
  DeserializationError error = deserializeJson(doc, input, sizeof(input));
  if(error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return 0;
  }
  else
  {
    if(doc.containsKey("straightMotorEffort"))
    {
      straightMotorEffort = doc["straightMotorEffort"];         // Extract/save "straightMotorEffort" value
      return 1;
    }
    else
    {
      Serial.println("Parsing failed! Invalid Key\r\n"); // No valid keys found in JSON Message
      return 0;
    }
  }
}

int event_Deserialize(void)
{
  char input[256];
  strcpy(input, myRobot->mqttc->receive_message());
  StaticJsonDocument<48> doc;
  DeserializationError error = deserializeJson(doc, input, sizeof(input));
  if(error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return 0;
  }
  else
  {
    if(doc.containsKey("event"))
    {
      event = doc["event"];         // Extract/save "event" value
      return 1;
    }
    else
    {
      Serial.println("Parsing failed! Invalid Key\r\n"); // No valid keys found in JSON Message
      return 0;
    }
  }
}