
/***************************************
 * ��LED
 * LED ������3.3V�������� GPIOx ��
 * 
 * ��ִ�к���������£�
 * Flash_Led_Config(TIM4,GPIOC,GPIO_Pin_13 | GPIO_Pin_14); 
 * Flash_Led_Config(TIM4,GPIOB,GPIO_Pin_12); 
 * 
 * ��Ҫ��LED������£�
 * Led_Auto_Off(GPIOC,GPIO_Pin_13 | GPIO_Pin_14,8); //0.08��
 * Led_Auto_Off(GPIOB,GPIO_Pin_12,1000); //10 ��
**********************************************************************************/


#include "flash_led.h"

uint16_t _m_Flash_LED_GPIO_Pin[] = {0,0,0,0,0,0,0,0,0,0};
GPIO_TypeDef *_m_Flash_LED_GPIOx[] = {0,0,0,0,0,0,0,0,0,0};
unsigned long _m_Flash_LED_Delay[] = {0,0,0,0,0,0,0,0,0,0};


/**
 * ��LED����ָ����ʱ�ر�
 */
void Led_Auto_Off(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin,unsigned long delay) 
{
	int i;
	
	if(GPIO_Pin == 0)
	{
		return;
	}
	
	for(i = 0;i<10;i++)
	{
		//δ���û�δռ��
		if(_m_Flash_LED_GPIO_Pin[i] == 0)
		{
			_m_Flash_LED_GPIO_Pin[i] = GPIO_Pin;
			_m_Flash_LED_GPIOx[i] = GPIOx;
			break;
		}
		
		//�ҵ���������ͬGPIO�����
		if(_m_Flash_LED_GPIO_Pin[i] == GPIO_Pin && _m_Flash_LED_GPIOx[i] == GPIOx)
		{
			break;
		}
		
	}
	
	//���_m_Flash_LED_Delay[i]δ���û��Ѿ��Լ���0����ô��LED��δ����0��ʾ������
	if(_m_Flash_LED_Delay[i] == 0)
	{
		//LED�͵�ƽ��
		GPIO_ResetBits(_m_Flash_LED_GPIOx[i], _m_Flash_LED_GPIO_Pin[i]);
	}
	
	//���¼�����
	_m_Flash_LED_Delay[i] = delay;
}



/**
 * TIMx�жϵ��Զ��ر�LED�Ļص�����
 */
void Auto_Off_TIMx_CallBack(void)  
{  
	int i;
	
	for(i = 0;i<10;i++)
	{
		//����ƶ���δ���û�δռ��
		if(_m_Flash_LED_GPIO_Pin[i] == 0)
		{
			break;
		}
		//�Լ���0����LED
		if(_m_Flash_LED_Delay[i] == 0)
		{
			//LED�ߵ�ƽ��
			GPIO_SetBits(_m_Flash_LED_GPIOx[i],_m_Flash_LED_GPIO_Pin[i]);
		}
		else if(_m_Flash_LED_Delay[i] > 0)
		{
			_m_Flash_LED_Delay[i] --;
		}
	
	}
}

/**
 * ��˸LED���ã�ע���жϵĻص�����
 * ��Ҫͨ������ TIMx_With_NVIC_Config(TIM4,7199,99,NVIC_PriorityGroup_2,2,0); ��ʼ��һ�������ȼ���ʱ���ж�
 */
void Flash_Led_Config(TIM_TypeDef* TIMx,GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	Register_TIMx_Callback(TIMx,Auto_Off_TIMx_CallBack);
	Out_PP_GPIO_Config(GPIOx, GPIO_Pin);
	//Ĭ��LED�ߵ�ƽ��
	GPIO_SetBits(GPIOx, GPIO_Pin);
}
