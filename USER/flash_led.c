
/***************************************
 * 闪LED
 * LED 正极接3.3V，负极接 GPIOx 口
 * 
 * 在执行函数添加如下：
 * Flash_Led_Config(TIM4,GPIOC,GPIO_Pin_13 | GPIO_Pin_14); 
 * Flash_Led_Config(TIM4,GPIOB,GPIO_Pin_12); 
 * 
 * 需要闪LED添加如下：
 * Led_Auto_Off(GPIOC,GPIO_Pin_13 | GPIO_Pin_14,8); //0.08秒
 * Led_Auto_Off(GPIOB,GPIO_Pin_12,1000); //10 秒
**********************************************************************************/


#include "flash_led.h"

uint16_t _m_Flash_LED_GPIO_Pin[] = {0,0,0,0,0,0,0,0,0,0};
GPIO_TypeDef *_m_Flash_LED_GPIOx[] = {0,0,0,0,0,0,0,0,0,0};
unsigned long _m_Flash_LED_Delay[] = {0,0,0,0,0,0,0,0,0,0};


/**
 * 打开LED，并指定延时关闭
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
		//未设置或未占用
		if(_m_Flash_LED_GPIO_Pin[i] == 0)
		{
			_m_Flash_LED_GPIO_Pin[i] = GPIO_Pin;
			_m_Flash_LED_GPIOx[i] = GPIOx;
			break;
		}
		
		//找到已设置相同GPIO的序号
		if(_m_Flash_LED_GPIO_Pin[i] == GPIO_Pin && _m_Flash_LED_GPIOx[i] == GPIOx)
		{
			break;
		}
		
	}
	
	//如果_m_Flash_LED_Delay[i]未设置或已经自减到0，那么亮LED，未减到0表示正在亮
	if(_m_Flash_LED_Delay[i] == 0)
	{
		//LED低电平亮
		GPIO_ResetBits(_m_Flash_LED_GPIOx[i], _m_Flash_LED_GPIO_Pin[i]);
	}
	
	//更新计数器
	_m_Flash_LED_Delay[i] = delay;
}



/**
 * TIMx中断的自动关闭LED的回调函数
 */
void Auto_Off_TIMx_CallBack(void)  
{  
	int i;
	
	for(i = 0;i<10;i++)
	{
		//序号移动到未设置或未占用
		if(_m_Flash_LED_GPIO_Pin[i] == 0)
		{
			break;
		}
		//自减到0，灭LED
		if(_m_Flash_LED_Delay[i] == 0)
		{
			//LED高电平灭
			GPIO_SetBits(_m_Flash_LED_GPIOx[i],_m_Flash_LED_GPIO_Pin[i]);
		}
		else if(_m_Flash_LED_Delay[i] > 0)
		{
			_m_Flash_LED_Delay[i] --;
		}
	
	}
}

/**
 * 闪烁LED配置，注册中断的回调函数
 * 需要通过类似 TIMx_With_NVIC_Config(TIM4,7199,99,NVIC_PriorityGroup_2,2,0); 初始化一个低优先级的时间中断
 */
void Flash_Led_Config(TIM_TypeDef* TIMx,GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	Register_TIMx_Callback(TIMx,Auto_Off_TIMx_CallBack);
	Out_PP_GPIO_Config(GPIOx, GPIO_Pin);
	//默认LED高电平灭
	GPIO_SetBits(GPIOx, GPIO_Pin);
}
