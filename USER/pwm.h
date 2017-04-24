
#ifndef __PWM_H
#define	__PWM_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"  
#include "misc.h"
#include "timx.h"

#define TIM_Output_Compare_1 1
#define TIM_Output_Compare_2 2
#define TIM_Output_Compare_3 3
#define TIM_Output_Compare_4 4



/**
 * 通道对应引脚
 * TIMx		CH1		CH2		CH3		CH4
 * TIM1		PA8		PA9		PA10	PA11
 * TIM2		PA0		PA1		PA2		PA3
 * TIM3		PA6		PA7		PB0		PB1
 * TIM4		PB6		PB7		PB8		PB9
 */
void PWM_Config(TIM_TypeDef* TIMx,
	uint16_t TIM_Output_Compare,
	uint16_t TIM_Prescaler,
	uint16_t TIM_Period,
	GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);


void PWM_Mode_Config(TIM_TypeDef* TIMx,
	uint16_t TIM_Output_Compare,
	uint16_t TIM_Pulse);



#endif 

