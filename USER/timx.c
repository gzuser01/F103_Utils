#include "timx.h"

/**
 * TIM2�жϻص�������ָ��
 */
void (*_m_tim2_irqhandler_ptr[])(void) = {0,0,0,0,0,0,0,0,0,0};

/**
 * TIM3�жϻص�������ָ��
 */
void (*_m_tim3_irqhandler_ptr[])(void) = {0,0,0,0,0,0,0,0,0,0};

/**
 * TIM4�жϻص�������ָ��
 */
void (*_m_tim4_irqhandler_ptr[])(void) = {0,0,0,0,0,0,0,0,0,0};

/**
 * TIM5�жϻص�������ָ��
 */
void (*_m_tim5_irqhandler_ptr[])(void) = {0,0,0,0,0,0,0,0,0,0};


void TIMx_Config(TIM_TypeDef* timx,	
	uint16_t tim_prescaler,
	uint16_t tim_period)    
{  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	
	if(timx == TIM2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); 
	}
	else if(timx == TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	}
	else if(timx == TIM4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); 
	}		
	else if(timx == TIM5)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE); 
	}
    
  TIM_ClearITPendingBit(timx, TIM_IT_Update);  
  
  TIM_TimeBaseStructure.TIM_Period = tim_period; //99 �� 0.01���ж�
  TIM_TimeBaseStructure.TIM_Prescaler = tim_prescaler;//7199
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(timx, &TIM_TimeBaseStructure);  
  
  TIM_ITConfig(timx, TIM_IT_Update, ENABLE );  
  TIM_Cmd(timx,ENABLE);  
	
}

void TIMx_With_NVIC_Config(TIM_TypeDef* timx,	
	uint16_t tim_prescaler,
	uint16_t tim_period,
	uint32_t nvic_priority_group,uint8_t nvic_irq_channel_preemption_priority,uint8_t nvic_irq_channel_sub_priority)    
{  
  
	NVIC_InitTypeDef NVIC_InitStructure;    
	
	TIMx_Config(timx,tim_prescaler,tim_period);
		
  //�����ж����ȼ�  
  NVIC_PriorityGroupConfig(nvic_priority_group);    
 
	if(timx == TIM2)
	{
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
	}	
	else if(timx == TIM3)
	{
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	}
	else if(timx == TIM4)
	{
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	}		
	else if(timx == TIM5)
	{
		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
	}
	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = nvic_irq_channel_preemption_priority;    
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = nvic_irq_channel_sub_priority;    
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
  NVIC_Init(&NVIC_InitStructure);   
}  


/**
 * ע���жϻص�����
 * ÿ 0.01�� �� 10 ����ִ��һ�Σ�100Hz
 */
void Register_TIMx_Callback(TIM_TypeDef* timx,void (* ptr)())
{
	int i;
	void (**_m_timx_irqhandler_ptr)(void);
	
	if(timx == TIM2)
	{
		_m_timx_irqhandler_ptr = _m_tim2_irqhandler_ptr; 
	}
	else if(timx == TIM3)
	{
		_m_timx_irqhandler_ptr = _m_tim3_irqhandler_ptr; 
	}
	else if(timx == TIM4)
	{
		_m_timx_irqhandler_ptr = _m_tim4_irqhandler_ptr; 
	}		
	else if(timx == TIM5)
	{
		_m_timx_irqhandler_ptr = _m_tim5_irqhandler_ptr;  
	}
	
	for(i = 0;i<10;i++)
	{
		//�Ѿ�ע��
		if(_m_timx_irqhandler_ptr[i] == ptr)
		{
			return;
		}
		//����ƶ���δ���û�δռ��
		if(_m_timx_irqhandler_ptr[i] == 0)
		{
			_m_timx_irqhandler_ptr[i] = ptr;
			return;
		}
	}
}

/**
 * ��ʱ��2�жϷ������
 */
void TIM2_IRQHandler(void)  
{  
	int i;
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	
	for(i = 0;i<10;i++)
	{
		if(_m_tim2_irqhandler_ptr[i] == 0)
		{
			break;
		}

		(* _m_tim2_irqhandler_ptr[i])();

	}
	
}
  

/**
 * ��ʱ��3�жϷ������
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


/**
 * ��ʱ��4�жϷ������
 */
void TIM4_IRQHandler(void)  
{  
	int i;
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	
	for(i = 0;i<10;i++)
	{
		if(_m_tim4_irqhandler_ptr[i] == 0)
		{
			break;
		}

		(* _m_tim4_irqhandler_ptr[i])();

	}
	
}



/**
 * ��ʱ��5�жϷ������
 */
void TIM5_IRQHandler(void)  
{  
	int i;
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	
	for(i = 0;i<10;i++)
	{
		if(_m_tim5_irqhandler_ptr[i] == 0)
		{
			break;
		}

		(* _m_tim5_irqhandler_ptr[i])();

	}
	
}



//us�뼶��ʱ
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

//ms�뼶��ʱ
void Delay_Nms(uint32_t nms)
{
	uint32_t c;
	for(c = 0;c< nms;c++)
	{
		Delay_Nus(1000);
	}
}


