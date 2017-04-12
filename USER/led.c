

/***************************************
 * ����LED
 * LED ������3.3V�������� GPIOx ��
**********************************************************************************/


#include "led.h"
#include <stdarg.h>

   



/**
 * 
 * LED_Config(GPIOC,GPIO_Pin_13 | GPIO_Pin_14);
 */
void LED_Config(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(GPIOx == GPIOA)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	}
	else if(GPIOx == GPIOB)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	}
	else if(GPIOx == GPIOC)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	}
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;//��ʼ��LED0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOx, &GPIO_InitStructure);
	

	
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




