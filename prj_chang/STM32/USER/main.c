#include "delay.h"
#include "config.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"
#include "ds18b20.h"

unsigned char tempMax=27;
unsigned char strDat[10];
unsigned char buFang = 1;
unsigned char alarmFlag=0;
void showtitle()
{
	u8 offset = 33;
	OLED_ShowCHinese(0+offset,0,0);//智能家居
	OLED_ShowCHinese(18+offset,0,1);//
	OLED_ShowCHinese(36+offset,0,2);//
	OLED_ShowCHinese(54+offset,0,3);//
	
	offset = 0;
	OLED_ShowCHinese(0+offset,2,4);//温度
	OLED_ShowCHinese(18+offset,2,5);//
	OLED_ShowString(36+offset,2,":",16);
	OLED_ShowCHinese(95+offset,2,14);//箭头
	
	offset = 0;
	OLED_ShowCHinese(0+offset,4,6);//人体
	OLED_ShowCHinese(18+offset,4,7);//
	OLED_ShowString(36+offset,4,":",16);
	OLED_ShowChar(50+offset,4,'X' ,16);//人体
	
	offset = 70;
	OLED_ShowCHinese(0+offset,4,8);//火焰
	OLED_ShowCHinese(18+offset,4,9);//
	OLED_ShowString(36+offset,4,":",16);
	OLED_ShowChar(50+offset,4,'X' ,16);//火焰
	
	offset = 0;
	OLED_ShowCHinese(0+offset,6,10);//烟雾
	OLED_ShowCHinese(18+offset,6,11);//
	OLED_ShowString(36+offset,6,":",16);
	OLED_ShowChar(50+offset,6,'X' ,16);//烟雾
	
	offset = 70;
	OLED_ShowCHinese(0+offset,6,12);//布防
	OLED_ShowCHinese(18+offset,6,13);//
	OLED_ShowString(36+offset,6,":",16);
	OLED_ShowChar(50+offset,6,'Y' ,16);//布防
}
int main(void)
{	
//	u8 len;	
	short temperature;   
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();			     //LED端口初始化

	OLED_Init();			//初始化OLED  
	OLED_Clear()  	; 
	Io_CONFIG_Init();//初始化所有io口
	uart_init(115200);

		
	while(DS18B20_Init())	//DS18B20初始化	
	{
		OLED_ShowString(0,0,"DS18B20 Error",16);
		delay_ms(200);
		OLED_ShowString(0,0,"DS18B20      ",16);
		delay_ms(200);
	}
	showtitle();//显示基本布局
	while(1) 
	{		
	//******************
		temperature=DS18B20_Get_Temp();	
		OLED_ShowNum(0+45,2,temperature/10,2,16);	//显示正数部分	  
		OLED_ShowChar(16+45,2,'.',16);
		OLED_ShowNum(18+45,2,temperature%10,2,16);	//显示小数部分 
		//BEEP=0;
		delay_ms(1000);
		//printf("hello\r\n");
		//***********************************
		if(USART_RX_STA&0x8000)
		{					   
		//	len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度USART_RX_BUF[t];
			if(USART_RX_BUF[0]>='0'&&USART_RX_BUF[0]<='9')
				tempMax=(USART_RX_BUF[0]-0x30)*10+(USART_RX_BUF[1]-0x30);
			if(USART_RX_BUF[2]=='0'){
				OLED_ShowChar(50+70,6,'X' ,16);//布防
				buFang=0;
			}else
			{
				OLED_ShowChar(50+70,6,'Y' ,16);//布防
				buFang=1;
			}
			USART_RX_STA=0;
		}
		OLED_ShowNum(110,2,tempMax,2,16);	//显示温度最大值
		//=========================================
		if(temperature>tempMax*10)//检测温度
		{	
			if(buFang==1)
				alarmFlag=1;
		}
		//=========================================
		if(HuoYan==1)//火焰
		{	
			OLED_ShowChar(50+70,4,'X' ,16);//火焰
		}
		else
		{
			if(buFang==1)
				alarmFlag=1;
			OLED_ShowChar(50+70,4,'Y' ,16);//火焰
		}
		//=========================================
		if(YanWu==0)//烟雾
		{	if(buFang==1)
				alarmFlag=1;
			OLED_ShowChar(50+0,6,'Y' ,16);//烟雾
		}
		else
		{
		
			OLED_ShowChar(50+0,6,'X' ,16);//烟雾
		}
		//=========================================
		if(RenTi==1)//人体
		{	if(buFang==1)
				alarmFlag=1;
			OLED_ShowChar(50+0,4,'Y' ,16);//人体
		}
		else
		{
		
			OLED_ShowChar(50+0,4,'X' ,16);//人体
		}
		
		//==================
		if(KEY==0)//按键
		{	
			unsigned int time=0;
			
			while(KEY==0){
				delay_ms(1000);
				time++;
				if(time>=10)
				{
					printf("m0\r\n");
					break;
				}
			}

			if(buFang==1){
				OLED_ShowChar(50+70,6,'X' ,16);//布防
				buFang=0;
			}else
			{
				OLED_ShowChar(50+70,6,'Y' ,16);//布防
				buFang=1;
			}
			
		}
		//=============发送各个传感器信息======
		printf("%d,",HuoYan);
		printf("%d,",YanWu);
		printf("%d,",RenTi);
		printf("%d,",temperature);
		printf("%d,",tempMax);
		printf("%d,",buFang);
		printf("%d,",alarmFlag);
		printf("\r\n");
		//============蜂鸣器报警==========
		if(alarmFlag==1)
		{
			BEEP=0;
			alarmFlag=0;
		}else
		{
			BEEP=1;
		}
	}	  
	
}



