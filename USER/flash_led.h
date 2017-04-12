

#include "led.h"
#include "tim3.h"

uint16_t _m_Flash_LED_GPIO_Pin = 0;
unsigned long _m_Flash_LED_L = 0;


/**
 * ��LED����ָ����ʱ�ر�
 */
void Led_Auto_Off(unsigned long delay) 
{
	if(_m_Flash_LED_GPIO_Pin == 0)
	{
		return;
	}
	if(_m_Flash_LED_L == 0)
	{
		LED_On(_m_Flash_LED_GPIO_Pin);
	}
	_m_Flash_LED_L = delay;
}

/**
 * TIM3�жϵ��Զ��ر�LED�Ļص�����
 */
void Auto_Off_TIM3_CallBack(void)  
{  
	
	if(_m_Flash_LED_GPIO_Pin == 0)
	{
		return;
	}
	if(_m_Flash_LED_L == 0)
	{
			LED_Off(_m_Flash_LED_GPIO_Pin);
	}
	else if(_m_Flash_LED_L > 0)
	{
		_m_Flash_LED_L --;
	}
	
}

/**
 * ��˸LED���ã�ע���жϵĻص�����
 */
void Flash_Led_Config(uint16_t GPIO_Pin)
{
	_m_Flash_LED_GPIO_Pin = GPIO_Pin;
	Register_TIM3_IRQHandler(Auto_Off_TIM3_CallBack);
}

