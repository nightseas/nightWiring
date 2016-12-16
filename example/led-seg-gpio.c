/***************************************************
            For nightWiring GPIO Test
        By Xiaohai Li (haixiaolee@gmail.com)
***************************************************/

#include "nightWiring.h"
#include "nightWiringGPIO.h"
#include "nightWiringShift.h"
#include "stdio.h"
#include "fontEnSeg7.h"

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

void MAX7219_Write(unsigned char regAddr,unsigned char data)
{
    digitalWrite(8, LOW);
    shiftOut(15, 16, MSBFIRST, regAddr);
    shiftOut(15, 16, MSBFIRST, data);
    digitalWrite(8, HIGH);
}


void MAX7219_Init(void)
{
    MAX7219_Write(0x09, 0x00);       
    MAX7219_Write(0x0A, 0x03);       
    MAX7219_Write(0x0B, 0x07);       
    MAX7219_Write(0x0C, 0x01);      
    MAX7219_Write(0x0F, 0x00);      
}


int main(void)
{
    int i, j, h;

    unsigned char msg[8];

    nightWiringSetup();
    nightWiringGpioSetup(fennecGpioMap, 17);

    pinMode(8, OUTPUT);
    pinMode(15, OUTPUT);
    pinMode(16, OUTPUT);

    digitalWrite(8, HIGH);
    digitalWrite(15, LOW);
    digitalWrite(16, LOW);

    MAX7219_Init();
    printf("MAX7219 init complete, display loop starting...\n");

    while(1)
    {
        for(j=0;j<10;j++)
        {
            for(i=1;i<=8;i++)
            {
                MAX7219_Write(i, stab[2*j+3]);
            }
            printf(">");
            fflush(stdout);
            delay(500);
        }
    }
    return 0;
}
