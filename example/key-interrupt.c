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

void keyUpIntHandler(void)
{
    printf("Key UP interrupt is triggered!\n");
    delay(200);
}

void keyDownIntHandler(void)
{
    printf("Key DOWN interrupt is triggered!\n");
    delay(200);
}

void keyLeftIntHandler(void)
{
    printf("Key LEFT interrupt is triggered!\n");
    delay(200);
}

void keyRightIntHandler(void)
{
    printf("Key RIGHT interrupt is triggered!\n");
    delay(200);
}

void keyCenterIntHandler(void)
{
    printf("Key CENTER interrupt is triggered!\n");
    delay(200);
}


int main(void)
{
    int i;
    nightWiringSetup();
    nightWiringGpioSetup(fennecGpioMap, 17);

    nightWiringISR(keyMap[0], INT_EDGE_FALLING, keyUpIntHandler);
    nightWiringISR(keyMap[1], INT_EDGE_FALLING, keyCenterIntHandler);
    nightWiringISR(keyMap[2], INT_EDGE_FALLING, keyDownIntHandler);
    nightWiringISR(keyMap[3], INT_EDGE_FALLING, keyLeftIntHandler);
    nightWiringISR(keyMap[4], INT_EDGE_FALLING, keyRightIntHandler);

    while(1);
    return 0;
}
