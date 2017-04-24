
#ifndef __PWM_LED_H
#define	__PWM_LED_H

#include "pwm.h"



void PWM_LED_Config(TIM_TypeDef* TIMx,uint16_t PWM_LED_TIM_Output_Compare);

void PWM_LED_Callback(void);


#endif 

