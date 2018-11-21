#pragma once


#include <iostream>
#include <unistd.h>   // close
#include <fcntl.h>    // O_RDWR
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>  // I2C_SLAVE
#include <string>
#include <thread>




class QT1070 {

     public:
          enum Key {
               LEFT = 1,
               RIGHT = 2,
               UP = 4,
               DOWN = 8,
               B = 16,
               A = 32,
               X = 64
          };

     public:
          QT1070();
          int get_chip_id();
          Key button_pressed();

     private:
          const static int I2CADDRESS = 0x1B;
          const static std::string DEVICE;

          int i2cfile;

};

