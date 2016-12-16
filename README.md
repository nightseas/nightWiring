# Night Wiring

By Xiaohai Li (haixiaolee@gmail.com)

For now it's a cross-platform C lib to control the hardware interfaces including UART, I2C and SPI and GPIOs.
This lib is rewritten from wiringPi.

The GPIO function is based on sysfs for now, which is a little bit slower but flexible.

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
