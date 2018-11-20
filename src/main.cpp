// Compile using g++ -std=c++11 main.cpp -o i2c
// #include <iostream>
// #include <unistd.h>   // close
// #include <fcntl.h>    // O_RDWR
// #include <sys/ioctl.h>
// #include <linux/i2c-dev.h>  // I2C_SLAVE

using namespace std;
// #include <chrono>
// #include <thread>
// #include <fstream>
// #include <string>
// #include <cstring>
// #include <cstdio>
#include "lib/qt1070.h"


int main(void) {

  
  QT1070 touch;
  cout << "Chip ID is: " << touch.get_chip_id() << endl;


// // MBED

//     const int PUSH = 0x00;
//     const int PULL = 0x01;
//     const int CLEAR = 0x02;
//     const int PRINT = 0x03;


//     // READ DATA

//     cout << "\nData read" << endl;
//     cout << "----------------" << endl;
     
//     char key_adress = 0x03;
            
        
//         //--- Put pointer on write adress ---//
//         char read_buffer[] = { PULL, key_adress };
//         write(i2cfile, read_buffer, 2);

//         std::this_thread::sleep_for(std::chrono::milliseconds(10));

//         //--- Retreive data ---//
//         if (read(i2cfile, read_buffer, 5) != 5) {
//             printf("Failed to read from the i2c bus.\n");
//         }
//         else {
//             for(int i = 1; i < 5; i++ ){
//                 if (((k*4)+(i)) <= (unsigned)strlen(txtArray) && (unsigned)strlen(txtArray) < 256){
//                     cout << (char)read_buffer[i];
//                 }
                
//             }  
//         }

//         std::this_thread::sleep_for(std::chrono::milliseconds(5));



    

// cout << endl;

//     // print memory to serial

//     // char cornflakes[1] = { PRINT };
//     // if (write(i2cfile, cornflakes, 1) != 1) {
//     //     cout << "Failed to write to the i2c device." << endl;
//     // } else {
//     //     cout << "Successfully wrote to the i2c device." << endl;
//     // }




//   // Make sure to close the handle
//   close(i2cfile);

  return 0;
}

