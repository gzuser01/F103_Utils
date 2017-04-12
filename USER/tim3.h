#ifndef __TIM3_H
#define	__TIM3_H

#include "stm32f10x_tim.h"  
#include "misc.h" 


/**
 * ≈‰÷√ TIM3 ∆’Õ®∂® ±∆˜
 */
void TIM3_Configuration(void);

void Register_TIM3_Callback(void (* ptr)());  

void TIM3_IRQHandler(void);


#endif /* __TIM3_H */

