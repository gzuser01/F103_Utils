
#include "gpio.h"

unsigned short int _m_GPIOx_Enable;

void GPIO_Config(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode,GPIOSpeed_TypeDef GPIO_Speed)
{
	
	GPIO_InitTypeDef gpio_init_struct;
	

	if(GPIOx == GPIOA && (_m_GPIOx_Enable & 1) == 0)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
		_m_GPIOx_Enable = _m_GPIOx_Enable | 1;
	}
	else if(GPIOx == GPIOB && (_m_GPIOx_Enable & 2) == 0)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		_m_GPIOx_Enable = _m_GPIOx_Enable | 2;
	}
	else if(GPIOx == GPIOC && (_m_GPIOx_Enable & 4) == 0)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
		_m_GPIOx_Enable = _m_GPIOx_Enable | 4;
	}
	
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin;
  gpio_init_struct.GPIO_Mode = GPIO_Mode;
	
	//对于输入是不需要的
	gpio_init_struct.GPIO_Speed = GPIO_Speed;
	
  GPIO_Init(GPIOx, &gpio_init_struct);
	

	
}

void Out_PP_GPIO_Config(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	GPIO_Config(GPIOx,GPIO_Pin,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
}

