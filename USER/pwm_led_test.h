
#ifndef __PWM_LED_TEST_H
#define	__PWM_LED_TEST_H



#include "pwm_led.h"

void pwm_led_test(void)
{
	
	//ѡ��һ����ʱ��������ͨ����Ӧ���ţ�ָ����Ӧ�� Output Compare ������
	// ע�� ��ͨ����Ӧ���š�!!!
	TIMx_With_NVIC_Config(TIM2,7199,99,NVIC_PriorityGroup_2,0,0);
	PWM_LED_Config(TIM2,TIM_Output_Compare_2);
	PWM_Config(TIM2,TIM_Output_Compare_2,0,999,GPIOA,GPIO_Pin_1);

	//ѡ������һ����ʱ��������CCRֵ
	TIMx_With_NVIC_Config(TIM3,7199,99,NVIC_PriorityGroup_2,3,3);
	Register_TIMx_Callback(TIM3,PWM_LED_Callback);
	
	
}

#endif

