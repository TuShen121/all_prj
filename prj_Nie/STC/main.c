#include "reg51.h"
#include "intrins.h"
#include "config.h"
#include "oled.h"
#include "bmp.h"
#include "ds18b20.h"
#include "stdio.h"
#include "uart.h"

unsigned char phonenum[]="AT+CMGS=\"17858366909\"";
unsigned char alarmtex[]="Found the fire";
void mDelay(unsigned int i)
{
	while(i--);
}
void showInit()
{
	unsigned char offset_x=0;
	offset_x=8;
	OLED_ShowCHinese(offset_x+0,0,0);//火
	OLED_ShowCHinese(offset_x+18,0,1);//灾
	OLED_ShowCHinese(offset_x+36,0,2);//报
	OLED_ShowCHinese(offset_x+54,0,3);//警
	OLED_ShowCHinese(offset_x+72,0,4);//系
	OLED_ShowCHinese(offset_x+90,0,5);//统

	OLED_ShowCHinese(0,2,6);//温
	OLED_ShowCHinese(18,2,7);//度
	
	OLED_ShowCHinese(0,4,8);//烟雾
	OLED_ShowCHinese(18,4,9);//
	OLED_ShowChar(40,4,'X',16);//烟雾
	
	OLED_ShowCHinese(54+5,4,10);//火焰
	OLED_ShowCHinese(72+5,4,11);//
	OLED_ShowChar(100,4,'X',16);//火焰
	
	OLED_ShowCHinese(0,6,6);//温阈
	OLED_ShowCHinese(18,6,12);//
}

void main()
{
	unsigned char tempStr[5];
	float temp=0;
	static unsigned int tempE=30;
	unsigned int time=0;
	unsigned alarmFlag=0;
	P0M0|=0x08;
	P0M1|=0x08;

	P2M0|=0x80;
	P2M1|=0x80;
	
	
	InitUART();
	OLED_Init();//初始化OLED  
	OLED_Clear(); 
	showInit();
	mDelay(60000);
    while (1)
	{	
		
		
		
		temp=(float)Get_Tmp()/10;
		mDelay(60000);
		mDelay(60000);
		if(temp>=(float)tempE ){//如果温度超过阈值
			BEEP=0;
			LED=0;
			alarmFlag=1;
		}
		if(yanwu==0)//如果烟雾传感器报警
		{
			BEEP=0;
			LED=0;
			OLED_ShowChar(40,4,'Y',16);//烟雾
			alarmFlag=1;
		}else 
		{
			OLED_ShowChar(40,4,'X',16);//烟雾
		}
		if(huoyan==0)//如果火焰传感器报警
		{
			BEEP=0;
			LED=0;
			OLED_ShowChar(100,4,'Y',16);//火焰
			alarmFlag=1;
		}
		else
		{
			OLED_ShowChar(100,4,'X',16);//火焰
		}
		if(temp<(float)tempE &&  yanwu!=0 && huoyan!=0){//如果三者都没有警报
			BEEP=1;
			LED=1;
			alarmFlag=0;
		}
		
		if(alarmFlag==1)//如果发生警报
		{	
			if(time==10)//刚开机的时候温度会乱码，所以等报警时间大于50个周期在发送短信，确保报警正确
			{
				SendString(phonenum,21);
				SendOneByte(0x0d);
				mDelay(60000);
				SendString(alarmtex,14);
				SendOneByte(0x1a);
			}
			time++;
			if(time>=1200){//10一秒  每两分钟发生一次
				time=0;
			}	
		}else
		{
			time=0;
		}
		
		sprintf(tempStr,"%5f",temp);
		tempStr[4]='\0';
		OLED_ShowString(36,2,tempStr,16);	 //显示温度
		OLED_ShowNum(36,6,tempE,3,16);	 //显示温度阈值
		
		if(KEY_UP==0)
		{
			mDelay(60000);
			if(KEY_UP==0)
				tempE++;
		}
		else if(KEY_DWON==0)
		{
			mDelay(60000);
			if(KEY_DWON==0)
				tempE--;
		}
		
	}
}

