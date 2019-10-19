#include "ds18b20.h"
//温度显示程序==LED显示，精度0.1摄氏度
//晶振:12M
//单片机：stc12c5a60s2   1T
/******************************************
跳线设置:默认
注意事项:ds18b20切勿插反，有爆炸烫伤的危险，方向是ds18b20的平面（有字的一面）朝旁边的三极管Q4
***/
#include <reg51.h>
#include <intrins.h>
#include "config.h"
#define uchar unsigned char
#define uint unsigned int
unsigned char delayFlag=0;
unsigned char i=2;
//数据口define interface



uint wendu;                //温度值 variable of wenduerature

//不带小数点
unsigned char code table[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
         0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//带小数点
unsigned char code table1[] = {0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};


/*************精确延时函数**************** */
//                          数据表如下
/*
********************************************************************************************************************
延时时间      a的值     b的值       c的值     延时误差（us）
10us          1           1           1            -0.5    
20us          1           1           8            0  
30us          1           1           15           +0.5 
40us          2           1           9            0  
50us          1           1           28            0  
60us          1           1           35           +0.5 
70us          1           1           42           +1 
80us          1           1           48            0 
90us          1           1           55           +0.5  
100us         1           1           61           -0.5  
200us         1           1           128           0  
300us         3           1           63           +1.5  
400us         2           1           129           0  
500us         5           1           63           +0.5  
600us         6           1           63            0  
700us         7           1           63            -0.5  
800us         1           3           175           +0.5  
900us         9           1           63            -1.5  
1ms           1           3           219           -1.5
2ms           2           3           220           +3
3ms           3           3           220           +3
Xms           X           3           220           +3     
       (X的范围为2到255)
*/
void time0Start(unsigned char th0,unsigned char tl0)
{
    TMOD |= 0x01;
     TH0 = th0;
    TL0 = tl0;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
	delayFlag=0;
}
void Timer0Interrupt(void) interrupt 1
{
     TH0 = 0x0FF;
    TL0 = 0x0EE;
	delayFlag=1;
	TR0 = 0;
    //add your code here!
}
void Delay(unsigned char th0,unsigned char tl0)
{
	time0Start(th0,tl0);
	while(delayFlag==0);
}

/*****************DS18B20******************/ 
void Init_Ds18b20(void)     //DS18B20初始化send reset and initialization command
{
DQ = 1;                     //DQ复位,不要也可行。
Delay(0xff,0xee-i);                  //稍做延时  10us
DQ = 0;                    //单片机拉低总线
Delay(0xfb,0xae-i); //600  us      //精确延时，维持至少480us
//Delay(1,1,15); //20us
DQ = 1;                    //释放总线，即拉高了总线
Delay(0xfc,0x66-i); //500us               //此处延时有足够,确保能让DS18B20发出存在脉冲。
}

uchar Read_One_Byte()       //读取一个字节的数据read a byte date
                            //读数据时,数据以字节的最低有效位先从总线移出
{
uchar i   = 0;
uchar dat = 0;
for(i=8;i>0;i--)
   {
   DQ = 0;                  //将总线拉低，要在1us之后释放总线
                           //单片机要在此下降沿后的15us内读数据才会有效。
   _nop_();                 //至少维持了1us,表示读时序开始
   dat >>= 1;               //让从总线上读到的位数据，依次从高位移动到低位。
   DQ = 1;                  //释放总线，此后DS18B20会控制总线,把数据传输到总线上
   Delay(0xff,0xee-i);        //延时10us,此处参照推荐的读时序图，尽量把控制器采样时间放到读时序后的15us内的最后部分
   if(DQ)                   //控制器进行采样
   {
    dat |= 0x80;            //若总线为1,即DQ为1,那就把dat的最高位置1;若为0,则不进行处理,保持为0
   }        
   Delay(0xff,0xdb-i);       //20us        //此延时不能少，确保读时序的长度60us。
}
return (dat);
}

void Write_One_Byte(uchar dat)
{
uchar i = 0;
for(i=8;i>0;i--)
{
   DQ = 0;                        //拉低总线
   _nop_();                       //至少维持了1us,表示写时序(包括写0时序或写1时序)开始
   DQ = dat&0x01;                 //从字节的最低位开始传输
                                 //指令dat的最低位赋予给总线,必须在拉低总线后的15us内,
                                 //因为15us后DS18B20会对总线采样。
   Delay(0xff,0x91-i);               //必须让写时序持续至少60us
	
	DQ = 1;                        //写完后,必须释放总线,
   dat >>= 1;
   Delay(0xff,0x91-i);               //必须让写时序持续至少60us
}
}


uint Get_Tmp()                   //获取温度get the wenduerature
{
float tt;
uchar a,b;
Init_Ds18b20();                //初始化
Write_One_Byte(0xcc);          //忽略ROM指令
Write_One_Byte(0x44);          //温度转换指令
Init_Ds18b20();                 //初始化
Write_One_Byte(0xcc);          //忽略ROM指令
Write_One_Byte(0xbe);          //读暂存器指令
a = Read_One_Byte();           //读取到的第一个字节为温度LSB
b = Read_One_Byte();           //读取到的第一个字节为温度MSB
wendu = b;                      //先把高八位有效数据赋于wendu
wendu <<= 8;                    //把以上8位数据从wendu低八位移到高八位
wendu = wendu|a;                //两字节合成一个整型变量
tt = wendu*0.0625;              //得到真实十进制温度值
                                //因为DS18B20可以精确到0.0625度
                                //所以读回数据的最低位代表的是0.0625度
wendu = tt*10+0.5;               //放大十倍
                                //这样做的目的将小数点后第一位也转换为可显示数字
                                //同时进行一个四舍五入操作。
return wendu;
}



