
#ifndef __I2C_EEPROM_TEST_H
#define	__I2C_EEPROM_TEST_H

#include "i2c_eeprom.h"
#include "usart1_util.h"
#include "timx.h"

struct I2Cx_Conf I2C1_Conf;


void USART1_Recieve_Processor(void)
{
	int i = 0;
	u16 EE_Address;
	uint8_t date;
	unsigned char buff[usart1_data_node_char_len];
	
	while(1)
	{
		i = USART1_Read_To_Buffer(buff);

		//没有收据则退出
		if(i == 0)
		{
			break;
		}
		
		
		if(buff[0] == 0x01)
		{
			EE_Address = buff[1] * 256 + buff[2];
			date = I2Cx_EE_ReadByte(&I2C1_Conf,0xA1,EE_Address);
			USART1_Send_Byte(date);
		}
		else if(buff[0] == 0x02)
		{
			EE_Address = buff[1] * 256 + buff[2];	
			I2Cx_EE_WriteByte(&I2C1_Conf,0xA0,EE_Address,buff[3]);
			USART1_Send_Byte(0x00);
		}
		else
		{
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
	
	TIMx_With_NVIC_Config(USART1_Callback_TIMx,7199,99,NVIC_PriorityGroup_0,0,0); 
	
	USART1_Config(USART1_Tx_GPIOx,USART1_Tx_GPIO_Pin,USART1_Rx_GPIOx,USART1_Rx_GPIO_Pin); //USART1 配置 
	
	I2C1_Conf_Init(&I2C1_Conf);
	
	Register_USART1_Callback(USART1_Received_To_Buffer_Ignore_Error);
	
	Register_TIMx_Callback(USART1_Callback_TIMx,USART1_Recieve_Processor);
	
	USART1_Data_Init();
	
}

#endif 

