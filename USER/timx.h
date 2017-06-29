#ifndef __TIMX_H
#define	__TIMX_H

#include "stm32f10x_tim.h"  
#include "misc.h" 

/**
 * ���� TIMx ��ͨ��ʱ�������ж�
 */
void TIMx_Config(TIM_TypeDef* timx,	
	uint16_t tim_prescaler,
	uint16_t tim_period);



/**
 * ���� TIMx ��ͨ��ʱ�������ж�
 */
void TIMx_With_NVIC_Config(TIM_TypeDef* timx,	
	uint16_t tim_prescaler,
	uint16_t tim_period,
	uint32_t nvic_priority_group,uint8_t nvic_irq_channel_preemption_priority,uint8_t nvic_irq_channel_sub_priority);

void Register_TIMx_Callback(TIM_TypeDef* timx,void (* ptr)());  

void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM5_IRQHandler(void);


void Delay_Nus(uint32_t nus);
void Delay_Nms(uint32_t nms);

#endif

