#include "reg51.h"
#include "intrins.h"
#include "config.h"
#include "servo.h"
#include "oled.h"
#include "bmp.h"
#include "echo.h"
#include "ds18b20.h"
#include "stdio.h"

void mDelay(unsigned int i)
{
	while(i--);
}
void showInit()
{
	unsigned char offset_x=0;
	offset_x=20;
	OLED_ShowCHinese(offset_x+0,0,0);//智
	OLED_ShowCHinese(offset_x+18,0,1);//能
	OLED_ShowCHinese(offset_x+36,0,2);//垃
	OLED_ShowCHinese(offset_x+54,0,3);//圾
	OLED_ShowCHinese(offset_x+72,0,4);//桶

	OLED_ShowCHinese(0,2,5);//温
	OLED_ShowCHinese(18,2,6);//度
	
	OLED_ShowCHinese(0,4,7);//容
	OLED_ShowCHinese(18,4,8);//量
	
	OLED_ShowCHinese(0,6,5);//温
	OLED_ShowCHinese(18,6,9);//阈
}

void main()
{
	unsigned char tempStr[5];
	float temp=0;
	static unsigned int tempE=30;
	unsigned int closeMinTime=0;
	P0M0=0x40;
	P0M1=0x40;
	
	servoInit();
	OLED_Init();//初始化OLED  
	OLED_Clear(); 
	showInit();
	
    while (1)
	{	
		
		temp=(float)Get_Tmp()/10;
		if(temp>=(float)tempE){
			BEEP=0;
		}else{
			BEEP=1;
		}
		
		sprintf(tempStr,"%5f",temp);
		tempStr[4]='\0';
		OLED_ShowNum(36,4,getEcho(),3,16);	 //显示超声波
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
		
		if(RED==0)//
		{
			closeMinTime++;
			if(closeMinTime>=60){
				
				servoTurn();
			}
			LED=1;
		}
		else if(RED==1)//有人来了
		{
			closeMinTime=0;

			servoReturn();
			LED=0;
		}
	}
}

