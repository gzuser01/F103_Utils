
#include "usart1_util.h"
#include "flash_led.h"

GPIO_TypeDef *_m_USART1_R_LED_GPIOx;
GPIO_TypeDef *_m_USART1_W_LED_GPIOx;
uint16_t _m_USART1_R_LED_GPIO_Pin = 0;
uint16_t _m_USART1_W_LED_GPIO_Pin = 0;
unsigned short int _m_usart1_util_debug = 0;

unsigned long _m_usart1_received_count = 0;



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




void USART1_Received_Count(unsigned char c)
{

	_m_usart1_received_count ++;
	if(_m_usart1_received_count == 0xFFFEUL)
	{
		_m_usart1_received_count = 0;
	}
	 
}

void USART1_Resend_On_Error(unsigned char c)
{
	

	if(USART1_Received_To_Buffer(c) == 0 && _m_usart1_util_debug == 1)
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

	unsigned char buff[usart1_data_node_char_len];
	
	
	while(1)
	{
		i = USART1_Read_To_Buffer(buff);
		if(i == 0)
		{
			break;
		}
		

		for(i = 0;i< usart1_data_node_char_len;i++)
		{
			Flash_LED_USART1_Send_Byte(buff[i]);
		}

		
	}
	
	
}

void Flash_LED_USART1_Config(TIM_TypeDef* USART1_Callback_TIMx,
		GPIO_TypeDef *USART1_Tx_GPIOx,uint16_t USART1_Tx_GPIO_Pin,
		GPIO_TypeDef *USART1_Rx_GPIOx,uint16_t USART1_Rx_GPIO_Pin,
		TIM_TypeDef* LED_TIMx,
		GPIO_TypeDef *USART1_R_LED_GPIOx,uint16_t USART1_R_LED_GPIO_Pin,
		GPIO_TypeDef *USART1_W_LED_GPIOx,uint16_t USART1_W_LED_GPIO_Pin,
		unsigned short int debug)
{
	_m_usart1_util_debug = debug;
	_m_USART1_R_LED_GPIOx = USART1_R_LED_GPIOx;
	_m_USART1_R_LED_GPIO_Pin = USART1_R_LED_GPIO_Pin;
	_m_USART1_W_LED_GPIOx = USART1_W_LED_GPIOx;
	_m_USART1_W_LED_GPIO_Pin = USART1_W_LED_GPIO_Pin;
	
	
	USART1_Config(USART1_Tx_GPIOx,USART1_Tx_GPIO_Pin,USART1_Rx_GPIOx,USART1_Rx_GPIO_Pin); //USART1 ÅäÖÃ 		
	
	if(_m_USART1_R_LED_GPIO_Pin != 0)
	{
		Flash_Led_Config(LED_TIMx,_m_USART1_R_LED_GPIOx,_m_USART1_R_LED_GPIO_Pin); 
	}
	if(_m_USART1_W_LED_GPIO_Pin != 0)
	{
		Flash_Led_Config(LED_TIMx,_m_USART1_W_LED_GPIOx,_m_USART1_W_LED_GPIO_Pin); 
	}
		
	Register_USART1_Callback(USART1_Resend_On_Error);

	if(_m_usart1_util_debug == 1)
	{
		Register_USART1_Callback(USART1_Received_Count);
		
		Register_TIMx_Callback(USART1_Callback_TIMx,USART1_Send_Buffer_Data);
	}

	usart1_data_init();
}


