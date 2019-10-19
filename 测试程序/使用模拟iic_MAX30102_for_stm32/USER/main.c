#include "stm32f10x.h"
#include "usart.h"
//#include "ultrasonic.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "delay.h"//延时函数

#include <stdio.h>
#include <math.h>
#include "bsp_i2c_gpio.h"


/*************************************************
函数: fputc(int ch, FILE *f)
功能: 重定向c库函数printf到USART1
参数: 无
返回: 无
**************************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	return (ch);
}




/*************************************************
函数: void main_init(void)
功能: main初始化
参数: 无
返回: 无
**************************************************/
void main_init(void)
{
	Usart_Init();
	//I2C1_GPIO_Config();
	//I2C1_Mode_config();
	//I2C1_Configuration();
	bsp_InitI2C();
	delay_init(72);            //延时初始化
}

extern u8 max30102_Bus_Read(u8 Register_Address);
extern void max30102_init(void);

/*************************************************
函数: int main(void)
功能: main主函数
参数: 无
返回: 无
**************************************************/
int main(void)
{
	u8 temp_num=0;
	main_init();
	
	max30102_init();
	
	printf("\r\n MAX30102  init  \r\n");

	
	while(1)
	{
		delay_ms(1000);            
		max30102_init();
		temp_num = max30102_Bus_Read(0x1f);
		printf("当前温度 = %d\r\n",temp_num);
	}

}


#define max30102_WR_address 0xAE

u8 max30102_Bus_Write(u8 Register_Address, u8 Word_Data)
{

	/* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */

	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址 */
	i2c_SendByte(Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第5步：开始写入数据 */
	i2c_SendByte(Word_Data);

	/* 第6步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}



u8 max30102_Bus_Read(u8 Register_Address)
{
	u8  data;


	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址， */
	i2c_SendByte((uint8_t)Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	i2c_Start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第9步：读取数据 */
	{
		data = i2c_ReadByte();	/* 读1个字节 */

		i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return data;	/* 执行成功 返回data值 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}


void max30102_FIFO_Read(u8 Register_Address,u16  Word_Data[][2],u8 count)
{
	u8 i=0;
	u8 no = count;
	u8 data1, data2;
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址， */
	i2c_SendByte((uint8_t)Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	i2c_Start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第9步：读取数据 */
	while (no)
	{
		data1 = i2c_ReadByte();	
		i2c_Ack();
		data2 = i2c_ReadByte();
		i2c_Ack();
		Word_Data[i][0] = (((u16)data1 << 8) | data2);  //

		
		data1 = i2c_ReadByte();	
		i2c_Ack();
		data2 = i2c_ReadByte();
		if(1==no)
			i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		else
			i2c_Ack();
		Word_Data[i][1] = (((u16)data1 << 8) | data2); 

		no--;	
		i++;
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
}

#define INTERRUPT_REG  					0X00


void max30102_init()
{
	
	max30102_Bus_Write(0x09, 0x0b);  //mode configuration : temp_en[3]      MODE[2:0]=010 HR only enabled    011 SP02 enabled
	max30102_Bus_Write(0x01, 0xF0); //open all of interrupt
	max30102_Bus_Write(INTERRUPT_REG, 0x00); //all interrupt clear
	max30102_Bus_Write(0x03, 0x02); //DIE_TEMP_RDY_EN
	max30102_Bus_Write(0x21, 0x01); //SET   TEMP_EN

	max30102_Bus_Write(0x0a, 0x47); //SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS

	max30102_Bus_Write(0x0c, 0x47); 
	max30102_Bus_Write(0x0d, 0x47); 
											
}

