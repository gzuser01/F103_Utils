#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"
#include <stdio.h>



void LED_Config(uint16_t GPIO_Pin);

void LED_On(uint16_t GPIO_Pin);
void LED_Off(uint16_t GPIO_Pin);


#endif /* __LED_H */
