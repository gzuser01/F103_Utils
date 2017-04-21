

/***************************************
 * ����LED
 * LED ������3.3V�������� GPIOx ��
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
 * LED�͵�ƽ��
 * LED_On(GPIOC,GPIO_Pin_13 | GPIO_Pin_14);
 */
void LED_On(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	GPIO_ResetBits(GPIOx, GPIO_Pin); 
}

/**
 * LED�ߵ�ƽ��
 * LED_Off(GPIOC,GPIO_Pin_13 | GPIO_Pin_14);
 */
void LED_Off(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	GPIO_SetBits(GPIOx, GPIO_Pin); 
}




