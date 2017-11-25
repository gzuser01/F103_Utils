#ifndef __DS3231_WITH_LCD1602_TEST_H
#define	__DS3231_WITH_LCD1602_TEST_H


#include "lcd1602_test.h"
#include "ds3231_test.h"
#include "usart.h"
#include "usart_data.h"
#include "timx.h"
#include "stm32f10x_exti.h"  

uint8_t sqw_exti_fired = 0;
uint16_t timx_callback_count = 0;

uint8_t _m_last_data[7] = {0,0,0,0,0,0,0};


void display_ms(void)
{
	char c[1];
	if(sqw_exti_fired == 1)
	{
		sqw_exti_fired = 0;
		USART_Send_Byte(USART1,0xF0);
		USART_Send_Byte(USART1,0xF0);
		USART_Send_Byte(USART1,timx_callback_count >> 8);
		USART_Send_Byte(USART1,timx_callback_count & 0xFF);
		
		if(timx_callback_count != 9)
		{
			USART_Send_Byte(USART1,0xEF);
			USART_Send_Byte(USART1,0xEF);
			USART_Send_Byte(USART1,0xEF);
			USART_Send_Byte(USART1,0xEF);
			USART_Send_Byte(USART1,0xEF);
		}
		
		timx_callback_count = 0;
	}
	else
		{
		timx_callback_count ++;
	}
		
	LCD1602_SetCursor(&LCD1602_I2C_Conf,9,1);
	//
	sprintf(c, "%1d", timx_callback_count);
	LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);

	
}



void sqw_exti_init()  
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);

	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; 	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
	NVIC_Init(&NVIC_InitStructure); 		
}

void EXTI15_10_IRQHandler()	   
{
	char c[2];
	int i = 0;
	uint8_t data[7] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

	if(EXTI_GetITStatus(EXTI_Line15)==SET)
	{
   	EXTI_ClearITPendingBit(EXTI_Line15);//清中断
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == Bit_RESET)	   //确定沿
		{
			sqw_exti_fired = 1;
			
			DS3231_Read(&DS3231_I2C1_Conf,_m_DS3231_Chip_Address,_m_DS3231_Sub_Address,data);
			for(i=0;i<7;i++)
			{
				data[i] = bcd_to_dec(data[i]);
				USART_Send_Byte(USART1,data[i]);					
			}
			USART_Send_Byte(USART1,0xFF);
			USART_Send_Byte(USART1,0xFF);
			
			if(_m_last_data[1] != data[1])
			{
				LCD1602_Command(&LCD1602_I2C_Conf,0x01);
				LCD1602_Command(&LCD1602_I2C_Conf,0x02);
				for(i=0;i<7;i++)
				{
					_m_last_data[i] = 0xFF;
				}
			}

			if(_m_last_data[6] != data[6])
			{
				LCD1602_SetCursor(&LCD1602_I2C_Conf,0,0);
				sprintf(c, "%2d", data[6]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,'2');
				LCD1602_Send_Char(&LCD1602_I2C_Conf,'0');
				LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,'-');
				_m_last_data[6] = data[6];
			}
			//
			if(_m_last_data[5] != data[5])
			{
				LCD1602_SetCursor(&LCD1602_I2C_Conf,5,0);
				sprintf(c, "%02d", data[5]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,'-');
				_m_last_data[5] = data[5];
			}
			//
			if(_m_last_data[4] != data[4])
			{
				LCD1602_SetCursor(&LCD1602_I2C_Conf,8,0);
				sprintf(c, "%02d", data[4]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,' ');
				LCD1602_Send_Char(&LCD1602_I2C_Conf,' ');
				_m_last_data[4] = data[4];
			}

			//
			if(_m_last_data[2] != data[2])
			{
				LCD1602_SetCursor(&LCD1602_I2C_Conf,0,1);
				sprintf(c, "%02d", data[2]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,':');
				_m_last_data[2] = data[2];
			}
			//
			if(_m_last_data[1] != data[1])
			{
				LCD1602_SetCursor(&LCD1602_I2C_Conf,3,1);
				sprintf(c, "%02d", data[1]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
				LCD1602_Send_Char(&LCD1602_I2C_Conf,':');
				_m_last_data[1] = data[1];
			}
			//
			LCD1602_SetCursor(&LCD1602_I2C_Conf,6,1);
			sprintf(c, "%02d", data[0]);
			LCD1602_Send_Char(&LCD1602_I2C_Conf,c[0]);
			LCD1602_Send_Char(&LCD1602_I2C_Conf,c[1]);
			LCD1602_Send_Char(&LCD1602_I2C_Conf,'.');
		} 
	}		
}

void ds3231_with_lcd1602_test(void)
{

	sqw_exti_init();
	
	TIMx_With_NVIC_Config(TIM3,7199,999,NVIC_PriorityGroup_1,1,1); 
	
	USART_Config(USART1,_m_DS3231_USART1_BaudRate,
		_m_DS3231_USART1_Tx_GPIOx,_m_DS3231_USART1_Tx_GPIO_Pin,_m_DS3231_USART1_Rx_GPIOx,_m_DS3231_USART1_Rx_GPIO_Pin); //USART1 配置 	
	
	Register_USART_Callback(USART1,DS3231_USART_Received_Data);
	
	Register_TIMx_Callback(TIM3,DS3231_USART1_Recieve_Processor);
	Register_TIMx_Callback(TIM3,display_ms);
	
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
	
	//LCD1602_Command(&LCD1602_I2C_Conf,0x0c);
	//LCD1602_Command(&LCD1602_I2C_Conf,0x01);
	//LCD1602_Command(&LCD1602_I2C_Conf,0x02);
	//LCD1602_SetCursor(&LCD1602_I2C_Conf,0,0);
	//
	



}












#endif 
