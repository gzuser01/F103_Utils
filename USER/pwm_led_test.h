
#include "pwm_led.h"

void pwm_led_test(void)
{
	
	//选择一个定时器，根据通道对应引脚，指定对应的 Output Compare 和引脚
	// 注意 “通道对应引脚”!!!
	TIMx_Configuration(TIM2,7199,99,NVIC_PriorityGroup_2,0,0);
	PWM_LED_Config(TIM2,TIM_Output_Compare_2);
	PWM_Config(TIM2,TIM_Output_Compare_2,0,999,GPIOA,GPIO_Pin_1);

	//选择另外一个定时器，更新CCR值
	TIMx_Configuration(TIM3,7199,99,NVIC_PriorityGroup_2,3,3);
	Register_TIMx_Callback(TIM3,PWM_LED_Callback);
	
	
}

