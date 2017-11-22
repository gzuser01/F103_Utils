
#ifndef __USART1_UTIL_H
#define	__USART1_UTIL_H

#include "usart.h"
#include "usart_data.h"


void Flash_LED_USART_Send_Byte(USART_TypeDef* USARTx,unsigned char SendData);

unsigned char Flash_LED_USART_Get_Byte(USART_TypeDef* USARTx,unsigned char* GetData);


void Flash_LED_USART_Config(TIM_TypeDef* USARTx_TIMx,
		USART_TypeDef *USARTx,uint32_t USART_BaudRate,
		GPIO_TypeDef *USARTx_Tx_GPIOx,uint16_t USARTx_Tx_GPIO_Pin,
		GPIO_TypeDef *USARTx_Rx_GPIOx,uint16_t USARTx_Rx_GPIO_Pin,
		TIM_TypeDef* LED_TIMx,
		GPIO_TypeDef *USART_R_LED_GPIOx,uint16_t USART_R_LED_GPIO_Pin,
		GPIO_TypeDef *USART_W_LED_GPIOx,uint16_t USART_W_LED_GPIO_Pin
		);



#endif 

