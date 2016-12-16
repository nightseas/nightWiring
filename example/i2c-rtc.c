/***************************************************
            For nightWiring I2C Test
        By Xiaohai Li (haixiaolee@gmail.com)
***************************************************/

#include "nightWiring.h"
#include "nightWiringI2C.h"
#include "stdio.h"

#define DS1307_ADDR 0x68

int i2cFd;
int quitFlag = 0;


unsigned char DEC2BCD(unsigned char val)
{
   return ( (val/10*16) + (val%10) );
}

unsigned char BCD2DEC(unsigned char val)
{
   return ( (val/16*10) + (val%16) );
}


void rtcSetTime(unsigned char year, unsigned char month, unsigned char date, unsigned char dayofWeek, unsigned char hour, unsigned char min ,unsigned char sec)
{
  unsigned char reg = 0x00;

  i2cWriteReg8(i2cFd, 0x00, DEC2BCD(sec));
  i2cWriteReg8(i2cFd, 0x01, DEC2BCD(min));
  i2cWriteReg8(i2cFd, 0x02, DEC2BCD(hour));
  i2cWriteReg8(i2cFd, 0x03, DEC2BCD(dayofWeek));
  i2cWriteReg8(i2cFd, 0x04, DEC2BCD(date));
  i2cWriteReg8(i2cFd, 0x05, DEC2BCD(month));
  i2cWriteReg8(i2cFd, 0x06, DEC2BCD(year));   
}


void rtcGetTime() 
{
  unsigned char year, month, date, dayofWeek, hour, min ,sec;
  unsigned char reg = 0x00;

  sec       = BCD2DEC(i2cReadReg8(i2cFd, 0x00) & 0x7f);
  min       = BCD2DEC(i2cReadReg8(i2cFd, 0x01));
  hour      = BCD2DEC(i2cReadReg8(i2cFd, 0x02) & 0x3f);
  dayofWeek = BCD2DEC(i2cReadReg8(i2cFd, 0x03));
  date      = BCD2DEC(i2cReadReg8(i2cFd, 0x04));
  month     = BCD2DEC(i2cReadReg8(i2cFd, 0x05));
  year      = BCD2DEC(i2cReadReg8(i2cFd, 0x06));

  printf("Time: %02d-%02d-%02d  %02d:%02d:%02d.\n", year, month, date, hour, min ,sec);
}

int main(void)
{
    int i;
    if((i2cFd=i2cSetup("/dev/i2c-4", DS1307_ADDR)) < 0)
    {
        printf("Error: I2C acess failed! i2cSetup() return %d\n",i2cFd);
        return 0;
    }
    printf("I2C interface init complete.\n");

    printf("Writing time 2016-10-01 Sat 21:10:00 to RTC...\n");
    rtcSetTime(16,10,1,6,21,10,0);

    while(1)
    {
        rtcGetTime();
        delay(1000);
    }
    return 0;
}

