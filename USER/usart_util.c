
#include "usart_util.h"
#include "flash_led.h"

GPIO_TypeDef *_m_USART_R_LED_GPIOx;
GPIO_TypeDef *_m_USART_W_LED_GPIOx;
uint16_t _m_USART_R_LED_GPIO_Pin = 0;
uint16_t _m_USART_W_LED_GPIO_Pin = 0;






void Flash_LED_USART_Send_Byte(USART_TypeDef* USARTx,unsigned char SendData)
{
	
	if(_m_USART_W_LED_GPIO_Pin)
	{
		Led_Auto_Off(_m_USART_R_LED_GPIOx,_m_USART_W_LED_GPIO_Pin,1);
	}
	
	USART_Send_Byte(USARTx,SendData);
	

}

unsigned char Flash_LED_USART_Get_Byte(USART_TypeDef* USARTx,unsigned char* GetData)
{
	if(_m_USART_R_LED_GPIO_Pin)
	{
		Led_Auto_Off(_m_USART_W_LED_GPIOx,_m_USART_R_LED_GPIO_Pin,1);
	}
	
	return USART_Get_Byte(USARTx,GetData);
	

}



void Flash_LED_USART_Config(TIM_TypeDef* USARTx_Callback_TIMx,
		USART_TypeDef *USARTx,uint32_t USART_BaudRate,
		GPIO_TypeDef *USARTx_Tx_GPIOx,uint16_t USARTx_Tx_GPIO_Pin,
		GPIO_TypeDef *USARTx_Rx_GPIOx,uint16_t USARTx_Rx_GPIO_Pin,
		TIM_TypeDef* LED_TIMx,
		GPIO_TypeDef *USART_R_LED_GPIOx,uint16_t USART_R_LED_GPIO_Pin,
		GPIO_TypeDef *USART_W_LED_GPIOx,uint16_t USART_W_LED_GPIO_Pin)
{
	_m_USART_R_LED_GPIOx = USART_R_LED_GPIOx;
	_m_USART_R_LED_GPIO_Pin = USART_R_LED_GPIO_Pin;
	_m_USART_W_LED_GPIOx = USART_W_LED_GPIOx;
	_m_USART_W_LED_GPIO_Pin = USART_W_LED_GPIO_Pin;
	
	
	USART_Config(USARTx,USART_BaudRate,
		USARTx_Tx_GPIOx,USARTx_Tx_GPIO_Pin,USARTx_Rx_GPIOx,USARTx_Rx_GPIO_Pin); //USART1 ≈‰÷√ 		
	
	if(_m_USART_R_LED_GPIO_Pin != 0)
	{
		Flash_Led_Config(LED_TIMx,_m_USART_R_LED_GPIOx,_m_USART_R_LED_GPIO_Pin); 
	}
	if(_m_USART_W_LED_GPIO_Pin != 0)
	{
		Flash_Led_Config(LED_TIMx,_m_USART_W_LED_GPIOx,_m_USART_W_LED_GPIO_Pin); 
	}
		

}


