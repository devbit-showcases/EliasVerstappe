#include "tlc59116.h"


const std::string TLC59116::DEVICE = "/dev/i2c-1";

TLC59116::TLC59116() {
  
    std::cout << "\033[4;35mConnecting to TLC59116...\033[0m" << std::endl;

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

void TLC59116::setGroupBrightness(float brightness) {
    setRegister(GRPPWM, brightness * 255.0);
}

void TLC59116::initialize() {
    // oscillator set to normal mode 
    enable();
    // enable individual brightness and group dimming
    setOutputState(0xFF,0xFF,0xFF,0xFF);
}

void TLC59116::enable() {
    // should readout register state first
    setRegister(0,0);
}

void TLC59116::setOutputState(int ledout0, int ledout1, int ledout2, int ledout3) {
    setRegister(0x14,ledout0);
    setRegister(0x15,ledout1);
    setRegister(0x16,ledout2);
    setRegister(0x17,ledout3);
}

void TLC59116::setRegister(int reg, int value) {
    char data[] = {NO_AUTO_INCREMENT + reg, value};
    write(i2cfile, data, 2);
}