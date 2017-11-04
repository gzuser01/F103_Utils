
#include "usart1_util.h"
#include "flash_led.h"

GPIO_TypeDef *_m_USART1_R_LED_GPIOx;
GPIO_TypeDef *_m_USART1_W_LED_GPIOx;
uint16_t _m_USART1_R_LED_GPIO_Pin = 0;
uint16_t _m_USART1_W_LED_GPIO_Pin = 0;






void Flash_LED_USART1_Send_Byte(unsigned char SendData)
{
	
	if(_m_USART1_W_LED_GPIO_Pin)
	{
		Led_Auto_Off(_m_USART1_R_LED_GPIOx,_m_USART1_W_LED_GPIO_Pin,1);
	}
	
	USART1_Send_Byte(SendData);
	

}

unsigned char Flash_LED_USART1_Get_Byte(unsigned char* GetData)
{
	if(_m_USART1_R_LED_GPIO_Pin)
	{
		Led_Auto_Off(_m_USART1_W_LED_GPIOx,_m_USART1_R_LED_GPIO_Pin,1);
	}
	
	return USART1_Get_Byte(GetData);
	

}



void Flash_LED_USART1_Config(TIM_TypeDef* USART1_Callback_TIMx,
		GPIO_TypeDef *USART1_Tx_GPIOx,uint16_t USART1_Tx_GPIO_Pin,
		GPIO_TypeDef *USART1_Rx_GPIOx,uint16_t USART1_Rx_GPIO_Pin,
		TIM_TypeDef* LED_TIMx,
		GPIO_TypeDef *USART1_R_LED_GPIOx,uint16_t USART1_R_LED_GPIO_Pin,
		GPIO_TypeDef *USART1_W_LED_GPIOx,uint16_t USART1_W_LED_GPIO_Pin)
{
	_m_USART1_R_LED_GPIOx = USART1_R_LED_GPIOx;
	_m_USART1_R_LED_GPIO_Pin = USART1_R_LED_GPIO_Pin;
	_m_USART1_W_LED_GPIOx = USART1_W_LED_GPIOx;
	_m_USART1_W_LED_GPIO_Pin = USART1_W_LED_GPIO_Pin;
	
	
	USART1_Config(USART1_Tx_GPIOx,USART1_Tx_GPIO_Pin,USART1_Rx_GPIOx,USART1_Rx_GPIO_Pin); //USART1 ≈‰÷√ 		
	
	if(_m_USART1_R_LED_GPIO_Pin != 0)
	{
		Flash_Led_Config(LED_TIMx,_m_USART1_R_LED_GPIOx,_m_USART1_R_LED_GPIO_Pin); 
	}
	if(_m_USART1_W_LED_GPIO_Pin != 0)
	{
		Flash_Led_Config(LED_TIMx,_m_USART1_W_LED_GPIOx,_m_USART1_W_LED_GPIO_Pin); 
	}
		

}


