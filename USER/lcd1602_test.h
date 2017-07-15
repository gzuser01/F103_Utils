#ifndef __LCD1602_TEST_H
#define	__LCD1602_TEST_H

#include "i2c.h"
#include "timx.h"
#include "usart1_data.h"

struct I2Cx_Conf LCD1602_I2C1_Conf;

uint8_t _m_LCD1602_Chip_Address = 0x7F;

int8_t _m_LCD1602_I2C_Error_Code;


uint8_t LCD1602_Write(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t chip_address, uint8_t data)
{
	_m_LCD1602_I2C_Error_Code = 0x00;
	
	I2C1_Conf_Init(I2Cx_Conf_Struct);
	
	I2Cx_Ack_Enable(I2Cx_Conf_Struct);
	
	if(!I2Cx_Start(I2Cx_Conf_Struct))
	{
		_m_LCD1602_I2C_Error_Code = 0x12;
		return 0;
	}
	
	if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,chip_address, I2C_Direction_Transmitter))
	{
		_m_LCD1602_I2C_Error_Code = 0x13;
		return 0;
	}
	
	if(!I2Cx_Transmit(I2Cx_Conf_Struct, data))
	{
		_m_LCD1602_I2C_Error_Code = 0x15;
		return 0;
	}
	
	if(!I2Cx_Stop(I2Cx_Conf_Struct))
	{
		_m_LCD1602_I2C_Error_Code = 0x16;
		return 0;
	}
	return 1;
}


void LCD1602_PulseEnable(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t _data)
{
	LCD1602_Write(I2Cx_Conf_Struct,_m_LCD1602_Chip_Address,_data | 0x04 | 0x08);	// En high
	Delay_Nus(10);		// enable pulse must be >450ns

	LCD1602_Write(I2Cx_Conf_Struct,_m_LCD1602_Chip_Address,_data & ~0x04 | 0x08);	// En low
	Delay_Nus(50);		// commands need > 37us to settle
}

void LCD1602_Write4bits(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t value) 
{
	//LCD1602_Write(I2Cx_Conf_Struct,value);
	LCD1602_PulseEnable(I2Cx_Conf_Struct,value);
}


void LCD1602_Send(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t value, uint8_t mode)
{
	uint8_t highnib = value&0xf0;
	uint8_t lownib = (value<<4)&0xf0;
  LCD1602_Write4bits(I2Cx_Conf_Struct,(highnib)|mode);
	LCD1602_Write4bits(I2Cx_Conf_Struct,(lownib)|mode);
}

void LCD1602_Send_Char(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t c) 
{
	LCD1602_Send(I2Cx_Conf_Struct,c,0x01);
}

void LCD1602_Command(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t cmd)
{
	LCD1602_Send(I2Cx_Conf_Struct,cmd,0x00);
}
	


void LCD1602_SetCursor(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t col, uint8_t row){
	uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > 4 ) {
		row = 4-1;
	}
	LCD1602_Command(I2Cx_Conf_Struct,0x80 | (col + row_offsets[row]));
}

void LCD1602_USART1_Recieve_Processor(void)
{
	int i = 0;
	unsigned char buff[USART1_Data_Node_Char_Len];
	
	while(1)
	{
		i = USART1_Read_To_Buffer(buff);

		//没有数据则退出
		if(i == 0)
		{
			break;
		}
		
		
		
	}
	
	
}

void LCD1602_Flash(void)
{
	uint8_t i;
	for(i = 0;i<3;i++)
	{
		Delay_Nms(500);
		LCD1602_Write(&LCD1602_I2C1_Conf,_m_LCD1602_Chip_Address,0x00);
		if(_m_LCD1602_I2C_Error_Code != 0x00)
		{
			USART1_Send_Byte(0xEF);
			USART1_Send_Byte(0x08);
			USART1_Send_Byte(_m_LCD1602_I2C_Error_Code);
		}
		Delay_Nms(500);
		LCD1602_Write(&LCD1602_I2C1_Conf,_m_LCD1602_Chip_Address,0x08);
		if(_m_LCD1602_I2C_Error_Code != 0x00)
		{
			USART1_Send_Byte(0xEF);
			USART1_Send_Byte(0x08);
			USART1_Send_Byte(_m_LCD1602_I2C_Error_Code);
		}
	}
}

/*
void LCD1602_Write_Char(uint8_t c)
{
	LCD1602_Write(&LCD1602_I2C1_Conf,_m_LCD1602_Chip_Address,0x01 | 0x08,(c&0xf0) | 0x08);
	LCD1602_Write(&LCD1602_I2C1_Conf,_m_LCD1602_Chip_Address,0x01 | 0x08,((c<<4)&0xf0) | 0x08);
	
	
	if(_m_LCD1602_I2C_Error_Code != 0x00)
	{
		USART1_Send_Byte(0xEF);
		USART1_Send_Byte(0x01);
		USART1_Send_Byte(_m_LCD1602_I2C_Error_Code);
	}
}
*/
void lcd1602_test(void)
{

	TIM_TypeDef* USART1_Callback_TIMx = TIM3;
	
	GPIO_TypeDef *USART1_Tx_GPIOx = GPIOA;
	uint16_t USART1_Tx_GPIO_Pin = GPIO_Pin_9;

	GPIO_TypeDef *USART1_Rx_GPIOx = GPIOA;
	uint16_t USART1_Rx_GPIO_Pin = GPIO_Pin_10;
	
	TIMx_With_NVIC_Config(USART1_Callback_TIMx,7199,99,NVIC_PriorityGroup_2,1,1); 
	
	USART1_Config(USART1_Tx_GPIOx,USART1_Tx_GPIO_Pin,USART1_Rx_GPIOx,USART1_Rx_GPIO_Pin); //USART1 配置 
	
	Register_USART1_Callback(USART1_Received_To_Buffer_Ignore_Error);
	
	Register_TIMx_Callback(USART1_Callback_TIMx,LCD1602_USART1_Recieve_Processor);
	
	USART1_Data_Init();
	

	
	I2C1_Conf_Init(&LCD1602_I2C1_Conf);
	USART1_Send_Byte(0xFF);
	USART1_Send_Byte(0xFF);
	

	LCD1602_Flash();
	
	
	LCD1602_Command(&LCD1602_I2C1_Conf,0x01);
	LCD1602_Command(&LCD1602_I2C1_Conf,0x02);
	

	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'a');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'b');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'0');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'1');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'2');
	
	Delay_Nms(1000);
	LCD1602_Command(&LCD1602_I2C1_Conf,0x01);
	LCD1602_Command(&LCD1602_I2C1_Conf,0x02);
	
	LCD1602_SetCursor(&LCD1602_I2C1_Conf,0,0);
	
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'x');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'y');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'z');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,':');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'+');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'=');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'0');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'1');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'2');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'3');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'4');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'5');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'6');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'7');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'8');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'9');
	
	LCD1602_SetCursor(&LCD1602_I2C1_Conf,0,1);


	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'a');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'b');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'c');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'d');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'e');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'f');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'0');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'1');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'2');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'3');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'4');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'5');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'6');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'7');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'8');
	LCD1602_Send_Char(&LCD1602_I2C1_Conf,'9');


}



#endif 


