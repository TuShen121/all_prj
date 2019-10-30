#ifndef _UART_H_
#define _UART_H_

#define USART_REC_LEN  			100  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

#ifndef u16
	#define u16 unsigned int
#endif
#ifndef u8
	#define u8 unsigned char
#endif
extern xdata unsigned char  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern unsigned int USART_RX_STA;         		//接收状态标记	
extern bit E_uart_send;

void InitUART(void);
void SendOneByte(unsigned char c);
void SendString(unsigned char *p,unsigned char len);

#endif