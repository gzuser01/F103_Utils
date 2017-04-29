
/**
 * srf05测试
 * 注意供电，否则数据不准
 *
 */

#ifndef __SRF05_TEST_H
#define	__SRF05_TEST_H


#include "gpio.h"
#include "usart1.h"
#include "timx.h"

void init_char(char *c)
{
	int i;
	for(i=0;i<8;i++)
	{
		c[i] = 0x00;
	}
}

void srf05_test(void)
{
	//
	u16 count;
	float length;
	char x[8];
	unsigned int i;
	
	uint32_t timeout;
	//
	TIM_TypeDef* SRF05_TIMx = TIM2;
	
	GPIO_TypeDef *Trig_GPIOx = GPIOC;
	uint16_t Trig_GPIO_Pin = GPIO_Pin_14;

	GPIO_TypeDef *Echo_GPIOx = GPIOC;
	uint16_t Echo_GPIO_Pin = GPIO_Pin_15;
	
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
	
	TIMx_Config(SRF05_TIMx,71,0xFFFF);
	TIM_Cmd(SRF05_TIMx, DISABLE);	
	
	while (1)
	{
		count = 0;
		USART1_Send_Byte('\r');
		USART1_Send_Byte('\n');
		

		GPIO_SetBits(Trig_GPIOx,Trig_GPIO_Pin);
		Delay_Nus(20);
		GPIO_ResetBits(Trig_GPIOx,Trig_GPIO_Pin);

		
		SRF05_TIMx->CNT=0;//计数器清0
		
		
		timeout = 2592000; // 36ms * 72000000
		while(GPIO_ReadInputDataBit(Echo_GPIOx,Echo_GPIO_Pin)==Bit_RESET)//等待ECHO脚高电平
		{
			if(timeout == 0)
			{
				USART1_Send_Byte('-');
				USART1_Send_Byte('1');
				break;
			}
			timeout --;
		}
		
		
		
		if(timeout > 1)
		{
			
			TIM_Cmd(SRF05_TIMx, ENABLE);// 允许 计数器 计数
			
			timeout = 2592000; // 36ms * 72000000
			while((GPIO_ReadInputDataBit(Echo_GPIOx,Echo_GPIO_Pin)==Bit_SET) && (SRF05_TIMx->ARR-10))
			{
				if(timeout == 0)
				{
					USART1_Send_Byte('-');
					USART1_Send_Byte('1');
					break;
				}
				timeout --;
			}

			
			if(timeout > 1)
			{

				TIM_Cmd(SRF05_TIMx, DISABLE);
				count = SRF05_TIMx->CNT;//ECHO脚低电平后读取计数器的值，从而算出往返时间
				
				length = count / 58.0;

				init_char(x);
				sprintf(x, "%g", length);

				for(i = 0;i<8;i++)
				{
					USART1_Send_Byte(x[i]);
				}
			}
			USART1_Send_Byte('\r');
			USART1_Send_Byte('\n');
			
		}
		//200 ms
		Delay_Nms(200);
	}
	
	
}




#endif
