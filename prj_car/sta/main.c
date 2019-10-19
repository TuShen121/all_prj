#include <STC89C52.h>
#include "uart.h"
#include "config.h"
#include "oled.h"

void delay(unsigned int i)
{
	while(i--);
}
void main()
{
	unsigned char len;
	unsigned int i=0;
	unsigned char sdat[]="00";
	InitUART();
	OLED_Init();
	OLED_Clear();
	OLED_ShowString(0,0, "car light system",16);
	OLED_ShowString(0,2, "speed:",16);
	while(1)
	{
		i++;
		if(i>=20000){
			i=0;
			if(Kup==0){
				delay(100);
				if(Kup==0)
				{
					if(Kp1==0)
					{
						sdat[0]='0';
						sdat[1]='1';
					}
					else if(Kp2==0)
					{
						sdat[0]='0';
						sdat[1]='2';
					}
					else if(Kp3==0)
					{
						sdat[0]='0';
						sdat[1]='3';
					}
				}
			}
			else if(Kdon==0){
				delay(100);
				if(Kdon==0)
				{
					if(Kp1==0)
					{
						sdat[0]='1';
						sdat[1]='1';
					}
					else if(Kp2==0)
					{
						sdat[0]='1';
						sdat[1]='2';
					}
					else if(Kp3==0)
					{
						sdat[0]='1';
						sdat[1]='3';
					}
				}
			}
			else {
				sdat[0]='0';
				sdat[1]='0';
			}
			SendString(sdat,2);
			SendString("\r\n",2);
		}
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			SendString(USART_RX_BUF,len);
			USART_RX_BUF[len]='\0';
			OLED_ShowString(45,4, "    ",16);
			OLED_ShowString(45,4, USART_RX_BUF,16);
			USART_RX_STA=0;
		}
	}
}