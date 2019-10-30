#include "STC89C5x.h"
#include "uart.h"
#include "temp.h"
#include "stdio.h"
#include "oled.h"

unsigned char DisplayData[6];
unsigned char alarmFlag=0;
unsigned strong =0;
sbit windSwitch = P0^1;//=0闭合
sbit hotSwitch = P0^0;
sbit key=P2^2;

sbit led1=P3^5;//=1亮
sbit led2=P3^6;

unsigned char tempMax=30;
void showTitle()
{
	unsigned char offset=15;
	OLED_ShowCHinese(0+offset,0,0);//大棚温度监控系统
	OLED_ShowCHinese(18+offset,0,1);//
	OLED_ShowCHinese(36+offset,0,2);//
	OLED_ShowCHinese(54+offset,0,3);//
	OLED_ShowCHinese(72+offset,0,4);//
	OLED_ShowCHinese(90+offset,0,5);//
	
	offset=0;
	OLED_ShowCHinese(0+offset,2,2);//温度
	OLED_ShowCHinese(18+offset,2,3);//
	
	OLED_ShowChar(90,2,'#');//箭头
	
	offset=0;
	OLED_ShowCHinese(0+offset,4,8);//制冷
	OLED_ShowCHinese(18+offset,4,9);//
	OLED_ShowChar(40+offset,4,'X');////制冷显示
	
	offset=0;
	OLED_ShowCHinese(0+offset,6,10);//加热
	OLED_ShowCHinese(18+offset,6,11);//
	OLED_ShowChar(40+offset,6,'X');//加热显示
}
void datapros(int temp) 	 
{
	
   	float tp;  
	if(temp< 0)				//当温度值为负数
  	{
		DisplayData[0] = 0x40; 	  //   -
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
 
  	}
 	else
  	{			
		DisplayData[0] = 0x00;
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
	
		if(temp/100>tempMax+1)//温度超过阈值1度
		{
			alarmFlag=1;
			if(strong==0){
				windSwitch=0;////打开风扇
				hotSwitch=1;
				OLED_ShowChar(40+0,4,'Y');////制冷显示
				OLED_ShowChar(40+0,6,'X');//加热显示
			}
		}else if(temp/100<tempMax-1)//温度低于阈值1度
		{
			alarmFlag=0;
			if(strong==0){
				windSwitch=1;
				hotSwitch=0;//打开制热
				OLED_ShowChar(40+0,4,'X');////制冷显示
				OLED_ShowChar(40+0,6,'Y');//加热显示
			}
		}else//正常
		{
			alarmFlag=3;
			if(strong==0){
				windSwitch=1;//两个都关闭
				hotSwitch=1;
				OLED_ShowChar(40+0,4,'X');////制冷显示
				OLED_ShowChar(40+0,6,'X');//加热显示
			}
		}
	
	sprintf(DisplayData,"%.2f",temp*0.01);//在屏幕上显示温度信息
	OLED_ShowString(36,2,DisplayData);
}

void showTempMax()
{
	OLED_ShowNum(100,2,tempMax,2,16);
	
}
void delay(unsigned int i)
{
	while(i--);
}
void main()
{
	//u8 len;
	unsigned int time;
	unsigned char timeSend=0;
	InitUART();
	OLED_Init();
	showTitle();
	delay(65530);//延时一会儿
	SendString("m1\r\n",4);//开始联网
	while(1)
	{
		
		datapros(Ds18b20ReadTemp());	 //温度接收和显示
		showTempMax();//在屏幕上显示温度阈值
		if(USART_RX_STA&0x8000)
		{			
		
			//len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度USART_RX_BUF[t];
			if(USART_RX_BUF[0]=='1')//打开风扇
			{
				strong=1;
				windSwitch=0;//闭合
				hotSwitch=1;
				OLED_ShowChar(40+0,4,'Y');////制冷显示
				OLED_ShowChar(40+0,6,'X');//加热显示
			}
			else if(USART_RX_BUF[0]=='2')//打开加热
			{
				strong=1;
				windSwitch=1;
				hotSwitch=0;//闭合
				OLED_ShowChar(40+0,4,'X');////制冷显示
				OLED_ShowChar(40+0,6,'Y');//加热显示
			}
			else if(USART_RX_BUF[0]=='3')//二者关闭
			{
				strong=1;
				windSwitch=1;
				hotSwitch=1;
				OLED_ShowChar(40+0,4,'X');////制冷显示
				OLED_ShowChar(40+0,6,'X');//加热显示
			}
			else if(USART_RX_BUF[0]=='4')//关闭强制，让单片机自动选择
			{
				strong=0;
//				windSwitch=1;
//				hotSwitch=1;
//				OLED_ShowChar(40+0,4,'X');////制冷显示
//				OLED_ShowChar(40+0,6,'X');//加热显示
			}
			if(USART_RX_BUF[1]>='0' && USART_RX_BUF[1]<='9' )//收到温度阈值
			{
				tempMax=USART_RX_BUF[1]-0x30;
			}
			if(USART_RX_BUF[2]>='0' && USART_RX_BUF[2]<='9' )
			{
				tempMax=tempMax*10+USART_RX_BUF[2]-0x30;
			}
			USART_RX_STA=0;
		}
		
		if(key==0){
			time++;
			if(time>100)
			{
				SendString("m0\r\n",4);//开始配网
				time=0;
				led2=1;
			}
		}else
		{
			time=0;
			led2=0;
		}
		timeSend++;
		if(timeSend>=20)
		{
			timeSend=0;
			SendOneByte((char)windSwitch+0x30);//发送风扇开关信息1未开启
			SendOneByte(',');
			SendOneByte((char)hotSwitch+0x30);//发送加热开关信息1未开启
			SendOneByte(',');
			SendOneByte(alarmFlag+0x30);//发送报警信息1高0低3正常
			SendOneByte(',');
			SendString(DisplayData,5);//发送温度信息
			SendOneByte(',');
			if(tempMax>10)//温度阈值信息
			{
				SendOneByte(tempMax/10+0x30);
				SendOneByte(tempMax%10+0x30);
			}
			else
			{
				SendOneByte(tempMax+0x30);
			}
			SendOneByte(',');
			SendOneByte(strong+0x30);//是否强制，1是
			SendOneByte(',');
			SendString("\r\n",2);
		}
	}
}