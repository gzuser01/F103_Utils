

/**
 * 通道对应引脚
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
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//设置 TIM_TimeBaseStructure 
	GPIO_InitTypeDef GPIO_InitStructure;//设置 GPIO_InitStructure 
	
	
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
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin;//选择引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//选择输出模式为复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//设置引脚最高输出速率为50MHz
	GPIO_Init(GPIOx,&GPIO_InitStructure);//将设定好的参数初始化
	
	
	TIM_TimeBaseStructure.TIM_Period=TIM_Period;//PWM的周期
	TIM_TimeBaseStructure.TIM_Prescaler=TIM_Prescaler;//设置用来作为 TIMx 时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//选择了计数器模式
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);//将设置好的参数初始化
}

void PWM_Mode_Config(TIM_TypeDef* TIMx,uint16_t TIM_Output_Compare,uint16_t TIM_Pulse)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;//设置 TIM_OCInitStructure
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//设置TIM输出模式PWM1
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//选择输出比较状态
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//TIM 输出比较极性高
	TIM_OCInitStructure.TIM_Pulse=TIM_Pulse;//设置了待装入捕获比较寄存器的脉冲值
	switch(TIM_Output_Compare)
	{
		case TIM_Output_Compare_1:
		{
			TIM_OC1Init(TIMx, &TIM_OCInitStructure);//将设定好的参数初始化给通道
			TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);//使能TIMx在CCR上的预装载寄存器
			break;
		}
		case TIM_Output_Compare_2:
		{
			TIM_OC2Init(TIMx, &TIM_OCInitStructure);//将设定好的参数初始化给通道
			TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);//使能TIMx在CCR上的预装载寄存器
			break;
		}
		case TIM_Output_Compare_3:
		{
			TIM_OC3Init(TIMx, &TIM_OCInitStructure);//将设定好的参数初始化给通道
			TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);//使能TIMx在CCR上的预装载寄存器
			break;
		}
		case TIM_Output_Compare_4:
		{
			TIM_OC4Init(TIMx, &TIM_OCInitStructure);//将设定好的参数初始化给通道
			TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);//使能TIMx在CCR上的预装载寄存器
			break;
		}
	}
	TIM_ARRPreloadConfig(TIMx,ENABLE);//使能TIMx在 ARR 上的预装载寄存器
	TIM_Cmd(TIMx,ENABLE);//使能TIMx的外设
}






