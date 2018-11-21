
using namespace std;
#include "lib/qt1070.h"

enum Key {
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
      }
      return "";     // Unknown key? Should never happen
    }
};


int main(void) {

  
  QT1070 touch;
  cout << "Chip ID is: " << touch.get_chip_id() << endl;
  Key button;


  while (1) {

  button = (Key)touch.button_pressed();

    if (button != NULL) {
        cout << "Button pressed: " << KeyConverter::key_to_string(button) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    
  }





  return 0;
}


