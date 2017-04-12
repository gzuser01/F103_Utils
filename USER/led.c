

/***************************************
 * 点亮LED
 * LED 正极接3.3V，负极接 GPIOC 口
**********************************************************************************/


#include "led.h"
#include <stdarg.h>

   



/**
 * 要放在 main 函数
 * LED_Config(GPIO_Pin_13 | GPIO_Pin_14);
 */
void LED_Config(uint16_t GPIO_Pin)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;//初始化LED0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  
	
}





/**
 * LED低电平亮
 * LED_On(GPIO_Pin_13 | GPIO_Pin_14);
 */
void LED_On(uint16_t GPIO_Pin)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin); 
}

/**
 * LED高电平灭
 * LED_Off(GPIO_Pin_13 | GPIO_Pin_14);
 */
void LED_Off(uint16_t GPIO_Pin)
{
	GPIO_SetBits(GPIOC, GPIO_Pin); 
}




