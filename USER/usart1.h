#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);

void UART1_Send_Byte(unsigned char SendData);

unsigned char UART1_Get_Byte(unsigned char* GetData);

void Register_USART1_Callback(void (* ptr)(unsigned char));

	
#endif /* __USART1_H */
