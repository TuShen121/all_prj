//90-115

#include <STC89C52.h>
#include "uart.h"
#include "config.h"
#include "stdio.h"
unsigned int count_motto1=0;//速度格子计数
unsigned int count_motto2=0;
unsigned int mediumSpeed=0;
unsigned char moto_1=0;//占空比
unsigned char moto_2=0;

unsigned char fangXiang=1;
void delay(unsigned int i)
{
	while(i--);
}
void Int0Init()
{
	//设置INT0
	IT0=1;//跳变沿出发方式（下降沿）
	EX0=1;//打开INT0的中断允许。 
	EA=1;//打开总中断 
}
void Int1Init()
{
	//设置INT1
	IT1=1;//跳变沿出发方式（下降沿）
	EX1=1;//打开INT1的中断允许。 
	EA=1;//打开总中断 
}
void InitTimer0(void)
{
    TMOD |= 0x01;//10000us
    TH0 = 0x0B8;
    TL0 = 0x00;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
}
void InitTimer2(void)//100us秒中断一次
{
	T2MOD = 0;		//初始化模式寄存器
	T2CON = 0;		//初始化控制寄存器
	TL2 = 0xFF;		//设置定时初值
	TH2 = 0xA4;		//设置定时初值
	RCAP2L = 0xFF;		//设置定时重载值
	RCAP2H = 0xA4;		//设置定时重载值
	TR2 = 1;		//定时器2开始计时	
	ET2 =1;	
}
void main()
{
	unsigned char len;
	unsigned char sdat[5];
	unsigned int time=0;
	InitUART();
	Int0Init();
	Int1Init();
	InitTimer0();
	InitTimer2();
	while(1)
	{
		time++;
		if(time==2000)
		{
			time=0;
			if(mediumSpeed<=60){
				Red_led = 0;
				gre_led = 1;
				blu_led = 1;
				
			}else if(mediumSpeed<=93){
				Red_led = 1;
				gre_led = 0;
				blu_led = 1;
			}else
			{
				Red_led = 1;
				gre_led = 1;
				blu_led = 0;
			}
			sprintf(sdat,"%d",mediumSpeed);
			SendString(sdat,3);
			SendString("\r\n",2);
		}
		
		if(USART_RX_STA&0x8000){					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度

			if(USART_RX_BUF[0]=='0')
			{
				fangXiang=1;
				
				IN2=0;
				IN4=0;
				if(USART_RX_BUF[1]=='1')
				{
					moto_1=200;
					moto_2=200;
				}else if(USART_RX_BUF[1]=='2')
				{
					moto_1=125;
					moto_2=125;
				}else if(USART_RX_BUF[1]=='3')
				{
					moto_1=10;
					moto_2=10;
				}else if(USART_RX_BUF[1]=='0')
				{
					moto_1=0;
					moto_2=0;
				}
			}else if(USART_RX_BUF[0]=='1')
			{
				fangXiang=0;
				
				IN1=0;
				IN3=0;
				if(USART_RX_BUF[1]=='1')
				{
					moto_1=200;
					moto_2=200;
				}else if(USART_RX_BUF[1]=='2')
				{
					moto_1=125;
					moto_2=125;
				}else if(USART_RX_BUF[1]=='3')
				{
					moto_1=10;
					moto_2=10;
				}
			}
			USART_RX_STA=0;
		}
	
	
	}
}
void Int0() interrupt 0  //外部中断0的中断函数
{
	count_motto1++;
}
void Int1() interrupt 2  //外部中断1的中断函数
{
	count_motto2++;
}
void Timer0Interrupt(void) interrupt 1
{
	static count =0;
    TH0 = 0x0B8;
    TL0 = 0x00;
    //add your code here!
	count++;
	if(count>=100)
	{
		count=0;
//		SendOneByte(count_motto1);
//		SendOneByte(count_motto2);
//		SendOneByte(0xff);
		mediumSpeed=(count_motto1+count_motto2)/2;
		count_motto1=0;
		count_motto2=0;
	}
	
}


void Timer1Interrupt(void) interrupt 5 using 1
{
	static unsigned char i=0;
    //add your code here!
	i++;
	if(fangXiang==1){
		if(i>=255)
		{
			i=0;
			IN1=0;
			IN3=0;
		}
		else{
			if(i==moto_1)
			{
				IN1=1;
			}
			if(i==moto_2)
			{
				IN3=1;
			}
		}
	}else if(fangXiang==0){
		if(i>=255)
		{
			i=0;
			IN2=0;
			IN4=0;
		}
		else{
			if(i==moto_1)
			{
				IN2=1;
			}
			if(i==moto_2)
			{
				IN4=1;
			}
		}
	}
}