/***************************************************
            For nightWiring SPI Test
        By Xiaohai Li (haixiaolee@gmail.com)
***************************************************/

#include "nightWiring.h"
#include "nightWiringSPI.h"
#include "stdio.h"

int spiFd;

unsigned char matrixTable[36][8] = { 
    {0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0
    {0x10,0x30,0x50,0x10,0x10,0x10,0x10,0x10},//1
    {0x7E,0x2,0x2,0x7E,0x40,0x40,0x40,0x7E},//2
    {0x3E,0x2,0x2,0x3E,0x2,0x2,0x3E,0x0},//3
    {0x8,0x18,0x28,0x48,0xFE,0x8,0x8,0x8},//4
    {0x3C,0x20,0x20,0x3C,0x4,0x4,0x3C,0x0},//5
    {0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x0},//6
    {0x3E,0x22,0x4,0x8,0x8,0x8,0x8,0x8},//7
    {0x0,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E},//8
    {0x3E,0x22,0x22,0x3E,0x2,0x2,0x2,0x3E},//9
    {0x8,0x14,0x22,0x3E,0x22,0x22,0x22,0x22},//A
    {0x3C,0x22,0x22,0x3E,0x22,0x22,0x3C,0x0},//B
    {0x3C,0x40,0x40,0x40,0x40,0x40,0x3C,0x0},//C
    {0x7C,0x42,0x42,0x42,0x42,0x42,0x7C,0x0},//D
    {0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C},//E
    {0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40},//F
    {0x3C,0x40,0x40,0x40,0x40,0x44,0x44,0x3C},//G
    {0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44},//H
    {0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C},//I
    {0x3C,0x8,0x8,0x8,0x8,0x8,0x48,0x30},//J
    {0x0,0x24,0x28,0x30,0x20,0x30,0x28,0x24},//K
    {0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C},//L
    {0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81},//M
    {0x0,0x42,0x62,0x52,0x4A,0x46,0x42,0x0},//N
    {0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//O
    {0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20},//P
    {0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D},//Q
    {0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21},//R
    {0x0,0x1E,0x20,0x20,0x3E,0x2,0x2,0x3C},//S
    {0x0,0x3E,0x8,0x8,0x8,0x8,0x8,0x8},//T
    {0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C},//U
    {0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},//V
    {0x0,0x49,0x49,0x49,0x49,0x2A,0x1C,0x0},//W
    {0x0,0x41,0x22,0x14,0x8,0x14,0x22,0x41},//X
    {0x41,0x22,0x14,0x8,0x8,0x8,0x8,0x8},//Y
    {0x0,0x7F,0x2,0x4,0x8,0x10,0x20,0x7F} };//Z
  
unsigned char dataBuff[2];

void MAX7219_Write(unsigned char regAddr,unsigned char data)
{
    dataBuff[0] = regAddr;
    dataBuff[1] = data;
    spiDataTransfer(spiFd, dataBuff, 2);
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
    int i, j;
    if((spiFd = spiSetupMode("/dev/spidev0.0", 8000000, 2)) < 0)
    {
        printf("Error: SPI acess failed! SPISetup() return %d\n",spiFd);
        return 0;
    }
    printf("SPI interface init complete.\n");
    
    MAX7219_Init();
    printf("MAX7219 init complete, display loop starting...\n");
    
    while(1)
    {
        for(j=0;j<36;j++)
        {
            for(i=1;i<=8;i++)
                MAX7219_Write(i, matrixTable[j][i-1]);

            printf(">");
            fflush(stdout);
            delay(500);
        }
    }
    return 0;
}