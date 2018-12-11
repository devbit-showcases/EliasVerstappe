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


TODO - setLedNr.....

![datasheet_led_addresses](/media/led_addresses.PNG)

One of these functions is `colorLoop`.TODO



## MQTT


I have used [this example](https://github.com/iot-devices-2019/simple_mqtt_client/blob/master/examples/hello_mqtt.cpp) to find out how to use mqtt.

The MQTT library's have been installed using the setup script.







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


## Extra's

The buttons and their values   
![buttons_and_their_values](/media/buttons_and_their_values.PNG)



:octocat:
