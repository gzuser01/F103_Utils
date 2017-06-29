
#ifndef __I2C_EEPROM_TEST_H
#define	__I2C_EEPROM_TEST_H

#include "i2c_eeprom.h"
#include "usart1_util.h"
#include "timx.h"



uint8_t eeprom_read_chip_address = 0xA1;
uint8_t eeprom_write_chip_address = 0xA0;



struct I2Cx_Conf ee_i2c1_conf;


void EE_USART1_Recieve_Processor(void)
{
	int i = 0;
	uint16_t EE_Address;
	uint8_t date;
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
			EE_Address = buff[1] * 256 + buff[2];
			if(I2Cx_EE_ReadByte(&ee_i2c1_conf,eeprom_read_chip_address,EE_Address,&date))
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
			EE_Address = buff[1] * 256 + buff[2];	
			if(I2Cx_EE_WriteByte(&ee_i2c1_conf,eeprom_write_chip_address,EE_Address,buff[3]))
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
			eeprom_read_chip_address = buff[1];	
			USART1_Send_Byte(0x00);
		}
		else if(buff[0] == 0xA1)
		{
			eeprom_write_chip_address = buff[1];	
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
	
	TIM_TypeDef* usart1_callback_timx = TIM3;
	
	GPIO_TypeDef *usart1_tx_gpiox = GPIOA;
	uint16_t usart1_tx_gpio_pin = GPIO_Pin_9;

	GPIO_TypeDef *usart1_rx_gpiox = GPIOA;
	uint16_t usart1_rx_gpio_pin = GPIO_Pin_10;
	
	TIMx_With_NVIC_Config(usart1_callback_timx,7199,99,NVIC_PriorityGroup_2,1,1); 
	
	USART1_Config(usart1_tx_gpiox,usart1_tx_gpio_pin,usart1_rx_gpiox,usart1_rx_gpio_pin); //USART1 配置 
	
	I2C1_Conf_Init(&ee_i2c1_conf);
	
	Register_USART1_Callback(USART1_Received_To_Buffer_Ignore_Error);
	
	Register_TIMx_Callback(usart1_callback_timx,EE_USART1_Recieve_Processor);
	
	USART1_Data_Init();
	
}

#endif 

