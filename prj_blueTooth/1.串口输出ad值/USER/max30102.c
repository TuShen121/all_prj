#include "max30102.h"



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


void max30102_FIFO_Read(u8 Register_Address,u32  Word_Data[][2],u8 count)
{
	u8 i=0;
	u8 no = count;
	u8 data1, data2,data3;
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
		data3 = i2c_ReadByte();
		i2c_Ack();
		Word_Data[i][0] = ( (((u32)data1 << 16)&0X30000) | (((u16)data2 << 8)&0XFF00) | data3);  //

		
		data1 = i2c_ReadByte();	
		i2c_Ack();
		data2 = i2c_ReadByte();
		i2c_Ack();
		data3 = i2c_ReadByte();
		if(1==no)
			i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		else
			i2c_Ack();
		Word_Data[i][1] = ( (((u32)data1 << 16)&0X30000) | (((u16)data2 << 8)&0XFF00) | data3);  //

		no--;	
		i++;
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
}



#define INTERRUPT_REG  							0X00
#define INTERRUPT_REG_A_FULL  				(0X01<<7)
#define INTERRUPT_REG_PPG_RDY  				(0X01<<6)
#define INTERRUPT_REG_ALC_OVF  				(0X01<<5)  
#define INTERRUPT_REG_PROX_INT 				(0X01<<4)
#define INTERRUPT_REG_PWR_RDY  				(0X01<<0)

#define INTERRUPT_ENABLE_REG  					0X02
#define INTERRUPT_ENABLE_REG_A_FULL_EN 		(0X01<<7)
#define INTERRUPT_ENABLE_REG_PPG_RDY_EN		(0X01<<6)
#define INTERRUPT_ENABLE_REG_ALC_OVF_EN  	(0X01<<5)  
#define INTERRUPT_ENABLE_REG_PROX_INT_EN 	(0X01<<4)

#define INTERRUPT_DIE_TEMP_REG  					0X03
#define INTERRUPT_DIE_TEMP_REG_DIE_TEMP_EN 		(0X01<<1)

#define INTERRUPT_FIFO_WR_PTR_REG  					0X04
#define INTERRUPT_OVF_COUNTER_REG  					0X05
#define INTERRUPT_RD_PTR_REG  						0X06
#define INTERRUPT_FIF0_DATA_REG  						0X07

#define INTERRUPT_FIFO_CONFIG_REG  					0X08
#define INTERRUPT_FIFO_CONFIG_REG_SMP_AVE  			(0X00<<5)  //SPM_AVE[2:0] = 000 不要 样本平均
#define INTERRUPT_FIFO_CONFIG_REG_FIFO_ROLLOVER_EN (0X01<<4)  //  自动翻转fifo
#define INTERRUPT_FIFO_CONFIG_REG_FIFO_ALL_FULL		(0X0F<<0)  // READ 17 data  for one  interrupt


#define INTERRUPT_MODE_CONFIG_REG  					0X09
#define INTERRUPT_MODE_CONFIG_REG_SHDN 				(0X00<<7)  // shutdown control
#define INTERRUPT_MODE_CONFIG_REG_RESET				(0X00<<6)  // reset  control
#define INTERRUPT_MODE_CONFIG_REG_MODE				(0X03<<0)  // Spo2  mode


#define INTERRUPT_SPO2_CONFIG_REG  					0X0a
#define INTERRUPT_SPO2_CONFIG_REG_ADC_RGE 			(0X03<<5)  // SP02_ADC_RGE[1:0]=11

#ifdef SAMPLE_50
#define INTERRUPT_SPO2_CONFIG_REG_SR 				(0X00<<2)  // SP02_SR[2:0]=000     Sample Rate = 50
#else
#define INTERRUPT_SPO2_CONFIG_REG_SR 				(0X01<<2)  // SP02_SR[2:0]=001     Sample Rate = 100
#endif

#define INTERRUPT_SPO2_CONFIG_REG_LED_PW			(0X03<<0)  // SP02_LED_PW[1:0]=11

#define INTERRUPT_LED1_PA_REG  					0X0C
#define INTERRUPT_LED2_PA_REG  					0X0D


#define ONES_READ_DATA_BY_FIFO		(32-INTERRUPT_FIFO_CONFIG_REG_FIFO_ALL_FULL)  // READ NUM  data  for one  interrupt


void max30102_init()
{
	max30102_Bus_Write(INTERRUPT_ENABLE_REG, INTERRUPT_ENABLE_REG_A_FULL_EN       |
												INTERRUPT_ENABLE_REG_PPG_RDY_EN  |
												INTERRUPT_ENABLE_REG_ALC_OVF_EN  |
												INTERRUPT_ENABLE_REG_PROX_INT_EN); //all interrupt enable

	max30102_Bus_Write(INTERRUPT_DIE_TEMP_REG, INTERRUPT_DIE_TEMP_REG_DIE_TEMP_EN); //DIE_TEMP_RDY_EN

	max30102_Bus_Write(INTERRUPT_FIFO_WR_PTR_REG, 0x00);   //set FIFO write Pointer reg = 0x00 for clear it
	max30102_Bus_Write(INTERRUPT_OVF_COUNTER_REG, 0x00);	//set Over Flow Counter  reg = 0x00 for clear it
	max30102_Bus_Write(INTERRUPT_RD_PTR_REG, 0x00);	//set FIFO Read Pointer  reg = 0x00 for clear it

	max30102_Bus_Write(INTERRUPT_FIFO_CONFIG_REG,  INTERRUPT_FIFO_CONFIG_REG_SMP_AVE|
													INTERRUPT_FIFO_CONFIG_REG_FIFO_ROLLOVER_EN  |
													INTERRUPT_FIFO_CONFIG_REG_FIFO_ALL_FULL); 											

	max30102_Bus_Write(INTERRUPT_MODE_CONFIG_REG,  INTERRUPT_MODE_CONFIG_REG_SHDN |
													INTERRUPT_MODE_CONFIG_REG_RESET  |
													INTERRUPT_MODE_CONFIG_REG_MODE); 	

	max30102_Bus_Write(INTERRUPT_SPO2_CONFIG_REG,  INTERRUPT_SPO2_CONFIG_REG_ADC_RGE |
													INTERRUPT_SPO2_CONFIG_REG_SR  |
													INTERRUPT_SPO2_CONFIG_REG_LED_PW); 	

	
	max30102_Bus_Write(INTERRUPT_LED1_PA_REG, 0xe0);	
	max30102_Bus_Write(INTERRUPT_LED2_PA_REG, 0xe0);	

	
}

#if  1
 #define PYTHON_USED
unsigned long test_max30102_fun(void)
{
	static u16 temp_num=0;
	static u32 fifo_word_buff[ONES_READ_DATA_BY_FIFO][2];
	
//	while(1)
//	{
		temp_num = max30102_Bus_Read(INTERRUPT_REG);
#ifdef	PYTHON_USED
	if( INTERRUPT_REG_PPG_RDY&temp_num )
	{
		max30102_FIFO_Read(INTERRUPT_FIF0_DATA_REG,fifo_word_buff, 1); //read the hr and spo2 data form fifo 
		
//			Usart1_PutChar(0xA0);
//			Usart1_PutChar(0xE0);
		//printf("%ld\r\n",fifo_word_buff[0][0]);
		return (unsigned long)fifo_word_buff[0][0];
	}	else return 0;	
#endif
//	}
		
}
#endif
