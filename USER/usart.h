#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART_Config(USART_TypeDef* USARTx,uint32_t USART_BaudRate,
	GPIO_TypeDef *Tx_GPIOx,uint16_t Tx_GPIO_Pin,GPIO_TypeDef *Rx_GPIOx,uint16_t Rx_GPIO_Pin);

void USART_Send_Byte(USART_TypeDef* USARTx,unsigned char Send_Data);

unsigned char USART_Get_Byte(USART_TypeDef* USARTx,unsigned char* Get_Data);

void Register_USART_Callback(USART_TypeDef* USARTx,void (* ptr)(USART_TypeDef* USARTx,unsigned char));

	
#endif /* __USART1_H */
