#ifndef __TIMX_H
#define	__TIMX_H

#include "stm32f10x_tim.h"  
#include "misc.h" 

/**
 * ���� TIMx ��ͨ��ʱ�������ж�
 */
void TIMx_Config(TIM_TypeDef* TIMx,	
	uint16_t TIM_Prescaler,
	uint16_t TIM_Period);



/**
 * ���� TIMx ��ͨ��ʱ�������ж�
 */
void TIMx_With_NVIC_Config(TIM_TypeDef* TIMx,	
	uint16_t TIM_Prescaler,
	uint16_t TIM_Period,
	uint32_t NVIC_PriorityGroup,uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority);

void Register_TIMx_Callback(TIM_TypeDef* TIMx,void (* ptr)());  

void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM5_IRQHandler(void);


void Delay_Nus(uint32_t nus);
void Delay_Nms(uint32_t nms);

#endif

