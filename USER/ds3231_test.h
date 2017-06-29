#ifndef __DS3231_TEST_H
#define	__DS3231_TEST_H

#include "i2c.h"
#include "usart1_util.h"
#include "timx.h"



uint8_t ds3231_read_chip_address = 0xD0;
uint8_t ds3231_write_chip_address = 0xD0;

int8_t i2cx_ds3231_error_code;

struct I2Cx_Conf ds3231_i2c1_conf;


uint8_t DS3231_Init(uint8_t chip_address)
{
	uint16_t i;
	
	i2cx_ds3231_error_code = 0x00;
	
	while (I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)); // Wait until I2C free
	
	I2C_GenerateSTART(I2C1,ENABLE);
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT);i++) // Wait for EV5
	{
		if(i> TRY_TIMES)
		{
			i2cx_ds3231_error_code = 0xA0;
			return 0;
		}
	}
	
	I2C_Send7bitAddress(I2C1,chip_address,I2C_Direction_Transmitter); // Send DS3231 slave address
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);i++) // Wait for EV6
	{
		if(i> TRY_TIMES)
		{
			i2cx_ds3231_error_code = 0xA1;
			return 0;
		}
	}
	
	I2C_SendData(I2C1,0x0E); // Send DS3231 control register address
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);i++) // Wait for EV8
	{
		if(i> TRY_TIMES)
		{
			i2cx_ds3231_error_code = 0xA2;
			return 0;
		}
	}
	
	I2C_SendData(I2C1,0x00); // DS3231 EOSC enabled, INTCN enabled, SQW set to 1Hz
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);i++) // Wait for EV8
	{
		if(i> TRY_TIMES)
		{
			i2cx_ds3231_error_code = 0xA3;
			return 0;
		}
	}
	
	
	I2C_SendData(I2C1,0x00); // DS3231 clear alarm flags
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);i++) // Wait for EV8
	{
		if(i> TRY_TIMES)
		{
			i2cx_ds3231_error_code = 0xA4;
			return 0;
		}
	}
		
	I2C_GenerateSTOP(I2C1,ENABLE);
	return 1;
	
}



uint8_t DS3231_ReadByte(struct I2Cx_Conf *i2cx_conf,uint8_t chip_address, uint8_t sub_address,uint8_t* data)
{
	i2cx_ds3231_error_code = 0x00;
	
	I2C1_Conf_Init(&ds3231_i2c1_conf);

	I2Cx_Ack_Enable(i2cx_conf);
	
	if(!I2Cx_Wait(i2cx_conf))
	{
		i2cx_ds3231_error_code = 0x01;
		//return 0;
	}
	if(!I2Cx_Start(i2cx_conf))
	{
		i2cx_ds3231_error_code = 0x02;
		//return 0;
	}
	if(!I2Cx_Address_Direction(i2cx_conf,chip_address, I2C_Direction_Transmitter))
	{
		i2cx_ds3231_error_code = 0x03;
		return 0;
	}
	
	if(!I2Cx_Transmit(i2cx_conf, sub_address))
	{
		i2cx_ds3231_error_code = 0x04;
		return 0;
	}
	if(!I2Cx_Start(i2cx_conf))
	{
		i2cx_ds3231_error_code = 0x05;
		return 0;
	}
	if(!I2Cx_Address_Direction(i2cx_conf,chip_address, I2C_Direction_Receiver))
	{
		i2cx_ds3231_error_code = 0x06;
		return 0;
	}
	if(!I2Cx_Receives(i2cx_conf,data,7))
	{
		i2cx_ds3231_error_code = 0x07;
		return 0;
	}

	
	I2Cx_Ack_Disable(i2cx_conf);
		
	if(!I2Cx_Stop(i2cx_conf))
	{
		i2cx_ds3231_error_code = 0x09;
		return 0;
	}

	return 1;
	
}


uint8_t DS3231_WriteByte(struct I2Cx_Conf *i2cx_conf,uint8_t chip_address, uint8_t sub_address, uint8_t data)
{
	i2cx_ds3231_error_code = 0x00;
	if(!I2Cx_Wait(i2cx_conf))
	{
		i2cx_ds3231_error_code = 0x11;
		return 0;
	}
	if(!I2Cx_Start(i2cx_conf))
	{
		i2cx_ds3231_error_code = 0x12;
		return 0;
	}
	if(!I2Cx_Address_Direction(i2cx_conf,chip_address, I2C_Direction_Transmitter))
	{
		i2cx_ds3231_error_code = 0x13;
		return 0;
	}
	if(!I2Cx_Transmit(i2cx_conf, sub_address))
	{
		i2cx_ds3231_error_code = 0x14;
		return 0;
	}
	if(!I2Cx_Transmit(i2cx_conf, data))
	{
		i2cx_ds3231_error_code = 0x15;
		return 0;
	}
	if(!I2Cx_Stop(i2cx_conf))
	{
		i2cx_ds3231_error_code = 0x16;
		return 0;
	}
	return 1;
}


int8_t I2Cx_DS3231_Error_Code(void)
{
	return i2cx_ds3231_error_code;
}




void DS3231_USART1_Recieve_Processor(void)
{
	int i = 0;
	int j = 0;
	uint8_t ds3231_address;
	uint8_t data[7] = {0,0,0,0,0,0,0};
	unsigned char buff[usart1_data_node_char_len];
	
	while(1)
	{
		i = USART1_Read_To_Buffer(buff);

		//没有数据则退出
		if(i == 0)
		{
			break;
		}
		
		if(buff[0] == 0x01)
		{
			ds3231_address = buff[1];
			if(DS3231_ReadByte(&ds3231_i2c1_conf,ds3231_read_chip_address,ds3231_address,data))
			{
				for(j=0;j<7;j++)
				{
					USART1_Send_Byte(data[j]);
				}
			}
			if(I2Cx_DS3231_Error_Code() != 0x00)
			{
				USART1_Send_Byte(0xFF);
				USART1_Send_Byte(0xFF);
				USART1_Send_Byte(I2Cx_DS3231_Error_Code());
			}
		}
		else if(buff[0] == 0x02)
		{
			ds3231_address = buff[1];
			if(DS3231_WriteByte(&ds3231_i2c1_conf,ds3231_write_chip_address,ds3231_address,buff[2]))
			{
				USART1_Send_Byte(0x00);
			}
			if(I2Cx_DS3231_Error_Code() != 0x00)
			{
				USART1_Send_Byte(0xFF);
				USART1_Send_Byte(0xFF);
				USART1_Send_Byte(I2Cx_DS3231_Error_Code());
			}
		}
		else
		{
			USART1_Send_Byte(0xFF);
		}
		
	}
	
	
}


void ds3231_test(void)
{
	
	TIM_TypeDef* usart1_callback_timx = TIM3;
	
	GPIO_TypeDef *usart1_tx_gpiox = GPIOA;
	uint16_t usart1_tx_gpio_pin = GPIO_Pin_9;

	GPIO_TypeDef *usart1_rx_gpiox = GPIOA;
	uint16_t usart1_rx_gpio_pin = GPIO_Pin_10;
	
	TIMx_With_NVIC_Config(usart1_callback_timx,7199,99,NVIC_PriorityGroup_0,0,0); 
	
	USART1_Config(usart1_tx_gpiox,usart1_tx_gpio_pin,usart1_rx_gpiox,usart1_rx_gpio_pin); //USART1 配置 
	
	I2C1_Conf_Init(&ds3231_i2c1_conf);
	
	Register_USART1_Callback(USART1_Received_To_Buffer_Ignore_Error);
	
	Register_TIMx_Callback(usart1_callback_timx,DS3231_USART1_Recieve_Processor);
	
	USART1_Data_Init();
	
	DS3231_Init(ds3231_read_chip_address);
	if(I2Cx_DS3231_Error_Code() != 0x00)
	{
		USART1_Send_Byte(0xFF);
		USART1_Send_Byte(0xFF);
		USART1_Send_Byte(I2Cx_DS3231_Error_Code());
	}
	else
	{
		USART1_Send_Byte(0xFF);
		USART1_Send_Byte(0xFF);
	}
	
	
	
	
}





#endif 
