#ifndef __DS3231_WITH_LCD1602_TEST_H
#define	__DS3231_WITH_LCD1602_TEST_H


#include "ds3231_test.h"
#include "lcd1602_test.h"


//uint8_t last_second = 0;
//uint8_t ms = 0;
uint8_t delay = 0;

void display_time(void)
{
	int i;
	char c[2];
	uint8_t data[7] = {0,0,0,0,0,0,0};
	
	if(delay < 10)
	{
		delay ++;
		return;
	}
	delay = 0;
	
	//
	if(DS3231_Read(&DS3231_I2C1_Conf,_m_DS3231_Chip_Address,_m_DS3231_Sub_Address,data))
	{
		for(i=0;i<7;i++)
		{
			data[i] = bcd_to_dec(data[i]);
			USART_Send_Byte(USART1,data[i]);					
		}
		USART_Send_Byte(USART1,'\n');
		
		//
		//LCD1602_Command(&LCD1602_I2C_Conf,0x01);
		//LCD1602_Command(&LCD1602_I2C_Conf,0x02);
		//
		LCD1602_SetCursor(&LCD1602_I2C_Conf,0,0);
		//
		sprintf(c, "%2d", data[6]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,'2');
		LCD1602_Send_Char(&LCD1602_I2C_Conf,'0');
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,'-');
		//
		sprintf(c, "%2d", data[5]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,'-');
		//
		sprintf(c, "%2d", data[4]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,' ');
		LCD1602_Send_Char(&LCD1602_I2C_Conf,' ');
		//
		LCD1602_SetCursor(&LCD1602_I2C_Conf,0,1);
		//
		sprintf(c, "%2d", data[2]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,':');
		//
		sprintf(c, "%2d", data[1]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,':');
		//
		sprintf(c, "%2d", data[0]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
		
		//
		/*
		if(last_second == data[0])
		{
			ms ++;
		}
		else
		{
			last_second = data[0];
			ms = 0;
		}
		sprintf(c, "%2d", ms);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,'.');
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
		LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
		*/
		
		LCD1602_Send_Char(&LCD1602_I2C_Conf,' ');
		LCD1602_Send_Char(&LCD1602_I2C_Conf,' ');
		
	}
}

void ds3231_with_lcd1602_test(void)
{

	
	
	TIMx_With_NVIC_Config(_m_DS3231_USART_Callback_TIMx,7199,99,NVIC_PriorityGroup_0,0,0); 
	
	USART_Config(USART1,_m_DS3231_USART1_BaudRate,
		_m_DS3231_USART1_Tx_GPIOx,_m_DS3231_USART1_Tx_GPIO_Pin,_m_DS3231_USART1_Rx_GPIOx,_m_DS3231_USART1_Rx_GPIO_Pin); //USART1 配置 	
	
	Register_USART_Callback(USART1,DS3231_USART_Received_Data);
	
	Register_TIMx_Callback(_m_DS3231_USART_Callback_TIMx,DS3231_USART1_Recieve_Processor);
	Register_TIMx_Callback(_m_DS3231_USART_Callback_TIMx,display_time);
	
	_m_DS3231_USART1_Eof_Len = sizeof(_m_DS3231_USART1_Data_Eof) / sizeof(_m_DS3231_USART1_Data_Eof[0]);

	USART_Data_Init(&_m_DS3231_USART1_Data,
		_m_DS3231_USART1_Data_Nodes,
		DS3231_USART1_Data_Node_Len,
		_m_DS3231_USART1_Data_Node_Chars,
		DS3231_USART1_Data_Node_Char_Len,
		_m_DS3231_USART1_Data_Eof,
		_m_DS3231_USART1_Eof_Len);
	
	//如果初始成功串口打印 01 02，否则只有 01
	USART_Send_Byte(USART1,0x01);
	
	I2C1_Conf_Init(&DS3231_I2C1_Conf);
	I2C2_Conf_Init(&LCD1602_I2C_Conf);
	
	DS3231_Init(_m_DS3231_Chip_Address);
	
	USART_Send_Byte(USART1,0x02);
	USART_Send_Byte(USART1,0xFF);
	USART_Send_Byte(USART1,0xFF);
	if(DS3231_I2C_Error_Code() != 0x00)
	{
		USART_Send_Byte(USART1,DS3231_I2C_Error_Code());
	}
	
	
	LCD1602_Command(&LCD1602_I2C_Conf,0x0c);
	LCD1602_Command(&LCD1602_I2C_Conf,0x01);
	LCD1602_Command(&LCD1602_I2C_Conf,0x02);
	
	LCD1602_Flash();
	
	LCD1602_Command(&LCD1602_I2C_Conf,0x01);
	LCD1602_Command(&LCD1602_I2C_Conf,0x02);
	LCD1602_SetCursor(&LCD1602_I2C_Conf,0,0);
	//
	



}












#endif 
