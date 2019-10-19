#include "servo.h"
#include "config.h"
void InitTimer0(void)
{
    TMOD |= 0x01;
    TH0 = 0x0FF;
    TL0 = 0x0A4;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
}
void servoInit()
{
	InitTimer0();
	CCON = 0;                       //Initial PCA control register
                                    //PCA timer stop running
                                    //Clear CF flag
                                    //Clear all module interrupt flag
    CL = 0;                         //Reset PCA base timer
    CH = 0;
    CMOD = 0x04;                    //Set PCA timer clock source as Fosc/2
                                    //Disable PCA timer overflow interrupt
	//fb-0.5ms
	//e7-2.5ms
	
    CCAP0H = CCAP0L = 0xef;         //PWM0 port output 50% duty cycle square wave
    CCAPM0 = 0x42;                  //PCA module-0 work in 8-bit PWM mode and no PCA interrupt

    CR = 1;                         //PCA timer start run
	
}
void servoReturn()
{
	CCAP0H = CCAP0L = 0xf9;
}
//fb-0.5ms
//e7-2.5ms
void servoTurn()
{
	CCAP0H = CCAP0L = 0xef;
}
void Timer0Interrupt(void) interrupt 1
{
    TH0 = 0x0FF;
    TL0 = 0x0A4;
    //add your code here!
}
