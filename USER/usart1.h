#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(GPIO_TypeDef *tx_gpiox,uint16_t tx_gpio_pin,GPIO_TypeDef *rx_gpiox,uint16_t rx_gpio_pin);

void USART1_Send_Byte(unsigned char send_data);

unsigned char USART1_Get_Byte(unsigned char* get_data);

void Register_USART1_Callback(void (* ptr)(unsigned char));

	
#endif /* __USART1_H */
