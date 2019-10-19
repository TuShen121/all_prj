#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include "ds1302.h"

int main(void)
{
	u8 time[15];
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //按键初始化
	OLED_Init();
	DS1302_Init();
	delay_ms(10);
	DS1302_Write_Time();
	
	printf("DS1302 TEST,\r\n");
	OLED_ShowString(36,0,"Z-STM32",16);
	OLED_ShowString(20,16,"DS1302 TEST",16);
	OLED_Refresh_Gram();		//更新显示到OLED 
	
	while(1) 
	{
		DS1302_Get_Time(time);
		printf("%d%d%d%d年%d%d月%d%d日 ", time[0],time[1],time[2],time[3],time[4],time[5],time[6],time[7]);
		printf("%d%d时%d%d分%d%d秒 星期%d\n", time[9],time[10],time[11],time[12],time[13],time[14],time[8]);
	 	delay_ms(1000);
		LED_BLUE=!LED_BLUE;
	}	
}

