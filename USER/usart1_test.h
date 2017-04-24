

#include "stm32f10x.h"

#include "flash_led.h"
#include "usart1_util.h"


void usart1_test(void)
{
	
	TIM_TypeDef* USART1_TIMx = TIM3;
	TIM_TypeDef* LED_TIMx = TIM4;

	GPIO_TypeDef *USART1_R_LED_GPIOx = GPIOB;
	uint16_t UART1_R_LED_GPIO_Pin = GPIO_Pin_11;

	GPIO_TypeDef *USART1_W_LED_GPIOx = GPIOB;
	uint16_t UART1_W_LED_GPIO_Pin = GPIO_Pin_12;

	
	TIMx_Configuration(USART1_TIMx,7199,99,NVIC_PriorityGroup_2,1,0);  
	TIMx_Configuration(LED_TIMx,7199,99,NVIC_PriorityGroup_2,2,0); 
	
	Flash_Led_Config(TIM4,GPIOB,UART1_R_LED_GPIO_Pin | UART1_W_LED_GPIO_Pin ); 
	
	USART1_Util_Config(USART1_TIMx,LED_TIMx,USART1_R_LED_GPIOx,UART1_R_LED_GPIO_Pin,USART1_W_LED_GPIOx,UART1_W_LED_GPIO_Pin,1); //USART1 ≈‰÷√ 		
	
}
