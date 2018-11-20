#include "qt1070.h"


const std::string QT1070::DEVICE = "/dev/i2c-1";

QT1070::QT1070() {
    
    std::cout << "\n\n-------------------------------" << std::endl;
    std::cout << "Starting touchberry application" << std::endl;
    std::cout << "-------------------------------\n" << std::endl; 
  
  if ((i2cfile = open(DEVICE.c_str(), O_RDWR)) < 0) {
      std::cout << "Could not open bus" << std::endl;
      exit(1);
  }
    std::cout << "Successfully opened the i2c bus" << std::endl;

  if (ioctl(i2cfile, I2C_SLAVE, I2CADDRESS) < 0) {
      std::cout << "Failed to acquire bus access and/or talk to slave." << std::endl;
      exit(1);
  }
    std::cout << "Ready to communicate with slave device\n" << std::endl;
}

int QT1070::get_chip_id() {
    int chipID = 0;

    
    // Write
    char buffer[] = { 0x00 };
    write(i2cfile, buffer, 1);


    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    // Read
    if (read(i2cfile, buffer, 1) != 1) {
        std::cout << "Failed to read from the i2c bus.\n" << std::endl;
    }
    else {
        chipID = (int)buffer[0];
    }


    return chipID;
}

QT1070::Key QT1070::button_pressed() {
    int buttonPressed = 0;

    // Write
    char buffer[] = { 0x03 };
    write(i2cfile, buffer, 1);


    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    // Read
    if (read(i2cfile, buffer, 1) != 1) {
        std::cout << "Failed to read from the i2c bus.\n" << std::endl;
    }
    else {
        buttonPressed = (int)buffer[0];
    }


    return (Key)buttonPressed;
}




