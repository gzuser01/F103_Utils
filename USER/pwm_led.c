/*

	//选择一个定时器，根据通道对应引脚，指定对应的 Output Compare 和引脚
	// 注意 “通道对应引脚”!!!
	TIMx_With_NVIC_Config(TIM2,7199,99,NVIC_PriorityGroup_2,0,0);
	PWM_LED_Config(TIM2,TIM_Output_Compare_2);
	PWM_Config(TIM2,TIM_Output_Compare_2,0,999,GPIOA,GPIO_Pin_1);

	//选择另外一个定时器，更新CCR值
	TIMx_With_NVIC_Config(TIM3,7199,99,NVIC_PriorityGroup_2,3,3);
	Register_TIMx_Callback(TIM3,PWM_LED_Callback);
	
*/

#include "pwm_led.h"

u16 _m_PWM_LED_CCR_Val=1000;
u16 _m_PWM_LED_Counter = 0;
TIM_TypeDef* _m_PWM_LED_TIMx;
uint16_t _m_PWM_LED_TIM_Output_Compare;


void PWM_LED_Config(TIM_TypeDef* TIMx,uint16_t PWM_LED_TIM_Output_Compare)
{
	
	_m_PWM_LED_TIMx = TIMx;
	_m_PWM_LED_TIM_Output_Compare = PWM_LED_TIM_Output_Compare;
	
}


void PWM_LED_Callback(void)
{

	if(_m_PWM_LED_Counter < 100)
	{
		_m_PWM_LED_CCR_Val -= 10;
		PWM_Mode_Config(_m_PWM_LED_TIMx,_m_PWM_LED_TIM_Output_Compare,_m_PWM_LED_CCR_Val);
		_m_PWM_LED_Counter ++;
	}
	else
	{
		_m_PWM_LED_CCR_Val += 10;
		PWM_Mode_Config(_m_PWM_LED_TIMx,_m_PWM_LED_TIM_Output_Compare,_m_PWM_LED_CCR_Val);
		_m_PWM_LED_Counter++;
		if(_m_PWM_LED_Counter == 200) _m_PWM_LED_Counter=0;
	}
}

