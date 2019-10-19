#include "stm32f10x.h"
#include "usart.h"
#include "ultrasonic.h"
#include "stm32f10x_gpio.h"

#include "delay.h"//延时函数

#include <stdio.h>
#include <math.h>

#include "max30102.h"
#include <stm32f10x.h>
#include "oled.h"
#include "bmp.h"

/*************************************************
函数: int main(void)
功能: main主函数
参数: 无
返回: 无
**************************************************/
int main(void)
{
	unsigned char len;
	uart_init(9600);
	bsp_InitI2C();
	delay_init(72);            //延时初始化
	max30102_init();
	
	OLED_Init();			//初始化OLED  
		OLED_Clear()  	; 
	
	
//		OLED_ShowCHinese(0,0,0);//中
//				OLED_ShowCHinese(18,0,1);//景
//		OLED_ShowCHinese(36,0,2);//园
//		OLED_ShowCHinese(54,0,3);//电
//		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowString(30,0,"Wenwen",16);	
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
	printf("\r\n MAX30102========  init  \r\n");

	while(1)
	{

		
		u32 bet = test_max30102_fun();		
		if(bet)
		{
			putChar(0xa5);//低位先发
//			printf("%ld\r\n",bet);
			bet=bet%100000;
			putChar(bet);
			putChar(bet>>8);
			putChar(bet>>26);
			putChar(bet>>24);
			putChar((u8)(bet)+(u8)(bet>>8)+(u8)(bet>>16)+(u8)(bet>>24));
			putChar(0x5a);
		}
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			USART_RX_BUF[len]='\0';
			
			printf("\r\n%d\r\n",USART_RX_BUF);
			
			USART_RX_STA=0;
		}
	}
}

//#if 0	
//	while(1)
//	{
//		delay_ms(1000);            
//		max30102_init();
//		temp_num = max30102_Bus_Read(0x1f);
//		printf("当前温度 = %d\r\n",temp_num);
//	}
//#endif
