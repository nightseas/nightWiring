/***************************************************
           For nightWiring Serial Debug
        By Xiaohai Li (haixiaolee@gmail.com)
***************************************************/

#include "nightWiring.h"
#include "nightWiringSerial.h"
#include "stdio.h"
#include "string.h"

int pipFd;
int quitFlag = 0;
char bmcBuffer[] = {0x55, 0x01, 0x00, 0x00, 0x00, 0xAA};

int SerialInit(void)
{
    pipFd = serialOpen("/dev/ttyS1", 115200);
    return pipFd;
}

void* ThserialGet(void *args)
{
    while(1)
    {
        if(serialDataAvail(pipFd) > 0)
            putchar(serialGetchar(pipFd));
    }
}

int main(void)
{
    char inChar = 0x00;
    int i;
    printf("Init serial interface...\n");
    if(SerialInit() < 0)
    {
        printf("Error: Serial init failed! serialOpen() return %d\n", pipFd);
        return 0;
    }
    printf("Serial interface init complete.\n");

    if(nightWiringThreadCreate(ThserialGet) < 0)
    {
        printf("Error: Serail rx thread creating failed!\n");
        return 0;  
    }

    while(!quitFlag)
    {
        printf("\n\n=== BMC Debug Tool (by Xiaohai Li) ===\n 1. Get board info.\n 2. Get all sensors.\n 3. System power off.\n 4. System power on(debug).\n q. Quit.\n\nInput>\n");
        inChar = getchar();
        switch(inChar)
        {
            case '1':
                bmcBuffer[1] = 0x01;
                write(pipFd, bmcBuffer, 6);
                serialFlush(pipFd);
                break;
            case '2':
                bmcBuffer[1] = 0x02;
                bmcBuffer[2] = 0x00;
                write(pipFd, bmcBuffer, 6);
                serialFlush(pipFd);
                break;
            case '3':
                bmcBuffer[1] = 0x03;
                bmcBuffer[2] = 0x01;
                bmcBuffer[3] = 0x01;
                write(pipFd, bmcBuffer, 6);
                serialFlush(pipFd);
                break;
            case '4':
                bmcBuffer[1] = 0x03;
                bmcBuffer[2] = 0x01;
                bmcBuffer[3] = 0x00;
                write(pipFd, bmcBuffer, 6);
                serialFlush(pipFd);
                break;
            case 'q':
                quitFlag = 1;
                break;
        }
    }
    serialClose(pipFd);
    return 0;
}

