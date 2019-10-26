#include "config.h"

void Io_CONFIG_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PA,PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //LED0-->PA.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
 GPIO_SetBits(GPIOC,GPIO_Pin_1);						 //PA.8 输出高

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;//PC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;//PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
}

