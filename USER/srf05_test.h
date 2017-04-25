
#ifndef __SRF05_TEST_H
#define	__SRF05_TEST_H


#include "gpio.h"
#include "usart1.h"
#include "timx.h"


//微秒级的延时
void delay_nus(uint32_t nus)
{
 uint32_t temp;
 SysTick->LOAD = nus * 11;
 SysTick->VAL = 0x00;
 SysTick->CTRL = 0x01 ;
 do
 {
  temp=SysTick->CTRL;
 
 }while(temp&0x01&&!(temp&(1<<16)));
 SysTick->CTRL = 0x00;
 SysTick->VAL = 0X00;
}



void usart1_test(void)
{
	//
	u16 count;
	float length;
	char x[8];
	unsigned int i;
	//
	TIM_TypeDef* SRF05_TIMx = TIM2;
	
	GPIO_TypeDef *Trig_GPIOx = GPIOA;
	uint16_t Trig_GPIO_Pin = GPIO_Pin_5;

	GPIO_TypeDef *Echo_GPIOx = GPIOA;
	uint16_t Echo_GPIO_Pin = GPIO_Pin_6;
	
	//
	GPIO_TypeDef *USART1_Tx_GPIOx = GPIOA;
	uint16_t USART1_Tx_GPIO_Pin = GPIO_Pin_9;

	GPIO_TypeDef *USART1_Rx_GPIOx = GPIOA;
	uint16_t USART1_Rx_GPIO_Pin = GPIO_Pin_10;
	
	//
	USART1_Config(USART1_Tx_GPIOx,USART1_Tx_GPIO_Pin,USART1_Rx_GPIOx,USART1_Rx_GPIO_Pin); //USART1 配置 	
	//
	GPIO_Config(Trig_GPIOx,Trig_GPIO_Pin,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
	GPIO_Config(Echo_GPIOx,Echo_GPIO_Pin,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);
	
	TIMx_Config(SRF05_TIMx,71,49999);
	TIM_Cmd(SRF05_TIMx, DISABLE);	
	
	while (1)
	{
		
		GPIO_SetBits(Trig_GPIOx,Trig_GPIO_Pin);
		
		delay_nus(20);//拉高超过10us，发射超声波
		
		GPIO_ResetBits(Trig_GPIOx,Trig_GPIO_Pin);
		
		SRF05_TIMx->CNT=0;//计数器清0
		
		while(GPIO_ReadInputDataBit(Echo_GPIOx,Echo_GPIO_Pin)==0);//等待ECHO脚高电平
		
		TIM_Cmd(SRF05_TIMx, ENABLE);// 允许 计数器 计数
		
		while((GPIO_ReadInputDataBit(Echo_GPIOx,Echo_GPIO_Pin)==1)&&(SRF05_TIMx->CNT < SRF05_TIMx->ARR - 10));
		
		TIM_Cmd(SRF05_TIMx, DISABLE);
		count = SRF05_TIMx->CNT;//ECHO脚低电平后读取计数器的值，从而算出往返时间
		length = count / 58.0;
		
		sprintf(x, "%g", length);
		USART1_Send_Byte('\r');
		USART1_Send_Byte('\n');
		for(i = 0;i<8;i++)
		{
			USART1_Send_Byte(x[i]);
		}
		USART1_Send_Byte('\r');
		USART1_Send_Byte('\n');
		
		//200 ms
		delay_nus(200000);
	}
	
	
}




#endif
