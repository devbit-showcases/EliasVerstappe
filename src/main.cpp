
using namespace std;
#include "lib/qt1070.h"
#include "lib/tlc59116.h"
#include <simple_mqtt_client/simple_mqtt_client.h>


using namespace BiosSimpleMqttClient;

enum Key {
  NONE = 0,
  LEFT = 1,
  RIGHT = 2,
  UP = 4,
  DOWN = 8,
  B = 16,
  A = 32,
  X = 64
  };

class KeyConverter {

    public:
    static std::string key_to_string(Key key) {
      switch (key) {
        case UP:
          return "UP";
        case DOWN:
          return "DOWN";
        case LEFT:
          return "LEFT";
        case RIGHT:
          return "RIGHT";
        case X:
          return "X";
        case B:
          return "B";
        case A:
          return "A";
        case NONE:
          return "NONE";  
      }
      return "";     // Unknown key? Should never happen
    }
};

const std::string SERVER_ADDRESS("tcp://mqtt.labict.be:1883");
const std::string CLIENT_ID("eeltbal123456");
const std::string TOPIC("test/hello");
const std::string JOIN("test/nubg/join");
const std::string UPDATE("test/nubg/devgame/update");


int main(void) {

  std::string registerTank = "{ \"name\": \"eeltbal\" }";
  std::string JWT_TOKEN = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0YW5rX2lkIjoiMmZiZTNiODM1N2U0NmJiZTRjOWY3ZGFkOTVlMWU5ZmRmMzNkZWY1ZSIsImdhbWVfaWQiOiJkZXZnYW1lIn0.R1NvwJJBPjVMpiUUuq50Cl7XWtC1046e6cCG5m-fkMk";
  
  std::string driveFwd = "{ \"token\": \"" + JWT_TOKEN + "\", \"drive\": 10, \"turn\": 0 }";
  std::string driveBwd = "{ \"token\": \"" + JWT_TOKEN + "\", \"drive\": -10, \"turn\": 0 }";
  std::string turnLeft = "{ \"token\": \"" + JWT_TOKEN + "\", \"drive\": 10, \"turn\": -30 }";
  std::string turnRight = "{ \"token\": \"" + JWT_TOKEN + "\", \"drive\": 10, \"turn\": 30 }";






  std::cout << "\n\n\033[7m+-------------------------------+" << std::endl;
  std::cout << "|Starting touchberry application|" << std::endl;
  std::cout << "+-------------------------------+\033[0m\n" << std::endl; 

  QT1070 touch;
  TLC59116 led;
  Key button;

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  led.clearLeds();

  std::cout << "\033[4;35mMaking MQTT connection...\033[0m" << std::endl;
  SimpleMQTTClient simpleClient(SERVER_ADDRESS, CLIENT_ID);
  std::this_thread::sleep_for(std::chrono::milliseconds(2500));
  std::cout << "\033[4;35mConnected succesfully!\033[0m" << std::endl;



  MQTTMessage message(JOIN, registerTank);
  simpleClient.publish(message);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));


  while (1) {
  button = (Key)touch.button_pressed();
    if (button != NONE) {
      cout << "Button pressed: " << KeyConverter::key_to_string(button) << endl;

      switch(button) {
        case RIGHT : {
          MQTTMessage message(UPDATE, turnRight);
          simpleClient.publish(message);
          led.setLedNr(4, 0, 0, 0xCC);
          led.setLedNr(5, 0, 0, 0xCC);
          std::this_thread::sleep_for(std::chrono::milliseconds(75));
          break;}
        
        case LEFT : {
          MQTTMessage message(UPDATE, turnLeft);
          simpleClient.publish(message);
          led.setLedNr(1, 0, 0, 0xCC);
          led.setLedNr(2, 0, 0, 0xCC);
          std::this_thread::sleep_for(std::chrono::milliseconds(75));
          break;}

        case UP : {
          MQTTMessage message(UPDATE, driveFwd);
          simpleClient.publish(message);
          led.setAllLeds(0, 0xCC, 0);
          break;}
        
        case DOWN : {
          MQTTMessage message(UPDATE, driveBwd);
          simpleClient.publish(message);
          led.setAllLeds(0x99, 0, 0);
          break;}

        case B :
          led.colorLoop();
        
          break;
        
        case X :
          
        
          break;

        case A :
        
          break;


        
        
      }
        
    }
  
  led.clearLeds();
  }


  return 0;
}


