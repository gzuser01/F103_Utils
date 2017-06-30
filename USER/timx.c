#include "timx.h"

/**
 * TIM2中断回调函数的指针
 */
void (*_m_TIM2_IRQHandler_Ptr[])(void) = {0,0,0,0,0,0,0,0,0,0};

/**
 * TIM3中断回调函数的指针
 */
void (*_m_TIM3_IRQHandler_Ptr[])(void) = {0,0,0,0,0,0,0,0,0,0};

/**
 * TIM4中断回调函数的指针
 */
void (*_m_TIM4_IRQHandler_Ptr[])(void) = {0,0,0,0,0,0,0,0,0,0};

/**
 * TIM5中断回调函数的指针
 */
void (*_m_TIM5_IRQHandler_Ptr[])(void) = {0,0,0,0,0,0,0,0,0,0};


void TIMx_Config(TIM_TypeDef* TIMx,	uint16_t TIM_Prescaler,uint16_t TIM_Period)    
{  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	
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
    
  TIM_ClearITPendingBit(TIMx, TIM_IT_Update);  
  
  TIM_TimeBaseStructure.TIM_Period = TIM_Period; //99 即 0.01秒中断
  TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler;//7199
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);  
  
  TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE );  
  TIM_Cmd(TIMx,ENABLE);  
	
}

void TIMx_With_NVIC_Config(TIM_TypeDef* TIMx,	uint16_t TIM_Prescaler,	uint16_t TIM_Period,
	uint32_t NVIC_PriorityGroup,uint8_t NVIC_IRQ_ChannelPreemptionPriority,uint8_t NVIC_IRQ_ChannelSubPriority)    
{  
  
	NVIC_InitTypeDef NVIC_InitStructure;    
	
	TIMx_Config(TIMx,TIM_Prescaler,TIM_Period);
		
  //配置中断优先级  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup);    
 
	if(TIMx == TIM2)
	{
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
	}	
	else if(TIMx == TIM3)
	{
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	}
	else if(TIMx == TIM4)
	{
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	}		
	else if(TIMx == TIM5)
	{
		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
	}
	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQ_ChannelPreemptionPriority;    
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQ_ChannelSubPriority;    
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
  NVIC_Init(&NVIC_InitStructure);   
}  


/**
 * 注册中断回调函数
 * 每 0.01秒 即 10 毫秒执行一次，100Hz
 */
void Register_TIMx_Callback(TIM_TypeDef* TIMx,void (* ptr)())
{
	int i;
	void (**_m_TIMx_IRQHandler_Ptr)(void);
	
	if(TIMx == TIM2)
	{
		_m_TIMx_IRQHandler_Ptr = _m_TIM2_IRQHandler_Ptr; 
	}
	else if(TIMx == TIM3)
	{
		_m_TIMx_IRQHandler_Ptr = _m_TIM3_IRQHandler_Ptr; 
	}
	else if(TIMx == TIM4)
	{
		_m_TIMx_IRQHandler_Ptr = _m_TIM4_IRQHandler_Ptr; 
	}		
	else if(TIMx == TIM5)
	{
		_m_TIMx_IRQHandler_Ptr = _m_TIM5_IRQHandler_Ptr;  
	}
	
	for(i = 0;i<10;i++)
	{
		//已经注册
		if(_m_TIMx_IRQHandler_Ptr[i] == ptr)
		{
			return;
		}
		//序号移动到未设置或未占用
		if(_m_TIMx_IRQHandler_Ptr[i] == 0)
		{
			_m_TIMx_IRQHandler_Ptr[i] = ptr;
			return;
		}
	}
}

/**
 * 定时器2中断服务程序
 */
void TIM2_IRQHandler(void)  
{  
	int i;
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	
	for(i = 0;i<10;i++)
	{
		if(_m_TIM2_IRQHandler_Ptr[i] == 0)
		{
			break;
		}

		(* _m_TIM2_IRQHandler_Ptr[i])();

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
		if(_m_TIM3_IRQHandler_Ptr[i] == 0)
		{
			break;
		}

		(* _m_TIM3_IRQHandler_Ptr[i])();

	}
	
}


/**
 * 定时器4中断服务程序
 */
void TIM4_IRQHandler(void)  
{  
	int i;
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	
	for(i = 0;i<10;i++)
	{
		if(_m_TIM4_IRQHandler_Ptr[i] == 0)
		{
			break;
		}

		(* _m_TIM4_IRQHandler_Ptr[i])();

	}
	
}



/**
 * 定时器5中断服务程序
 */
void TIM5_IRQHandler(void)  
{  
	int i;
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	
	for(i = 0;i<10;i++)
	{
		if(_m_TIM5_IRQHandler_Ptr[i] == 0)
		{
			break;
		}

		(* _m_TIM5_IRQHandler_Ptr[i])();

	}
	
}



//us秒级延时
void Delay_Nus(uint32_t nus)
{
	uint32_t c;
	SysTick->LOAD = nus * 9;
	SysTick->VAL = 0x00;
	SysTick->CTRL = 0x01 ;
	do
	{
		c=SysTick->CTRL;
	}while(c&0x01&&!(c&(1<<16)));
	SysTick->CTRL = 0x00;
	SysTick->VAL = 0X00;
}

//ms秒级延时
void Delay_Nms(uint32_t nms)
{
	uint32_t c;
	for(c = 0;c< nms;c++)
	{
		Delay_Nus(1000);
	}
}


