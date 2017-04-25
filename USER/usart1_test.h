
#ifndef __USAET1_TEST_H
#define	__USAET1_TEST_H


#include "stm32f10x.h"

#include "flash_led.h"
#include "usart1_util.h"


void usart1_test(void)
{
	
	TIM_TypeDef* USART1_Callback_TIMx = TIM3;
	
	GPIO_TypeDef *USART1_Tx_GPIOx = GPIOA;
	uint16_t UART1_Tx_GPIO_Pin = GPIO_Pin_9;

	GPIO_TypeDef *USART1_Rx_GPIOx = GPIOA;
	uint16_t UART1_Rx_GPIO_Pin = GPIO_Pin_10;

	TIM_TypeDef* LED_TIMx = TIM4;
	
	GPIO_TypeDef *USART1_R_LED_GPIOx = GPIOB;
	uint16_t UART1_R_LED_GPIO_Pin = GPIO_Pin_11;

	GPIO_TypeDef *USART1_W_LED_GPIOx = GPIOB;
	uint16_t UART1_W_LED_GPIO_Pin = GPIO_Pin_12;

	
	TIMx_With_NVIC_Config(USART1_TIMx,7199,99,NVIC_PriorityGroup_2,1,0);  
	TIMx_With_NVIC_Config(LED_TIMx,7199,99,NVIC_PriorityGroup_2,2,0); 
	
	Flash_Led_Config(TIM4,GPIOB,UART1_R_LED_GPIO_Pin | UART1_W_LED_GPIO_Pin ); 
	
	Flash_LED_USART1_Config(USART1_Callback_TIMx,
		USART1_Tx_GPIOx,UART1_Tx_GPIO_Pin,
		USART1_Rx_GPIOx,UART1_Rx_GPIO_Pin,
		LED_TIMx,
		USART1_R_LED_GPIOx,UART1_R_LED_GPIO_Pin,
		USART1_W_LED_GPIOx,UART1_W_LED_GPIO_Pin,
		1); //USART1 ≈‰÷√ 		
	
}

#endif

