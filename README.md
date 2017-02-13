# Night Wiring

By Xiaohai Li (haixiaolee@gmail.com)

It's a cross-platform C lib to control the hardware interfaces including UART, I2C and SPI and GPIOs.
This lib is rewritten from wiringPi (https://github.com/WiringPi/WiringPi).

## Idea

The idea of this little project is to make a HW platform independent peripheral lib. For example, if you only want to use I2C or SPI bus on your platform, usually you need to port the entire wiringPi and fix all the GPIO implementation for your own hardware, also the bus number are static in wiringPi, like I2C0, SPI1. 

A more flexible way is to open a universal interface to each of the peripheral such as I2C, serial port, SPI, or GPIO. Enable the driver in your Kernel and device tree, import the module you need. That's all.

In this way, the GPIO function is based on sysfs for now, which is slower but more flexible. To use the GPIO function a GPIO map is required. In the following demonstration 17 IOs are mapped for RK3288 Fennec board. IO0, first entry of the table, is GPIO2_A0 in RK3288 definition, and it's the 56th GPIO from Linux GPIO map for RK3288.

```C
static int fennecGpioMap[] = {
    /*  GPIO2_A0  */
            56,
    /*  GPIO2_A1,       GPIO2_A2,       GPIO2_A3,       GPIO2_A4    */
            57,             58,             59,             60,
    /*  GPIO2_A5,       GPIO2_A6,       GPIO7_B1,       GPIO2_A7    */
            61,             62,             225,            63,
    /*  GPIO2_B0,        GPIO2_B1,      GPIO2_B2,       GPIO2_B4    */
            64,             65,             66,             68,
    /*  GPIO2_B5,        GPIO7_B0,      GPIO7_A7,       GPIO7_B2    */
            69,             224,            223,            226
};
```

## Installation

```sh
cd nightWiring/
make -j4
sudo make install
```

## Programming

```C
//Including top level header
#include "nightWiring.h"
//Including header of each module by interface
#include "nightWiringSerial.h"
#include "nightWiringSPI.h"
#include "nightWiringI2C.h"
#include "nightWiringGPIO.h"

int main()
{
...
  // Setup nightWiring core function
  nightWiringSetup();

  // Setup nightWiring GPIO function with your GPIO pin map
  nightWiringGpioSetup(yourGpioMap, 17);
  
  // Control GPIO0
  pinMode(0, OUTPUT);  
  digitalWrite(0, HIGH);  
...
}
```

## Compile and execute your program

```sh
gcc -o output_exec_name code_file_name.c -lnightWiring -lpthread
sudo ./output_exec_name
```

Refer to examples for details.
