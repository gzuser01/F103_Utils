#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(GPIO_TypeDef *Tx_GPIOx,uint16_t Tx_GPIO_Pin,GPIO_TypeDef *Rx_GPIOx,uint16_t Rx_GPIO_Pin);

void USART1_Send_Byte(unsigned char Send_Data);

unsigned char USART1_Get_Byte(unsigned char* Get_Data);

void Register_USART1_Callback(void (* ptr)(unsigned char));

	
#endif /* __USART1_H */
