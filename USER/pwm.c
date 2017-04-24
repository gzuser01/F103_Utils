

/**
 * ͨ����Ӧ����
 * TIMx		CH1		CH2		CH3		CH4
 * TIM1		PA8		PA9		PA10	PA11
 * TIM2		PA0		PA1		PA2		PA3
 * TIM3		PA6		PA7		PB0		PB1
 * TIM4		PB6		PB7		PB8		PB9
 */


#include "pwm.h"



void PWM_Config(TIM_TypeDef* TIMx,
	uint16_t TIM_Output_Compare,
	uint16_t TIM_Prescaler,
	uint16_t TIM_Period,
	GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//���� TIM_TimeBaseStructure 
	GPIO_InitTypeDef GPIO_InitStructure;//���� GPIO_InitStructure 
	
	
	if(TIMx == TIM2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); 
	}
	else if(TIMx == TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	}
	else if(TIMx == TIM4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); 
	}		
	else if(TIMx == TIM5)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE); 
	}
    

	if(GPIOx == GPIOA)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	}
	else if(GPIOx == GPIOB)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	}
	else if(GPIOx == GPIOC)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	}
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin;//ѡ������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//ѡ�����ģʽΪ�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//������������������Ϊ50MHz
	GPIO_Init(GPIOx,&GPIO_InitStructure);//���趨�õĲ�����ʼ��
	
	
	TIM_TimeBaseStructure.TIM_Period=TIM_Period;//PWM������
	TIM_TimeBaseStructure.TIM_Prescaler=TIM_Prescaler;//����������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//ѡ���˼�����ģʽ
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);//�����úõĲ�����ʼ��
}

void PWM_Mode_Config(TIM_TypeDef* TIMx,uint16_t TIM_Output_Compare,uint16_t TIM_Pulse)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;//���� TIM_OCInitStructure
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//����TIM���ģʽPWM1
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//ѡ������Ƚ�״̬
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//TIM ����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse=TIM_Pulse;//�����˴�װ�벶��ȽϼĴ���������ֵ
	switch(TIM_Output_Compare)
	{
		case TIM_Output_Compare_1:
		{
			TIM_OC1Init(TIMx, &TIM_OCInitStructure);//���趨�õĲ�����ʼ����ͨ��
			TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);//ʹ��TIMx��CCR�ϵ�Ԥװ�ؼĴ���
			break;
		}
		case TIM_Output_Compare_2:
		{
			TIM_OC2Init(TIMx, &TIM_OCInitStructure);//���趨�õĲ�����ʼ����ͨ��
			TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);//ʹ��TIMx��CCR�ϵ�Ԥװ�ؼĴ���
			break;
		}
		case TIM_Output_Compare_3:
		{
			TIM_OC3Init(TIMx, &TIM_OCInitStructure);//���趨�õĲ�����ʼ����ͨ��
			TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);//ʹ��TIMx��CCR�ϵ�Ԥװ�ؼĴ���
			break;
		}
		case TIM_Output_Compare_4:
		{
			TIM_OC4Init(TIMx, &TIM_OCInitStructure);//���趨�õĲ�����ʼ����ͨ��
			TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);//ʹ��TIMx��CCR�ϵ�Ԥװ�ؼĴ���
			break;
		}
	}
	TIM_ARRPreloadConfig(TIMx,ENABLE);//ʹ��TIMx�� ARR �ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIMx,ENABLE);//ʹ��TIMx������
}






