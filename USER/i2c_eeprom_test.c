
#include "i2c_eeprom_test.h"



uint8_t _m_Read_EE_Chip_Address = 0xA1;
uint8_t _m_Write_EE_Chip_Address = 0xA0;

/** 串口的回调配置 **/
TIM_TypeDef* _m_I2C_EE_USART_Callback_TIMx = TIM3;
/** 串口1的配置 **/
uint32_t _m_I2C_EE_USART1_BaudRate = 115200;	
GPIO_TypeDef *_m_I2C_EE_USART1_Tx_GPIOx = GPIOA;
uint16_t _m_I2C_EE_USART1_Tx_GPIO_Pin = GPIO_Pin_9;
GPIO_TypeDef *_m_I2C_EE_USART1_Rx_GPIOx = GPIOA;
uint16_t _m_I2C_EE_USART1_Rx_GPIO_Pin = GPIO_Pin_10;
/** 串口1的数据接收时缓存配置 **/
#define I2C_EE_USART1_Data_Node_Len 4 //链表中缓存节点的个数
#define I2C_EE_USART1_Data_Node_Char_Len 8 //一个节点里面字节长度
unsigned char _m_I2C_EE_USART1_Data_Eof[] = {0xEF,0xFF};
unsigned short int _m_I2C_EE_USART1_Eof_Len;
struct Linked_List_Data _m_I2C_EE_USART1_Data;
struct Linked_List_Node _m_I2C_EE_USART1_Data_Nodes[ I2C_EE_USART1_Data_Node_Len ];
unsigned char _m_I2C_EE_USART1_Data_Node_Chars[ I2C_EE_USART1_Data_Node_Len * I2C_EE_USART1_Data_Node_Char_Len ];
/** 串口1的数据接收时缓存配置 **/



struct I2Cx_Conf EE_I2C1_Conf;


void EE_USART1_Recieve_Processor(void)
{

	unsigned int USART1_Read_Len = 0;
	uint16_t sub_address;
	uint8_t date;
	unsigned char USART1_Buff[I2C_EE_USART1_Data_Node_Char_Len];
	
	while(1)
	{
		USART1_Read_Len = USART_Read_From_Buffer(&_m_I2C_EE_USART1_Data,USART1_Buff);

		//没有数据则退出
		if(USART1_Read_Len == 0)
		{
			break;
		}
		
		
		if(USART1_Buff[0] == 0x01)
		{
			sub_address = USART1_Buff[1] * 256 + USART1_Buff[2];
			if(I2Cx_EE_ReadByte(&EE_I2C1_Conf,_m_Read_EE_Chip_Address,sub_address,&date))
			{
				USART_Send_Byte(USART1,date);
			}
			else
			{
				USART_Send_Byte(USART1,0xFF);
				USART_Send_Byte(USART1,0xFF);
				USART_Send_Byte(USART1,I2Cx_EE_Error_Code());
			}
		}
		else if(USART1_Buff[0] == 0x02)
		{
			sub_address = USART1_Buff[1] * 256 + USART1_Buff[2];	
			if(I2Cx_EE_WriteByte(&EE_I2C1_Conf,_m_Write_EE_Chip_Address,sub_address,USART1_Buff[3]))
			{
				USART_Send_Byte(USART1,0x00);
			}
			else
			{
				USART_Send_Byte(USART1,0xFF);
				USART_Send_Byte(USART1,0xFF);
				USART_Send_Byte(USART1,I2Cx_EE_Error_Code());
			}
		}
		else if(USART1_Buff[0] == 0xA0)
		{
			_m_Read_EE_Chip_Address = USART1_Buff[1];	
			USART_Send_Byte(USART1,0x00);
		}
		else if(USART1_Buff[0] == 0xA1)
		{
			_m_Write_EE_Chip_Address = USART1_Buff[1];	
			USART_Send_Byte(USART1,0x00);
		}
		else
		{
			USART_Send_Byte(USART1,0xEF);
			USART_Send_Byte(USART1,0xFF);
		}
		
	}
	
	
}


void I2C_EE_USART_Received_Data(USART_TypeDef* USARTx,unsigned char c)
{
	if(USARTx == USART1)
	{
		USART_Received_To_Buffer_Ignore_Error(&_m_I2C_EE_USART1_Data,c);
	}
	
}


void i2c_eeprom_test(void)
{
	
	
	TIMx_With_NVIC_Config(_m_I2C_EE_USART_Callback_TIMx,7199,99,NVIC_PriorityGroup_2,1,1); 
	
	USART_Config(USART1,_m_I2C_EE_USART1_BaudRate,
		_m_I2C_EE_USART1_Tx_GPIOx,_m_I2C_EE_USART1_Tx_GPIO_Pin,_m_I2C_EE_USART1_Rx_GPIOx,_m_I2C_EE_USART1_Rx_GPIO_Pin); //USART1 配置 
	
	I2C1_Conf_Init(&EE_I2C1_Conf);
	
	Register_USART_Callback(USART1,I2C_EE_USART_Received_Data);
	
	Register_TIMx_Callback(_m_I2C_EE_USART_Callback_TIMx,EE_USART1_Recieve_Processor);
	
	_m_I2C_EE_USART1_Eof_Len = sizeof(_m_I2C_EE_USART1_Data_Eof) / sizeof(_m_I2C_EE_USART1_Data_Eof[0]);
	USART_Data_Init(&_m_I2C_EE_USART1_Data,
		_m_I2C_EE_USART1_Data_Nodes,
		I2C_EE_USART1_Data_Node_Len,
		_m_I2C_EE_USART1_Data_Node_Chars,
		I2C_EE_USART1_Data_Node_Char_Len,
		_m_I2C_EE_USART1_Data_Eof,
		_m_I2C_EE_USART1_Eof_Len);
}


