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

void TIMx_Configuration(TIM_TypeDef* TIMx,uint32_t NVIC_PriorityGroup,
	uint8_t NVIC_IRQChannelPreemptionPriority,
	uint8_t NVIC_IRQChannelSubPriority)    
{  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;    
	
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
    
  TIM_TimeBaseStructure.TIM_Period = 99; //99 �� 0.01���ж�
  TIM_TimeBaseStructure.TIM_Prescaler = 7199;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);  
    
  TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE );  
  TIM_Cmd(TIMx,ENABLE);  
	
	
	
  //�����ж����ȼ�  
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
	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;    
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriority;    
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
  NVIC_Init(&NVIC_InitStructure);   
}  


/**
 * ע���жϻص�����
 * ÿ 0.01�� �� 10 ����ִ��һ�Σ�100Hz
 */
void Register_TIMx_Callback(TIM_TypeDef* TIMx,void (* ptr)())
{
	int i;
	void (**_m_timx_irqhandler_ptr)(void);
	
	if(TIMx == TIM2)
	{
		_m_timx_irqhandler_ptr = _m_tim2_irqhandler_ptr; 
	}
	else if(TIMx == TIM3)
	{
		_m_timx_irqhandler_ptr = _m_tim3_irqhandler_ptr; 
	}
	else if(TIMx == TIM4)
	{
		_m_timx_irqhandler_ptr = _m_tim4_irqhandler_ptr; 
	}		
	else if(TIMx == TIM5)
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

