/***************************************************
            For nightWiring GPIO Test
        By Xiaohai Li (haixiaolee@gmail.com)
***************************************************/

#include "nightWiring.h"
#include "nightWiringGPIO.h"
#include "stdio.h"

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

int ledMap[] = {2, 10, 3, 11};

int main(void)
{
    int i, j;
    nightWiringSetup();
    nightWiringGpioSetup(fennecGpioMap, 17);

    for(i=0; i<4; i++)
        pinMode(ledMap[i], OUTPUT);

    while(1)
    {
        for(i=0; i<4; i++)
        {
            for(j=0; j<4; j++)
                digitalWrite(ledMap[j], HIGH);
            digitalWrite(ledMap[i], LOW);
            delay(500);
        }
    }
    return 0;
}
