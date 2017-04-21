

/***************************************
 * 点亮LED
 * LED 正极接3.3V，负极接 GPIOx 口
**********************************************************************************/

#include "gpio.h"
#include "led.h"
#include <stdarg.h>

   



/**
 * 
 * LED_Config(GPIOC,GPIO_Pin_13 | GPIO_Pin_14);
 */
void LED_Config(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	
	GPIO_Config(GPIOx,GPIO_Pin,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);

	
}



/**
 * LED低电平亮
 * LED_On(GPIOC,GPIO_Pin_13 | GPIO_Pin_14);
 */
void LED_On(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	GPIO_ResetBits(GPIOx, GPIO_Pin); 
}

/**
 * LED高电平灭
 * LED_Off(GPIOC,GPIO_Pin_13 | GPIO_Pin_14);
 */
void LED_Off(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	GPIO_SetBits(GPIOx, GPIO_Pin); 
}




