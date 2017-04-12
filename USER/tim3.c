#include "tim3.h"


/**
 * TIM3�жϻص�������ָ��
 */
void (*_m_tim3_irqhandler_ptr[])(void) = {0,0,0,0,0,0,0,0,0,0};



void TIM3_Configuration(void)    
{  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;    
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  
    
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
    
  TIM_TimeBaseStructure.TIM_Period = 99; //999- 0.01���ж�
  TIM_TimeBaseStructure.TIM_Prescaler = 7199;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
    
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );  
  TIM_Cmd(TIM3,ENABLE);  
	
	
	
  //�����ж����ȼ�  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    
      
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;    
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
  NVIC_Init(&NVIC_InitStructure);   
}  


/**
 * ע���жϻص�����
 * ÿ 0.01�� �� 10 ����ִ��һ��
 */
void Register_TIM3_Callback(void (* ptr)())
{
	int i;
	for(i = 0;i<10;i++)
	{
		//�Ѿ�ע��
		if(_m_tim3_irqhandler_ptr[i] == ptr)
		{
			return;
		}
		//����ƶ���δ���û�δռ��
		if(_m_tim3_irqhandler_ptr[i] == 0)
		{
			_m_tim3_irqhandler_ptr[i] = ptr;
			return;
		}
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


