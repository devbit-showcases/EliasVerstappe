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
    initialize();

}

void TLC59116::setGroupBrightness(float brightness) {
    setRegister(GRPPWM, brightness * 255.0);
}

void TLC59116::setLed(int index, float brightness) {
    setRegister((FIRSTLED + (15-index)), (brightness * 255.0));
}

void TLC59116::setLedNr(int ledNumber, int red, int green, int blue) {
    int REDLEDS[] = {0x0E, 0x0B, 0x08, 0x05, 0x02};
    int GREENLEDS[] = {0x0F, 0x0C, 0x09, 0x06, 0x03};
    int BLUELEDS[] = {0x10, 0x0D, 0x0A, 0x07, 0x04};

    int ledIndex = ledNumber - 1;

    setRegister(REDLEDS[ledIndex], red);       //red
    setRegister(GREENLEDS[ledIndex], green);   //green
    setRegister(BLUELEDS[ledIndex], blue);      //blue
    //mss delays ertussen?
}

void TLC59116::setAllLeds(int red, int green, int blue) {
    int REDLEDS[] = {0x0E, 0x0B, 0x08, 0x05, 0x02};
    int GREENLEDS[] = {0x0F, 0x0C, 0x09, 0x06, 0x03};
    int BLUELEDS[] = {0x10, 0x0D, 0x0A, 0x07, 0x04};

    for (int i = 0; i < 5; i++){
        setRegister(REDLEDS[i], red);       //red
        setRegister(GREENLEDS[i], green);   //green
        setRegister(BLUELEDS[i], blue);      //blue
    }
    
}


void TLC59116::initialize() {
    // oscillator set to normal mode 
    enable();
    // enable individual brightness and group dimming
    setOutputState(0xFF,0xFF,0xFF,0xFF);
}

void TLC59116::enable() {
    // should readout register state first
    setRegister(0x00, 0);
}

void TLC59116::setOutputState(int ledout0, int ledout1, int ledout2, int ledout3) {
    setRegister(0x14,ledout0);
    setRegister(0x15,ledout1);
    setRegister(0x16,ledout2);
    setRegister(0x17,ledout3);
}

void TLC59116::setRegister(char reg, char value) {
    char data[] = {(char)reg, value};
    write(i2cfile, data, 2);
}

int TLC59116::readRegister(char address) {
    char data[] = { address };
    read(i2cfile, data, 1);

    return data[0];
}

void TLC59116::clearLeds() {
    for (int i = 2; i < 17; i++) {
        setRegister(i, 0);
    }
}

void TLC59116::disableLed(int ledNumber) {
    setLedNr(ledNumber, 0, 0, 0);
}

void TLC59116::randomAll() {
    for (int i = 2; i < 17; i++) {
        setRegister(i, (rand() % 255));
    }
}

void TLC59116::insideOut() {
    int speed = 200;

    for (int i = 0; i < 4; i++) {
        disableLed(1);
        disableLed(2);
        setLedNr(3, 255, 255, 255);
        disableLed(4);
        disableLed(5);
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));

        disableLed(1);
        setLedNr(2, 255, 255, 255);
        disableLed(3);
        setLedNr(4, 255, 255, 255);
        disableLed(5);
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));

        setLedNr(1, 255, 255, 255);
        disableLed(2);
        disableLed(3);
        disableLed(4);
        setLedNr(5, 255, 255, 255);
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));

        clearLeds();
    }
    
}

void TLC59116::outsideIn() {
    int speed = 200;

    for (int i = 0; i < 4; i++) {
        setLedNr(1, 255, 255, 255);
        disableLed(2);
        disableLed(3);
        disableLed(4);
        setLedNr(5, 255, 255, 255);
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));

        disableLed(1);
        setLedNr(2, 255, 255, 255);
        disableLed(3);
        setLedNr(4, 255, 255, 255);
        disableLed(5);
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));

        disableLed(1);
        disableLed(2);
        setLedNr(3, 255, 255, 255);
        disableLed(4);
        disableLed(5);
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));

        clearLeds();
        
    }
}

void TLC59116::right() {
    int speed = 200;

    for (int i = 1; i < 6; i++) {
        clearLeds();
        setLedNr(i, 255, 255, 255);
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        clearLeds();
    }
}

void TLC59116::left() {
    int speed = 200;
    
    for (int i = 5; i > 0; i--) {
        clearLeds();
        setLedNr(i, 255, 255, 255);
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        clearLeds();
    }
}

void TLC59116::colorLoop() {
    int speed = 2;

    for (int k = 0; k < 255; k++) {
        for (int i = 1; i < 6; i++) {
            setLedNr(i, 255, 0+k, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        }       
    }

    for (int l = 0; l < 255; l++) {
        for (int i = 1; i < 6; i++) {
            setLedNr(i, 255-l, 255, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        }       
    }

    for (int l = 0; l < 255; l++) {
        for (int i = 1; i < 6; i++) {
            setLedNr(i, 0, 255, 0+l);
            std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        }       
    }

    for (int l = 0; l < 255; l++) {
        for (int i = 1; i < 6; i++) {
            setLedNr(i, 0, 255-l, 255);
            std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        }       
    }

    for (int l = 0; l < 255; l++) {
        for (int i = 1; i < 6; i++) {
            setLedNr(i, 0+l, 0, 255);
            std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        }       
    }

    for (int l = 0; l < 255; l++) {
        for (int i = 1; i < 6; i++) {
            setLedNr(i, 255, 0, 255-l);
            std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        }       
    }

    clearLeds();
}

void TLC59116::randomWhite() {
    int color[] = {0xFF, 0x3D, 0x18};

    setLedNr((rand() % 5 + 1), color[(rand() % 2 + 1)], color[(rand() % 2 + 1)], color[(rand() % 2 + 1)]);

}

void TLC59116::prettyColors() {

    setLedNr(1, 0xAA, 0, 0);
    setLedNr(2, 0xAA, 0xA8, 0);
    setLedNr(3, 0, 0xAA, 0x25);
    setLedNr(4, 0, 0x41, 0xAA);
    setLedNr(5, 0x55, 0, 0xAA);

    setGroupBrightness(0.1);
}