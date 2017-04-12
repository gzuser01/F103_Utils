#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"
#include <stdio.h>



void LED_Config(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);

void LED_On(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
void LED_Off(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);


#endif /* __LED_H */
