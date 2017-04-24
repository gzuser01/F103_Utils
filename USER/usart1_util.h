
#ifndef __USART1_UTIL_H
#define	__USART1_UTIL_H

#include "usart1.h"
#include "usart1_data.h"


void USART1_Send_Byte(unsigned char SendData);

unsigned char USART1_Get_Byte(unsigned char* GetData);


void USART1_Util_Config(TIM_TypeDef* USART1_TIMx,TIM_TypeDef* LED_TIMx,
		GPIO_TypeDef *USART1_R_LED_GPIOx,uint16_t USART1_R_LED_GPIO_Pin,
		GPIO_TypeDef *USART1_W_LED_GPIOx,uint16_t USART1_W_LED_GPIO_Pin,
		unsigned short int debug);



#endif 

