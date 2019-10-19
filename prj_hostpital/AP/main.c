#include <reg52.h>
#include "oled.h"
#include "uart.h"
unsigned int countTime=0;
unsigned char onlineStatus[]="00";
void delay(unsigned int i)
{
	while(i--);
}
void showTitle()
{
	unsigned char offset=10;
	unsigned char line=0;
	/*病房叫号系统*/
	OLED_ShowCHinese(offset+0,line,0);
	OLED_ShowCHinese(offset+18,line,1);
	OLED_ShowCHinese(offset+36,line,2);
	OLED_ShowCHinese(offset+54,line,3);
	OLED_ShowCHinese(offset+72,line,4);
	OLED_ShowCHinese(offset+90,line,5);
	
	/*名称*/
	offset=0;
	line=2;
	OLED_ShowCHinese(offset+10,line,13);
	OLED_ShowCHinese(offset+28,line,14);

	/*连接*/
	offset=0;
	line=2;
	OLED_ShowCHinese(offset+54,line,7);
	OLED_ShowCHinese(offset+72,line,8);
	
	/*呼叫*/
	offset=0;
	line=2;
	OLED_ShowCHinese(offset+96,line,2);
	OLED_ShowCHinese(offset+112,line,3);
	
	/*num号床*/
	line=4;
	offset=0;
	OLED_ShowNum(offset+0,line,1,1,16);
	OLED_ShowCHinese(offset+18,line,11);
	OLED_ShowCHinese(offset+36,line,12);
	
	/*num号床*/
	line=6;
	offset=0;
	OLED_ShowNum(offset+0,line,2,1,16);
	OLED_ShowCHinese(offset+18,line,11);
	OLED_ShowCHinese(offset+36,line,12);
}
void ShowOline(unsigned char i)
{
	unsigned char offset=62;
	unsigned char line=0;
	if(i==1){
		line=4;
		OLED_ShowCHinese(offset,line,15);
	}
	else if(i==2){
		line=6;
		OLED_ShowCHinese(offset,line,15);
	}
}
void ShowDisOline(unsigned char i)
{
	unsigned char offset=62;
	unsigned char line=0;
	if(i==1){
		line=4;
		OLED_ShowCHinese(offset,line,16);
	}
	else if(i==2){
		line=6;
		OLED_ShowCHinese(offset,line,16);
	}
}
void setBlack(unsigned char i)
{
	unsigned char offset=104;
	unsigned char line=0;
	if(i==1){
		line=4;
		OLED_ShowCHinese(offset,line,17);
	}
	else if(i==2){
		line=6;
		OLED_ShowCHinese(offset,line,17);
	}
}
void clearBlack(unsigned char i)
{
	unsigned char offset=104;
	unsigned char line=0;
	if(i==1){
		line=4;
		OLED_ShowCHinese(offset,line,18);
	}
	else if(i==2){
		line=6;
		OLED_ShowCHinese(offset,line,18);
	}
}

void uartDel(unsigned char dat)
{
	if(dat==0x10)//1床号码
	{
		ShowOline(1);//显示在线，打钩
		onlineStatus[0]='1';
	}else if(dat==0x20)//2床号码
	{
		ShowOline(2);//显示在线，打钩
		onlineStatus[1]='1';
	}
	else if(dat==0x11)
	{
		ShowOline(1);//显示在线，打钩
		onlineStatus[0]='1';
		setBlack(1);
		LED=0;
		BEEP=0;
	}else if(dat==0x21)
	{
		ShowOline(2);//显示在线，打钩
		onlineStatus[1]='1';
		setBlack(2);
		LED=0;
		BEEP=0;
	}

}
void main(void)
{
    InitUART();
	OLED_Init();//初始化OLED  
	OLED_Clear(); 
	showTitle();
	ShowDisOline(1);
	ShowDisOline(2);
	while(1)
	{
		countTime++;
		if(countTime==50000)//大概5秒
		{
			countTime=0;
			if(onlineStatus[0]=='0')
			{
				ShowDisOline(1);
			}
			if(onlineStatus[1]=='0')
			{
				ShowDisOline(2);
			}
			onlineStatus[0]='0';
			onlineStatus[1]='0';
			
		}
		if(KEY==0)
		{
			delay(100);
			if(KEY==0)
			{
				LED=1;
				BEEP=1;
				clearBlack(1);
				clearBlack(2);
				SendOneByte(0xf0);
				SendString("\r\n",2);
				delay(10000);
				SendOneByte(0xf0);
				SendString("\r\n",2);
				delay(10000);
				SendOneByte(0xf0);
				SendString("\r\n",2);
				delay(10000);
				SendOneByte(0xf0);
				SendString("\r\n",2);
			}
			while(KEY==0);
		}
	}
}
