
#ifndef __GPIO_H
#define	__GPIO_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

void Out_PP_GPIO_Config(GPIO_TypeDef *gpiox,uint16_t gpio_pin);


void GPIO_Config(GPIO_TypeDef *gpiox,uint16_t gpio_pin,GPIOMode_TypeDef gpio_mode,GPIOSpeed_TypeDef gpio_speed);


#endif 
