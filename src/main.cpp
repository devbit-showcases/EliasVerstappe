
using namespace std;
#include "lib/qt1070.h"


int main(void) {

  
  QT1070 touch;
  cout << "Chip ID is: " << touch.get_chip_id() << endl;

  while (1) {
    cout << "Button pressed: " << touch.button_pressed() << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }





  return 0;
}

