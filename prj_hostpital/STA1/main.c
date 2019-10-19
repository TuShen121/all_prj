#include <reg52.h>
#include "config.h"
#include "uart.h"
unsigned int countTime=0;
unsigned char Status=0x10;

void delay(unsigned int i)
{
	while(i--);
}

void uartDel(unsigned char DAT)
{
	if(DAT==0xF0)
	{
		Status=0x10;
	}
}
void main(void)
{
//	unsigned char len;
    InitUART();
	while(1)
	{
		countTime++;
		if(countTime==50000)//¥Û∏≈5√Î
		{
			countTime=0;
			SendOneByte(Status);
			SendString("\r\n",2);
			if(Status==0x10)
			{
				LED=0;
			}else if(Status==0x11)
			{
				LED=!LED;
			}
		}
		if(KEY==0)
		{
			delay(100);
			Status=0x11;
			if(KEY==0)
			{
	//			SendOneByte(Status);
	//			SendString("\r\n",2);
				while(KEY==0);
			}
		}
	}
}
