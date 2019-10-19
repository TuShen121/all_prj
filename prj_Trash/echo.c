#include "echo.h"
#include<intrins.h>	
void  StartModule() 		         //启动模块
{
	TRIG=1;			                     //启动一次模块
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	TRIG=0;
}
unsigned long Conut(void)
{
	unsigned int time;
	unsigned long S=0;
	time=TH1*256+TL1;
	TH1=0;
	TL1=0;

	S=(time*1.7)/100;     //算出来是CM
	return S;
}
unsigned long getEcho(void)
{
	TMOD|=0x10;		   //设T0为方式1，GATE=1；
	TH1=0;
	TL1=0;          
	//ET1=1;             //允许T0中断
	EA=1;			   //开启总中断	
	StartModule();
	while(!ECHO);		//当RX为零时等待
	TR1=1;			    //开启计数
	while(ECHO);			//当RX为1计数并等待
	TR1=0;				//关闭计数
	return Conut();
}