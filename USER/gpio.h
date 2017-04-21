
#ifndef __GPIO_H
#define	__GPIO_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"




void GPIO_Config(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode,GPIOSpeed_TypeDef GPIO_Speed);


#endif 
