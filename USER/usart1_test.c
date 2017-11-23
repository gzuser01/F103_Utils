
#include "usart1_test.h"


unsigned long _m_USART1_Received_Count = 0;
unsigned long _m_USART2_Received_Count = 0;

uint16_t _m_USART1_Prompt_Counter  = 0;
uint16_t _m_USART2_Prompt_Counter  = 0;

/** LED的配置 **/
TIM_TypeDef* _m_LED_TIMx = TIM4;
GPIO_TypeDef *_m_USART_R_LED_GPIOx = GPIOB;
uint16_t _m_USART_R_LED_GPIO_Pin = GPIO_Pin_11;
GPIO_TypeDef *_m_USART_E_LED_GPIOx = GPIOB;
uint16_t _m_USART_E_LED_GPIO_Pin = GPIO_Pin_12;
/** LED的配置 **/

/** 串口的回调配置 **/
TIM_TypeDef* _m_USART_Callback_TIMx = TIM3;
/** 串口1的配置 **/
uint32_t _m_USART1_BaudRate = 115200;	
GPIO_TypeDef *_m_USART1_Tx_GPIOx = GPIOA;
uint16_t _m_USART1_Tx_GPIO_Pin = GPIO_Pin_9;
GPIO_TypeDef *_m_USART1_Rx_GPIOx = GPIOA;
uint16_t _m_USART1_Rx_GPIO_Pin = GPIO_Pin_10;
/** 串口1的数据接收时缓存配置 **/
#define USART1_Data_Node_Len 4 //链表中缓存节点的个数
#define USART1_Data_Node_Char_Len 8 //一个节点里面字节长度
unsigned char _m_USART1_Data_Eof[] = {0xEF,0xFF};
unsigned short int _m_USART1_Eof_Len;
struct Linked_List_Data _m_USART1_Data;
struct Linked_List_Node _m_USART1_Data_Nodes[ USART1_Data_Node_Len ];
unsigned char _m_USART1_Data_Node_Chars[ USART1_Data_Node_Len * USART1_Data_Node_Char_Len ];
/** 串口1的数据接收时缓存配置 **/

/** 串口2的配置 **/
uint32_t _m_USART2_BaudRate = 115200;	
GPIO_TypeDef *_m_USART2_Tx_GPIOx = GPIOA;
uint16_t _m_USART2_Tx_GPIO_Pin = GPIO_Pin_2;
GPIO_TypeDef *_m_USART2_Rx_GPIOx = GPIOA;
uint16_t _m_USART2_Rx_GPIO_Pin = GPIO_Pin_3;
/** 串口2的数据接收时缓存配置 **/
#define USART2_Data_Node_Len 4 //链表中缓存节点的个数
#define USART2_Data_Node_Char_Len 8 //一个节点里面字节长度
unsigned char _m_USART2_Data_Eof[] = {0xEF,0xFF};
unsigned short int _m_USART2_Eof_Len;
struct Linked_List_Data _m_USART2_Data;
struct Linked_List_Node _m_USART2_Data_Nodes[ USART2_Data_Node_Len ];
unsigned char _m_USART2_Data_Node_Chars[ USART2_Data_Node_Len * USART2_Data_Node_Char_Len ];
/** 串口2的数据接收时缓存配置 **/


void USART_Received_Data(USART_TypeDef* USARTx,unsigned char c)
{
	

	if((USARTx == USART1 && USART_Received_To_Buffer(&_m_USART1_Data,c) == 0) ||
		(USARTx == USART2 && USART_Received_To_Buffer(&_m_USART2_Data,c) == 0)
	)
	{
		USART_Send_Byte(USARTx,0xFF);
		USART_Send_Byte(USARTx,0xFF);
		USART_Send_Byte(USARTx,c);
		USART_Send_Byte(USARTx,0xFF);
		USART_Send_Byte(USARTx,0xFF);
		USART_Send_Byte(USARTx,0xFF);
		USART_Send_Byte(USARTx,0xFF);
		USART_Send_Byte(USARTx,0xFF);
	}
	
}

/**
 * 会被 _m_USART_Callback_TIMx 定义的时钟回调
 */
void USART_Send_Buffer_Data(void)
{
	unsigned int i = 0;
	unsigned int USART1_Read_Len = 0;
	unsigned int USART2_Read_Len = 0;
	
	unsigned char USART1_Buff[USART1_Data_Node_Char_Len];
	unsigned char USART2_Buff[USART2_Data_Node_Char_Len];
	
	
	while(1)
	{
		
		USART1_Read_Len = USART_Read_From_Buffer(&_m_USART1_Data,USART1_Buff);
		if(USART1_Read_Len == 0)//USART1没有收到
		{
			_m_USART1_Prompt_Counter = _m_USART1_Prompt_Counter + 1;
			if(_m_USART1_Prompt_Counter > 500)
			{
				//Led_Auto_Off(_m_USART_E_LED_GPIOx,_m_USART_E_LED_GPIO_Pin,1);
				USART_Send_Byte(USART1,'u');
				USART_Send_Byte(USART1,'s');
				USART_Send_Byte(USART1,'a');
				USART_Send_Byte(USART1,'r');
				USART_Send_Byte(USART1,'t');
				USART_Send_Byte(USART1,'1');
				_m_USART1_Prompt_Counter = 0;
			}
		}else{//USART1收到回显
			for(i = 0;i < USART1_Data_Node_Char_Len;i++)
			{
				Led_Auto_Off(_m_USART_R_LED_GPIOx,_m_USART_R_LED_GPIO_Pin,1);
				USART_Send_Byte(USART1,USART1_Buff[i]);
			}
			_m_USART1_Prompt_Counter = 0;
		}
		/** ================================== */
		USART2_Read_Len = USART_Read_From_Buffer(&_m_USART2_Data,USART2_Buff);
		if(USART2_Read_Len == 0)//USART2没有收到
		{
			_m_USART2_Prompt_Counter = _m_USART2_Prompt_Counter + 1;
			if(_m_USART2_Prompt_Counter > 500)
			{
				//Led_Auto_Off(_m_USART_R_LED_GPIOx,_m_USART_R_LED_GPIO_Pin,1);
				USART_Send_Byte(USART2,'u');
				USART_Send_Byte(USART2,'s');
				USART_Send_Byte(USART2,'a');
				USART_Send_Byte(USART2,'r');
				USART_Send_Byte(USART2,'t');
				USART_Send_Byte(USART2,'2');
				_m_USART2_Prompt_Counter = 0;
			}
		}else{//USART2收到回显
			for(i = 0;i < USART2_Data_Node_Char_Len;i++)
			{
				Led_Auto_Off(_m_USART_R_LED_GPIOx,_m_USART_R_LED_GPIO_Pin,1);
				USART_Send_Byte(USART2,USART2_Buff[i]);
			}
			_m_USART2_Prompt_Counter = 0;
		}
		
		
		//都没有待处理的数据,退出当前循环,等待下一次回调
		if(USART1_Read_Len == 0 && USART2_Read_Len == 0)
		{
			break;
		}
		
	}
	
	
}



void USART_Received_Count(USART_TypeDef* USARTx,unsigned char c)
{
	if(USARTx == USART1)
	{
		_m_USART1_Received_Count ++;
		if(_m_USART1_Received_Count == 0xFFFEUL)
		{
			_m_USART1_Received_Count = 0;
		}
	}
	if(USARTx == USART2)
	{
		_m_USART2_Received_Count ++;
		if(_m_USART2_Received_Count == 0xFFFEUL)
		{
			_m_USART2_Received_Count = 0;
		}
	}
	 
}




void usart1_test(void)
{
	
	
	TIMx_With_NVIC_Config(_m_USART_Callback_TIMx,7199,99,NVIC_PriorityGroup_2,1,0);  
	TIMx_With_NVIC_Config(_m_LED_TIMx,7199,99,NVIC_PriorityGroup_2,2,0); 
	
	Register_USART_Callback(USART1,USART_Received_Data);
	Register_USART_Callback(USART2,USART_Received_Data);
		
	Register_USART_Callback(USART1,USART_Received_Count);
	Register_USART_Callback(USART2,USART_Received_Count);
		
	Register_TIMx_Callback(_m_USART_Callback_TIMx,USART_Send_Buffer_Data);
	

	_m_USART1_Eof_Len = sizeof(_m_USART1_Data_Eof) / sizeof(_m_USART1_Data_Eof[0]);
	_m_USART2_Eof_Len = sizeof(_m_USART2_Data_Eof) / sizeof(_m_USART2_Data_Eof[0]);

	USART_Data_Init(&_m_USART1_Data,
		_m_USART1_Data_Nodes,
		USART1_Data_Node_Len,
		_m_USART1_Data_Node_Chars,
		USART1_Data_Node_Char_Len,
		_m_USART1_Data_Eof,
		_m_USART1_Eof_Len);
	USART_Data_Init(&_m_USART2_Data,
		_m_USART2_Data_Nodes,
		USART2_Data_Node_Len,
		_m_USART2_Data_Node_Chars,
		USART2_Data_Node_Char_Len,
		_m_USART2_Data_Eof,
		_m_USART2_Eof_Len);

	Flash_Led_Config(_m_LED_TIMx,_m_USART_R_LED_GPIOx,_m_USART_R_LED_GPIO_Pin); 
	Flash_Led_Config(_m_LED_TIMx,_m_USART_E_LED_GPIOx,_m_USART_E_LED_GPIO_Pin); 
	
	
	USART_Config(USART1,_m_USART1_BaudRate,
		_m_USART1_Tx_GPIOx,_m_USART1_Tx_GPIO_Pin,_m_USART1_Rx_GPIOx,_m_USART1_Rx_GPIO_Pin); //USART1 配置 	

	USART_Config(USART2,_m_USART2_BaudRate,
		_m_USART2_Tx_GPIOx,_m_USART2_Tx_GPIO_Pin,_m_USART2_Rx_GPIOx,_m_USART2_Rx_GPIO_Pin); //USART2 配置 

	
}

