

#ifndef __FLASH_LED_H
#define	__FLASH_LED_H

#include "gpio.h"
#include "timx.h"

void Led_Auto_Off(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin,unsigned long delay) ;


void Flash_Led_Config(TIM_TypeDef* TIMx,GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);

#endif 


