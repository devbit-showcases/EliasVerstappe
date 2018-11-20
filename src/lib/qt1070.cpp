#include "qt1070.h"


const std::string QT1070::DEVICE = "/dev/i2c-1";

QT1070::QT1070() {
  
  if ((i2cfile = open(DEVICE.c_str(), O_RDWR)) < 0) {
      std::cout << "Could not open bus" << std::endl;
      exit(1);
  }
  std::cout << "Successfully opened the i2c bus" << std::endl;

  if (ioctl(i2cfile, I2C_SLAVE, I2CADDRESS) < 0) {
      std::cout << "Failed to acquire bus access and/or talk to slave." << std::endl;
      exit(1);
  }
  std::cout << "Ready to communicate with slave device" << std::endl;
}

int QT1070::get_chip_id() {
    
    return 0;
}

