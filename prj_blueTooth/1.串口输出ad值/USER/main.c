#include "stm32f10x.h"
#include "usart.h"
#include "ultrasonic.h"
#include "stm32f10x_gpio.h"

#include "delay.h"//延时函数

#include <stdio.h>
#include <math.h>

#include "max30102.h"


/*************************************************
函数: int main(void)
功能: main主函数
参数: 无
返回: 无
**************************************************/
int main(void)
{
	unsigned char len;
	uart_init(115200);
	bsp_InitI2C();
	delay_init(72);            //延时初始化
	max30102_init();
	printf("\r\n MAX30102========  init  \r\n");

	while(1)
	{	
		unsigned long bet = test_max30102_fun();
		if(bet)
		printf("%ld\r\n",bet);
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			USART_RX_BUF[len]='\0';
			printf("\r\n%s\r\n",USART_RX_BUF);
			
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
