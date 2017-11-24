#ifndef __LCD1602_TEST_H
#define	__LCD1602_TEST_H

#include "i2c.h"
#include "timx.h"
#include "usart.h"
#include "usart_data.h"

struct I2Cx_Conf LCD1602_I2C_Conf;

uint8_t _m_LCD1602_Chip_Address = 0x7F;

int8_t _m_LCD1602_I2C_Error_Code;

/** 串口的回调配置 **/
TIM_TypeDef* _m_LCD1602_USART_Callback_TIMx = TIM3;
/** 串口1的配置 **/
uint32_t _m_LCD1602_USART1_BaudRate = 115200;	
GPIO_TypeDef *_m_LCD1602_USART1_Tx_GPIOx = GPIOA;
uint16_t _m_LCD1602_USART1_Tx_GPIO_Pin = GPIO_Pin_9;
GPIO_TypeDef *_m_LCD1602_USART1_Rx_GPIOx = GPIOA;
uint16_t _m_LCD1602_USART1_Rx_GPIO_Pin = GPIO_Pin_10;
/** 串口1的数据接收时缓存配置 **/
#define LCD1602_USART1_Data_Node_Len 4 //链表中缓存节点的个数
#define LCD1602_USART1_Data_Node_Char_Len 8 //一个节点里面字节长度
unsigned char _m_LCD1602_USART1_Data_Eof[] = {0xEF,0xFF};
unsigned short int _m_LCD1602_USART1_Eof_Len;
struct Linked_List_Data _m_LCD1602_USART1_Data;
struct Linked_List_Node _m_LCD1602_USART1_Data_Nodes[ LCD1602_USART1_Data_Node_Len ];
unsigned char _m_LCD1602_USART1_Data_Node_Chars[ LCD1602_USART1_Data_Node_Len * LCD1602_USART1_Data_Node_Char_Len ];
/** 串口1的数据接收时缓存配置 **/

uint8_t LCD1602_Write(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t chip_address, uint8_t data)
{
	_m_LCD1602_I2C_Error_Code = 0x00;
	
	I2C2_Conf_Init(I2Cx_Conf_Struct);
	
	I2Cx_Ack_Enable(I2Cx_Conf_Struct);
	
	if(!I2Cx_Start(I2Cx_Conf_Struct))
	{
		_m_LCD1602_I2C_Error_Code = 0x12;
		return 0;
	}
	
	if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,chip_address, I2C_Direction_Transmitter))
	{
		_m_LCD1602_I2C_Error_Code = 0x13;
		return 0;
	}
	
	if(!I2Cx_Transmit(I2Cx_Conf_Struct, data))
	{
		_m_LCD1602_I2C_Error_Code = 0x15;
		return 0;
	}
	
	if(!I2Cx_Stop(I2Cx_Conf_Struct))
	{
		_m_LCD1602_I2C_Error_Code = 0x16;
		return 0;
	}
	return 1;
}


void LCD1602_PulseEnable(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t _data)
{
	LCD1602_Write(I2Cx_Conf_Struct,_m_LCD1602_Chip_Address,_data | 0x04 | 0x08);	// En high
	Delay_Nus(10);		// enable pulse must be >450ns

	LCD1602_Write(I2Cx_Conf_Struct,_m_LCD1602_Chip_Address,_data & ~0x04 | 0x08);	// En low
	Delay_Nus(50);		// commands need > 37us to settle
}

void LCD1602_Write4bits(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t value) 
{
	//LCD1602_Write(I2Cx_Conf_Struct,value);
	LCD1602_PulseEnable(I2Cx_Conf_Struct,value);
}


void LCD1602_Send(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t value, uint8_t mode)
{
	uint8_t highnib = value&0xf0;
	uint8_t lownib = (value<<4)&0xf0;
  LCD1602_Write4bits(I2Cx_Conf_Struct,(highnib)|mode);
	LCD1602_Write4bits(I2Cx_Conf_Struct,(lownib)|mode);
}

void LCD1602_Send_Char(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t c) 
{
	LCD1602_Send(I2Cx_Conf_Struct,c,0x01);
}

void LCD1602_Command(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t cmd)
{
	LCD1602_Send(I2Cx_Conf_Struct,cmd,0x00);
}
	


void LCD1602_SetCursor(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t col, uint8_t row){
	uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > 4 ) {
		row = 4-1;
	}
	LCD1602_Command(I2Cx_Conf_Struct,0x80 | (col + row_offsets[row]));
}

void LCD1602_USART1_Recieve_Processor(void)
{
	unsigned int USART1_Read_Len = 0;
	unsigned char USART1_Buff[LCD1602_USART1_Data_Node_Char_Len];
	
	while(1)
	{
		USART1_Read_Len = USART_Read_From_Buffer(&_m_LCD1602_USART1_Data,USART1_Buff);

		//没有数据则退出
		if(USART1_Read_Len == 0)
		{
			break;
		}
		
		
		
	}
	
	
}

void LCD1602_Flash(void)
{
	uint8_t i;
	for(i = 0;i<3;i++)
	{
		Delay_Nms(500);
		LCD1602_Write(&LCD1602_I2C_Conf,_m_LCD1602_Chip_Address,0x00);
		if(_m_LCD1602_I2C_Error_Code != 0x00)
		{
			USART_Send_Byte(USART1,0xEF);
			USART_Send_Byte(USART1,0x08);
			USART_Send_Byte(USART1,_m_LCD1602_I2C_Error_Code);
		}
		Delay_Nms(500);
		LCD1602_Write(&LCD1602_I2C_Conf,_m_LCD1602_Chip_Address,0x08);
		if(_m_LCD1602_I2C_Error_Code != 0x00)
		{
			USART_Send_Byte(USART1,0xEF);
			USART_Send_Byte(USART1,0x08);
			USART_Send_Byte(USART1,_m_LCD1602_I2C_Error_Code);
		}
	}
}

/*
void LCD1602_Write_Char(uint8_t c)
{
	LCD1602_Write(&LCD1602_I2C_Conf,_m_LCD1602_Chip_Address,0x01 | 0x08,(c&0xf0) | 0x08);
	LCD1602_Write(&LCD1602_I2C_Conf,_m_LCD1602_Chip_Address,0x01 | 0x08,((c<<4)&0xf0) | 0x08);
	
	
	if(_m_LCD1602_I2C_Error_Code != 0x00)
	{
		USART1_Send_Byte(0xEF);
		USART1_Send_Byte(0x01);
		USART1_Send_Byte(_m_LCD1602_I2C_Error_Code);
	}
}
*/

void LCD1602_USART_Received_Data(USART_TypeDef* USARTx,unsigned char c)
{
	if(USARTx == USART1)
	{
		USART_Received_To_Buffer_Ignore_Error(&_m_LCD1602_USART1_Data,c);
	}
	
}


void lcd1602_test(void)
{


	
	TIMx_With_NVIC_Config(_m_LCD1602_USART_Callback_TIMx,7199,99,NVIC_PriorityGroup_2,1,1); 
	
	USART_Config(USART1,_m_LCD1602_USART1_BaudRate,
		_m_LCD1602_USART1_Tx_GPIOx,_m_LCD1602_USART1_Tx_GPIO_Pin,_m_LCD1602_USART1_Rx_GPIOx,_m_LCD1602_USART1_Rx_GPIO_Pin); //USART1 配置 	
	
	Register_USART_Callback(USART1,LCD1602_USART_Received_Data);
	
	Register_TIMx_Callback(_m_LCD1602_USART_Callback_TIMx,LCD1602_USART1_Recieve_Processor);
	
	USART_Data_Init(&_m_LCD1602_USART1_Data,
		_m_LCD1602_USART1_Data_Nodes,
		LCD1602_USART1_Data_Node_Len,
		_m_LCD1602_USART1_Data_Node_Chars,
		LCD1602_USART1_Data_Node_Char_Len,
		_m_LCD1602_USART1_Data_Eof,
		_m_LCD1602_USART1_Eof_Len);
	

	
	I2C2_Conf_Init(&LCD1602_I2C_Conf);
	USART_Send_Byte(USART1,0xFF);
	USART_Send_Byte(USART1,0xFF);
	

	LCD1602_Flash();
	
	
	LCD1602_Command(&LCD1602_I2C_Conf,0x01);
	LCD1602_Command(&LCD1602_I2C_Conf,0x02);
	

	LCD1602_Send_Char(&LCD1602_I2C_Conf,'a');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'b');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'0');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'1');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'2');
	/*
	Delay_Nms(1000);
	LCD1602_Command(&LCD1602_I2C_Conf,0x01);
	LCD1602_Command(&LCD1602_I2C_Conf,0x02);
	
	LCD1602_SetCursor(&LCD1602_I2C_Conf,0,0);
	
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'x');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'y');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'z');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,':');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'+');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'=');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'0');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'1');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'2');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'3');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'4');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'5');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'6');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'7');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'8');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'9');
	
	LCD1602_SetCursor(&LCD1602_I2C_Conf,0,1);


	LCD1602_Send_Char(&LCD1602_I2C_Conf,'a');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'b');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'c');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'d');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'e');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'f');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'0');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'1');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'2');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'3');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'4');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'5');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'6');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'7');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'8');
	LCD1602_Send_Char(&LCD1602_I2C_Conf,'9');

	*/
}



#endif 


