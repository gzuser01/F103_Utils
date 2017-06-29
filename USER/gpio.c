
#include "gpio.h"

unsigned short int GPIOx_Enable;

void GPIO_Config(GPIO_TypeDef *gpiox,uint16_t gpio_pin,GPIOMode_TypeDef gpio_mode,GPIOSpeed_TypeDef gpio_speed)
{
	
	GPIO_InitTypeDef gpio_init_struct;
	

	if(gpiox == GPIOA && (GPIOx_Enable & 1) == 0)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
		GPIOx_Enable = GPIOx_Enable | 1;
	}
	else if(gpiox == GPIOB && (GPIOx_Enable & 2) == 0)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		GPIOx_Enable = GPIOx_Enable | 2;
	}
	else if(gpiox == GPIOC && (GPIOx_Enable & 4) == 0)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
		GPIOx_Enable = GPIOx_Enable | 4;
	}
	
	
	gpio_init_struct.GPIO_Pin = gpio_pin;
  gpio_init_struct.GPIO_Mode = gpio_mode;
	
	//对于输入是不需要的
	gpio_init_struct.GPIO_Speed = gpio_speed;
	
  GPIO_Init(gpiox, &gpio_init_struct);
	

	
}

void Out_PP_GPIO_Config(GPIO_TypeDef *gpiox,uint16_t gpio_pin)
{
	GPIO_Config(gpiox,gpio_pin,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
}

