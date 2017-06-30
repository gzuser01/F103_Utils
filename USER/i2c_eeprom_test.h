
#ifndef __I2C_EEPROM_TEST_H
#define	__I2C_EEPROM_TEST_H

#include "i2c_eeprom.h"
#include "usart1_util.h"
#include "timx.h"



uint8_t _m_Read_EE_Chip_Address = 0xA1;
uint8_t _m_Write_EE_Chip_Address = 0xA0;



struct I2Cx_Conf EE_I2C1_Conf;


void EE_USART1_Recieve_Processor(void)
{
	int i = 0;
	uint16_t sub_address;
	uint8_t date;
	unsigned char buff[USART1_Data_Node_Char_Len];
	
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
			sub_address = buff[1] * 256 + buff[2];
			if(I2Cx_EE_ReadByte(&EE_I2C1_Conf,_m_Read_EE_Chip_Address,sub_address,&date))
			{
				USART1_Send_Byte(date);
			}
			else
			{
				USART1_Send_Byte(0xFF);
				USART1_Send_Byte(0xFF);
				USART1_Send_Byte(I2Cx_EE_Error_Code());
			}
		}
		else if(buff[0] == 0x02)
		{
			sub_address = buff[1] * 256 + buff[2];	
			if(I2Cx_EE_WriteByte(&EE_I2C1_Conf,_m_Write_EE_Chip_Address,sub_address,buff[3]))
			{
				USART1_Send_Byte(0x00);
			}
			else
			{
				USART1_Send_Byte(0xFF);
				USART1_Send_Byte(0xFF);
				USART1_Send_Byte(I2Cx_EE_Error_Code());
			}
		}
		else if(buff[0] == 0xA0)
		{
			_m_Read_EE_Chip_Address = buff[1];	
			USART1_Send_Byte(0x00);
		}
		else if(buff[0] == 0xA1)
		{
			_m_Write_EE_Chip_Address = buff[1];	
			USART1_Send_Byte(0x00);
		}
		else
		{
			USART1_Send_Byte(0xEF);
			USART1_Send_Byte(0xFF);
		}
		
	}
	
	
}


void i2c_eeprom_test(void)
{
	
	TIM_TypeDef* USART1_Callback_TIMx = TIM3;
	
	GPIO_TypeDef *USART1_Tx_GPIOx = GPIOA;
	uint16_t USART1_Tx_GPIO_Pin = GPIO_Pin_9;

	GPIO_TypeDef *USART1_Rx_GPIOx = GPIOA;
	uint16_t USART1_Rx_GPIO_Pin = GPIO_Pin_10;
	
	TIMx_With_NVIC_Config(USART1_Callback_TIMx,7199,99,NVIC_PriorityGroup_2,1,1); 
	
	USART1_Config(USART1_Tx_GPIOx,USART1_Tx_GPIO_Pin,USART1_Rx_GPIOx,USART1_Rx_GPIO_Pin); //USART1 配置 
	
	I2C1_Conf_Init(&EE_I2C1_Conf);
	
	Register_USART1_Callback(USART1_Received_To_Buffer_Ignore_Error);
	
	Register_TIMx_Callback(USART1_Callback_TIMx,EE_USART1_Recieve_Processor);
	
	USART1_Data_Init();
	
}

#endif 

