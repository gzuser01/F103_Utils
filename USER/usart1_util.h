
#ifndef __USART1_UTIL_H
#define	__USART1_UTIL_H

#include "usart1.h"
#include "usart1_data.h"


void Flash_LED_USART1_Send_Byte(unsigned char SendData);

unsigned char Flash_LED_USART1_Get_Byte(unsigned char* GetData);


void Flash_LED_USART1_Config(TIM_TypeDef* USART1_TIMx,
		GPIO_TypeDef *USART1_Tx_GPIOx,uint16_t USART1_Tx_GPIO_Pin,
		GPIO_TypeDef *USART1_Rx_GPIOx,uint16_t USART1_Rx_GPIO_Pin,
		TIM_TypeDef* LED_TIMx,
		GPIO_TypeDef *USART1_R_LED_GPIOx,uint16_t USART1_R_LED_GPIO_Pin,
		GPIO_TypeDef *USART1_W_LED_GPIOx,uint16_t USART1_W_LED_GPIO_Pin
		);



#endif 

