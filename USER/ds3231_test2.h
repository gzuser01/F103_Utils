#ifndef __TIMX_TEST_H
#define	__TIMX_TEST_H

/*********************
 * ds3231 连接 i2c1
 * ds3231 的 sqw 连接到 GPIOA 15，产生1Hz的io中断
 *
 *
 *
 *
 *********************/


#include "ds3231.h"
#include "usart.h"
#include "usart_data.h"
#include "timx.h"
#include "stm32f10x_exti.h"  


/** 串口的回调配置 **/
TIM_TypeDef* _m_TIMX_TEST_USART_Callback_TIMx = TIM3;
/** 串口1的配置 **/
uint32_t _m_TIMX_TEST_USART1_BaudRate = 115200;	
GPIO_TypeDef *_m_TIMX_TEST_USART1_Tx_GPIOx = GPIOA;
uint16_t _m_TIMX_TEST_USART1_Tx_GPIO_Pin = GPIO_Pin_9;
GPIO_TypeDef *_m_TIMX_TEST_USART1_Rx_GPIOx = GPIOA;
uint16_t _m_TIMX_TEST_USART1_Rx_GPIO_Pin = GPIO_Pin_10;
/** 串口1的配置 **/



uint8_t sqw_exti_fired = 0;
uint16_t timx_callback_count = 0;


//定时器中断通过一个计数器，统计 ds3231 的1Hz方波输出之间的实际中断次数，应等于定时器在1秒内执行的理论次数
void timx_callback(void)
{
	if(sqw_exti_fired == 1)
	{
		sqw_exti_fired = 0;
		USART_Send_Byte(USART1,0xF0);
		USART_Send_Byte(USART1,0xF0);
		USART_Send_Byte(USART1,timx_callback_count >> 8);
		USART_Send_Byte(USART1,timx_callback_count & 0xFF);
		if(timx_callback_count != 999)
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
	int i = 0;
	uint8_t data[7] = {0,0,0,0,0,0,0};

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
			}
			USART_Send_Byte(USART1,0xFF);
			USART_Send_Byte(USART1,0xFF);
			USART_Send_Byte(USART1,data[1]);
			USART_Send_Byte(USART1,data[0]);		
		} 
	}		
}


void ds3231_test2(void)
{
	
	sqw_exti_init();
	
	//优先级 sqw 的中断，否则它不能正常读取时间信息
	TIMx_With_NVIC_Config(TIM3,7199,9,NVIC_PriorityGroup_1,1,1); 
	
	USART_Config(USART1,_m_TIMX_TEST_USART1_BaudRate,
		_m_TIMX_TEST_USART1_Tx_GPIOx,_m_TIMX_TEST_USART1_Tx_GPIO_Pin,_m_TIMX_TEST_USART1_Rx_GPIOx,_m_TIMX_TEST_USART1_Rx_GPIO_Pin); //USART1 配置 	
	
	I2C1_Conf_Init(&DS3231_I2C1_Conf);
		
	Register_TIMx_Callback(TIM3,timx_callback);
	
	
	//如果初始成功串口打印 01 02，否则只有 01
	USART_Send_Byte(USART1,0x01);
	
	DS3231_Init(_m_DS3231_Chip_Address);
	
	USART_Send_Byte(USART1,0x02);
	USART_Send_Byte(USART1,0xFF);
	USART_Send_Byte(USART1,0xFF);
	if(DS3231_I2C_Error_Code() != 0x00)
	{
		USART_Send_Byte(USART1,DS3231_I2C_Error_Code());
	}
	
	
	
}






#endif 

