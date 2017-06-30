
#include "usart1_test.h"
#include "usart1_util.h"

unsigned long _m_USART1_Received_Count = 0;

void USART1_Resend_On_Error(unsigned char c)
{
	

	if(USART1_Received_To_Buffer(c) == 0)
	{
		Flash_LED_USART1_Send_Byte(0xFF);
		Flash_LED_USART1_Send_Byte(0xFF);
		Flash_LED_USART1_Send_Byte(c);
		Flash_LED_USART1_Send_Byte(0xFF);
		Flash_LED_USART1_Send_Byte(0xFF);
		Flash_LED_USART1_Send_Byte(0xFF);
		Flash_LED_USART1_Send_Byte(0xFF);
		Flash_LED_USART1_Send_Byte(0xFF);
	}
	

}

void USART1_Send_Buffer_Data(void)
{
	
	int i = 0;

	unsigned char buff[USART1_Data_Node_Char_Len];
	
	
	while(1)
	{
		i = USART1_Read_To_Buffer(buff);
		//没有收据则退出
		if(i == 0)
		{
			break;
		}
		

		for(i = 0;i< USART1_Data_Node_Char_Len;i++)
		{
			Flash_LED_USART1_Send_Byte(buff[i]);
		}

		
	}
	
	
}



void USART1_Received_Count(unsigned char c)
{

	_m_USART1_Received_Count ++;
	if(_m_USART1_Received_Count == 0xFFFEUL)
	{
		_m_USART1_Received_Count = 0;
	}
	 
}


void Debug_USART1_Config(TIM_TypeDef* USART1_Callback_TIMx,
		GPIO_TypeDef *USART1_Tx_GPIOx,uint16_t USART1_Tx_GPIO_Pin,
		GPIO_TypeDef *USART1_Rx_GPIOx,uint16_t USART1_Rx_GPIO_Pin,
		TIM_TypeDef* LED_TIMx,
		GPIO_TypeDef *USART1_R_LED_GPIOx,uint16_t USART1_R_LED_GPIO_Pin,
		GPIO_TypeDef *USART1_W_LED_GPIOx,uint16_t USART1_W_LED_GPIO_Pin
		)
{

		Flash_LED_USART1_Config(USART1_Callback_TIMx,
			USART1_Tx_GPIOx,USART1_Tx_GPIO_Pin,
			USART1_Rx_GPIOx,USART1_Rx_GPIO_Pin,
			LED_TIMx,
			USART1_R_LED_GPIOx,USART1_R_LED_GPIO_Pin,
			USART1_W_LED_GPIOx,USART1_W_LED_GPIO_Pin);

		Register_USART1_Callback(USART1_Resend_On_Error);
		
		Register_USART1_Callback(USART1_Received_Count);
		
		Register_TIMx_Callback(USART1_Callback_TIMx,USART1_Send_Buffer_Data);

}


void usart1_test(void)
{
	
	TIM_TypeDef* USART1_Callback_TIMx = TIM3;
	
	GPIO_TypeDef *USART1_Tx_GPIOx = GPIOA;
	uint16_t UART1_Tx_GPIO_Pin = GPIO_Pin_9;

	GPIO_TypeDef *USART1_Rx_GPIOx = GPIOA;
	uint16_t UART1_Rx_GPIO_Pin = GPIO_Pin_10;

	TIM_TypeDef* LED_TIMx = TIM4;
	
	GPIO_TypeDef *USART1_R_LED_GPIOx = GPIOB;
	uint16_t USART1_R_LED_GPIO_Pin = GPIO_Pin_11;

	GPIO_TypeDef *USART1_W_LED_GPIOx = GPIOB;
	uint16_t USART1_W_LED_GPIO_Pin = GPIO_Pin_12;

	
	TIMx_With_NVIC_Config(USART1_Callback_TIMx,7199,99,NVIC_PriorityGroup_2,1,0);  
	TIMx_With_NVIC_Config(LED_TIMx,7199,99,NVIC_PriorityGroup_2,2,0); 
	
	Flash_Led_Config(TIM4,GPIOB,USART1_R_LED_GPIO_Pin | USART1_W_LED_GPIO_Pin ); 
	
	Debug_USART1_Config(USART1_Callback_TIMx,
		USART1_Tx_GPIOx,UART1_Tx_GPIO_Pin,
		USART1_Rx_GPIOx,UART1_Rx_GPIO_Pin,
		LED_TIMx,
		USART1_R_LED_GPIOx,USART1_R_LED_GPIO_Pin,
		USART1_W_LED_GPIOx,USART1_W_LED_GPIO_Pin
		); //USART1 配置 		
	
}

