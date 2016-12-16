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

int keyMap[] = {6, 13, 4, 12, 5};

char keyName[5][7] = {"UP\0", "CENTER\0", "DOWN\0", "LEFT\0", "RIGHT\0"};     

int readKey(int num)
{
    int key = digitalRead(keyMap[num]);
    if(key == LOW)
    {
        // Delay for a while and re-detect the key status
        // Filtering glitches on the signal
        delay(10);
        if(key == LOW)
            return 1;
    }
    return 0;
}

int main(void)
{
    int i;
    nightWiringSetup();
    nightWiringGpioSetup(fennecGpioMap, 17);

    for(i=0; i<5; i++)
        pinMode(keyMap[i], INPUT);

    while(1)
    {
        for(i=0; i<5; i++)
        {
            if(readKey(i))
            {
                printf("Key %s is pressed!\n", keyName[i]);
                delay(500);
            }          
        }
    }
    return 0;
}
