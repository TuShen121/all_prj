
#include <STC89C52.h>

#include "uart.h"
xdata u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	
bit E_uart_send=1;       //发送使能
void InitUART(void)
{
	TMOD |= 0x20;
    SCON = 0x50;
    TH1 = 0xFA;
    TL1 = TH1;
    PCON = 0x00;
    EA = 1;
    ES = 1;
    TR1 = 1;
}
void SendOneByte(unsigned char c)
{
	if(E_uart_send){
		SBUF = c;
		while(!TI);
		TI = 0;
	}
		
}
void SendString(unsigned char *p,unsigned char len)
{
	unsigned char t;
	for(t=0;t<len;t++)
	{
		SendOneByte(p[t]);
	}
}
void UARTInterrupt(void) interrupt 4
{
	u8 Res;
    if(RI)
    {
        RI = 0;	
		Res =SBUF;	//读取接收到的数据		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else {
					USART_RX_STA|=0x8000;	//接收完成了 
					/*****************************************/
//					uartPwdDeal(USART_RX_BUF);//判断是否为密码并处理
//					ESP8266_deal(USART_RX_BUF);//WiFi状态判断
//					//SendOneByte(ESP8266_state+'0');
//			
//					USART_RX_STA=0;
					/********************************************/
				}
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)
						USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))
						USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
     
    }

}