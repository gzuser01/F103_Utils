#include "tim3.h"


/**
 * TIM3中断回调函数的指针
 */
void (*_m_tim3_irqhandler_ptr[])(void) = {0,0,0,0,0,0,0,0,0,0};



void TIM3_Configuration(void)    
{  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;    
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  
    
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
    
  TIM_TimeBaseStructure.TIM_Period = 99; //999- 0.01秒中断
  TIM_TimeBaseStructure.TIM_Prescaler = 7199;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
    
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );  
  TIM_Cmd(TIM3,ENABLE);  
	
	
	
  //配置中断优先级  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    
      
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;    
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
  NVIC_Init(&NVIC_InitStructure);   
}  


/**
 * 注册中断回调函数
 * 每 0.01秒 即 10 毫秒执行一次
 */
void Register_TIM3_Callback(void (* ptr)())
{
	int i;
	for(i = 0;i<10;i++)
	{
		//已经注册
		if(_m_tim3_irqhandler_ptr[i] == ptr)
		{
			return;
		}
		//序号移动到未设置或未占用
		if(_m_tim3_irqhandler_ptr[i] == 0)
		{
			_m_tim3_irqhandler_ptr[i] = ptr;
			return;
		}
	}
}
  

/**
 * 定时器3中断服务程序
 */
void TIM3_IRQHandler(void)  
{  
	int i;
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	
	for(i = 0;i<10;i++)
	{
		if(_m_tim3_irqhandler_ptr[i] == 0)
		{
			break;
		}

		(* _m_tim3_irqhandler_ptr[i])();

	}
	
}


