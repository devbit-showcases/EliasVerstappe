# NUBG with raspberry pi

## Project description

This project is an assignment from the 'IoT devices' course. The intention is to control a robot via the raspberry pi.


## Usage example

To compile this program, log in to your Pi, navigate to the correct directory and type `make` in the terminal.
To compile AND execute, execute `make && ./bin/nubg_touchberry` in the terminal.



## TouchBerry Pi Shield

The TouchBerry Pi Shield is an extention board for the raspberry pi and has two I²C chips.
The QT1070 is the chip that controls the buttons. The TLC59116 is an I²C chip that controls the LEDs.

![TouchBerry Pi Shield](/media/touch_shield.jpg)


### Buttons

The QT1070 chip is used to manage the buttons. I have written a `QT1070` class.

The default constructor initializes the chip.
Next, the class has 2  functions (`get_chip_id` and `button_pressed`).

These functions are actually really simple. All they do is read data from a register, a list of all registers can be found in the datasheet of the QT1070 chip.

![QT1070 registers](/media/qt1070_reg.PNG)

The `get_chip_id` function looks like this:

```c++
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
```

As I said, we need to read the content of a register. To do that you first have to put a 'pointer' on the address where you want to read data from.

That is indicated by the `// Write` part. We write `0x00`, wich is the address for the Chip ID.
After a short delay, the read function is executed. The function returns the Chip ID.

For the buttons the function is very similair. the only part that changes is the register address where data is read from (changes from `0x00` to `0x03`).



### Leds


To use the leds, we first need to enable the oscillator and then enable PWM control.
All of that is done in the `TLC59116` default contructor. First the I2C connection is established and then the `initialize` function is called, which enables the oscillator (via the `enable` function in the `initialize` function). Thereafter PWM is enaled.

I have also made various functions that control the leds such as enabling a single led, all leds, clear one or all leds...

You will also find a couple functions that perform animations.


Here you can see the `setLedNr` function, which makes a certain LED light up in the color you want.

```cpp
void TLC59116::setLedNr(int ledNumber, int red, int green, int blue) {
    int REDLEDS[] = {0x0E, 0x0B, 0x08, 0x05, 0x02};
    int GREENLEDS[] = {0x0F, 0x0C, 0x09, 0x06, 0x03};
    int BLUELEDS[] = {0x10, 0x0D, 0x0A, 0x07, 0x04};

    int ledIndex = ledNumber - 1;

    setRegister(REDLEDS[ledIndex], red);       //red
    setRegister(GREENLEDS[ledIndex], green);   //green
    setRegister(BLUELEDS[ledIndex], blue);     //blue
}
```

First I make three arrays that contain addresses which correspond with one of the three RGB colors (see images below).
Next I make a local variable that 'converts' the LED number to the index (because on the shield you can find led1, led2...).
Finally I set these three leds by writing the corresponding R, G or B value to the register with the R, G or B address.

Here are the addresses we can find in the datasheet:
![datasheet_led_addresses](/media/led_addresses.PNG)

And here you can find what LED corresponds with what address.
![led_addresses_shield](/media/led_addresses_shield.PNG)



One of these functions is `colorLoop`.
If I wanted to use RGB color represenataion, I would have had to make 6 loops that do alost the same thing. The difference beeing that the R, G and B value have to be incremented or decremented.
![color_loop_explained](/media/color_loop_explained.PNG)
the code would look like this:
```cpp
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
            setLedNr(i, value.R, value.G, value.B);
            std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        }       
    }     
    clearLeds();
}
```
As you can see this is a lot of code and it is not DRY.

I fixes this by using a RGB to HSL converter. If you represent color with an HSL code, only one vaiabe has to change. I have found this conversion on the internet, a link can be found in the **Sources** part of this document.

the function can be simplified a lot, as you can see here:
```cpp
void TLC59116::colorLoop() {
    HSL data = HSL(0, 1.00f, 0.50f);
    RGB value = HSLToRGB(data);

    int speed = 2;

    for (int k = 0; k < 360; k++) {
        for (int i = 1; i < 6; i++) {
            data = HSL(k, 1.00f, 0.50f);
            value = HSLToRGB(data);
            setLedNr(i, value.R, value.G, value.B);
            std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        }
    }       
    clearLeds();
}
```
This function is basically a loop that changes the **hue** value from 0 to 359 at e certain speed.



## MQTT


I have used [this example](https://github.com/iot-devices-2019/simple_mqtt_client/blob/master/examples/hello_mqtt.cpp) to find out how to use mqtt.

The MQTT library's have been installed using the setup script.



## Summary

What I have learned from this project is that I2C communication is super easy to do. The only thing you have to do is write some value to some register address. This was a fun and educational assignment.


## Full implementation

[Video on youtube](https://youtu.be/AlaNnouraK0)





## Development setup

[Fork the project](https://github.com/iot-devices-2019/nubg-touchberry-controller-EliasVerstappe)

If you want to contribute, follow the following steps:
1. Create your feature branch (`git checkout -b feature/fooBar`)
2. Commit your changes (`git commit -m 'Add some fooBar'`)
3. Push to the branch (`git push origin feature/fooBar`)
4. Create a new Pull Request

## How to ship a change

If a change is made, please add it to the change log.

## Change log

* 0.0.1
    * Basic functionality
    


### PROBLEMS





## License and author info
See the ``LICENSE`` file for details. In summary, this project is licensed under the MIT license, or public domain if desired and recognized in your jurisdiction.

## Sources
Terminal text customization; visited on 21/11/2018; 
https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal

TLC59116 library; visited on 21/11/2018; 
https://os.mbed.com/users/sillevl/code/TLC59116/

RGB to HSL;
https://www.programmingalgorithms.com/algorithm/rgb-to-hsl?lang=C%2B%2B

HSL to RGB;
https://www.programmingalgorithms.com/algorithm/hsl-to-rgb?lang=C%2B%2B

TLC59116 datasheet;
http://www.ti.com/lit/ds/symlink/tlc59116.pdf



:octocat:
