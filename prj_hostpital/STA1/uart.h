#ifndef _UART_H_
#define _UART_H_


#ifndef u16
	#define u16 unsigned int
#endif
#ifndef u8
	#define u8 unsigned char
#endif

void InitUART(void);
void SendOneByte(unsigned char c);
void SendString(unsigned char *p,unsigned char len);

#endif