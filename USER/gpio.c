
#include "gpio.h"

unsigned short int GPIOx_Enable;

void GPIO_Config(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode,GPIOSpeed_TypeDef GPIO_Speed)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	

	if(GPIOx == GPIOA && (GPIOx_Enable & 1) == 0)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
		GPIOx_Enable = GPIOx_Enable | 1;
	}
	else if(GPIOx == GPIOB && (GPIOx_Enable & 2) == 0)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		GPIOx_Enable = GPIOx_Enable | 2;
	}
	else if(GPIOx == GPIOC && (GPIOx_Enable & 4) == 0)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
		GPIOx_Enable = GPIOx_Enable | 4;
	}
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;//≥ı ºªØLED0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
  GPIO_Init(GPIOx, &GPIO_InitStructure);
	

	
}

void Out_PP_GPIO_Config(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	GPIO_Config(GPIOx,GPIO_Pin,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
}

