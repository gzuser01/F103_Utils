#ifndef __DS3231_TEST_H
#define	__DS3231_TEST_H

#include "i2c.h"
#include "usart.h"
#include "usart_data.h"
#include "timx.h"


/** 串口的回调配置 **/
TIM_TypeDef* _m_DS3231_USART_Callback_TIMx = TIM3;
/** 串口1的配置 **/
uint32_t _m_DS3231_USART1_BaudRate = 115200;	
GPIO_TypeDef *_m_DS3231_USART1_Tx_GPIOx = GPIOA;
uint16_t _m_DS3231_USART1_Tx_GPIO_Pin = GPIO_Pin_9;
GPIO_TypeDef *_m_DS3231_USART1_Rx_GPIOx = GPIOA;
uint16_t _m_DS3231_USART1_Rx_GPIO_Pin = GPIO_Pin_10;
/** 串口1的数据接收时缓存配置 **/
#define DS3231_USART1_Data_Node_Len 4 //链表中缓存节点的个数
#define DS3231_USART1_Data_Node_Char_Len 8 //一个节点里面字节长度
unsigned char _m_DS3231_USART1_Data_Eof[] = {0xEF,0xFF};
unsigned short int _m_DS3231_USART1_Eof_Len;
struct Linked_List_Data _m_DS3231_USART1_Data;
struct Linked_List_Node _m_DS3231_USART1_Data_Nodes[ DS3231_USART1_Data_Node_Len ];
unsigned char _m_DS3231_USART1_Data_Node_Chars[ DS3231_USART1_Data_Node_Len * DS3231_USART1_Data_Node_Char_Len ];
/** 串口1的数据接收时缓存配置 **/


//T(sec)(min)(hour)(dayOfWeek)(dayOfMonth)(month)(year)
//T(00-59)(00-59)(00-23)(1-7)(01-31)(01-12)(00-99)

uint8_t _m_DS3231_Chip_Address = 0xD0;
uint8_t _m_DS3231_Sub_Address = 0x00;

int8_t _m_DS3231_I2C_Error_Code;

struct I2Cx_Conf DS3231_I2C1_Conf;

u8 bcd_to_dec(u8 bcdByte) {
  return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}

u8 dec_to_bcd(u8 decByte) {
  return (((decByte / 10) << 4) | (decByte % 10));
}

uint16_t dayOfWeek(uint16_t year, uint16_t month, uint16_t day) {
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  year -= month < 3;
  return ((year + year/4 - year/100 + year/400 + t[month-1] + day) % 7); 
}


uint8_t DS3231_Init(uint8_t chip_address)
{
	uint16_t i;
	
	_m_DS3231_I2C_Error_Code = 0x00;
	
	while (I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)); // Wait until I2C free
	
	I2C_GenerateSTART(I2C1,ENABLE);
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT);i++) // Wait for EV5
	{
		if(i> TRY_TIMES)
		{
			_m_DS3231_I2C_Error_Code = 0xA0;
			return 0;
		}
	}
	
	I2C_Send7bitAddress(I2C1,chip_address,I2C_Direction_Transmitter); // Send DS3231 slave address
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);i++) // Wait for EV6
	{
		if(i> TRY_TIMES)
		{
			_m_DS3231_I2C_Error_Code = 0xA1;
			return 0;
		}
	}
	
	I2C_SendData(I2C1,0x0E); // Send DS3231 control register address
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);i++) // Wait for EV8
	{
		if(i> TRY_TIMES)
		{
			_m_DS3231_I2C_Error_Code = 0xA2;
			return 0;
		}
	}
	
	I2C_SendData(I2C1,0x00); // DS3231 EOSC enabled, INTCN enabled, SQW set to 1Hz
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);i++) // Wait for EV8
	{
		if(i> TRY_TIMES)
		{
			_m_DS3231_I2C_Error_Code = 0xA3;
			return 0;
		}
	}
	
	
	I2C_SendData(I2C1,0x00); // DS3231 clear alarm flags
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);i++) // Wait for EV8
	{
		if(i> TRY_TIMES)
		{
			_m_DS3231_I2C_Error_Code = 0xA4;
			return 0;
		}
	}
		
	I2C_GenerateSTOP(I2C1,ENABLE);
	return 1;
	
}



uint8_t DS3231_Read(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t chip_address, uint8_t sub_address,uint8_t* data)
{
	_m_DS3231_I2C_Error_Code = 0x00;
	
	I2C1_Conf_Init(I2Cx_Conf_Struct);

	I2Cx_Ack_Enable(I2Cx_Conf_Struct);
	
	if(!I2Cx_Wait(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x01;
		//return 0;
	}
	if(!I2Cx_Start(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x02;
		//return 0;
	}
	if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,chip_address, I2C_Direction_Transmitter))
	{
		_m_DS3231_I2C_Error_Code = 0x03;
		return 0;
	}
	
	if(!I2Cx_Transmit(I2Cx_Conf_Struct, sub_address))
	{
		_m_DS3231_I2C_Error_Code = 0x04;
		return 0;
	}
	if(!I2Cx_Start(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x05;
		return 0;
	}
	if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,chip_address, I2C_Direction_Receiver))
	{
		_m_DS3231_I2C_Error_Code = 0x06;
		return 0;
	}
	if(!I2Cx_Receives(I2Cx_Conf_Struct,data,7))
	{
		_m_DS3231_I2C_Error_Code = 0x07;
		return 0;
	}
	
	I2Cx_Ack_Disable(I2Cx_Conf_Struct);
		
	if(!I2Cx_Stop(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x09;
		return 0;
	}

	return 1;
	
}


uint8_t DS3231_Write(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t chip_address, uint8_t sub_address, uint8_t *data)
{
	uint8_t i;
	
	_m_DS3231_I2C_Error_Code = 0x00;
	
	I2C1_Conf_Init(I2Cx_Conf_Struct);

	I2Cx_Ack_Enable(I2Cx_Conf_Struct);
	
	if(!I2Cx_Wait(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x11;
		return 0;
	}
	if(!I2Cx_Start(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x12;
		return 0;
	}
	
	if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,chip_address, I2C_Direction_Transmitter))
	{
		_m_DS3231_I2C_Error_Code = 0x13;
		return 0;
	}
	
	if(!I2Cx_Transmit(I2Cx_Conf_Struct, sub_address))
	{
		_m_DS3231_I2C_Error_Code = 0x14;
		return 0;
	}
	
	for (i = 0; i < 7; i++) 
	{
		if(!I2Cx_Transmit(I2Cx_Conf_Struct, data[i]))
		{
			_m_DS3231_I2C_Error_Code = 0x15;
			return 0;
		}
	}
	
	I2Cx_Ack_Disable(I2Cx_Conf_Struct);
	
	if(!I2Cx_Stop(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x16;
		return 0;
	}
	return 1;
}


int8_t DS3231_I2C_Error_Code(void)
{
	return _m_DS3231_I2C_Error_Code;
}


/**
 * 发送 01 或 02 则返回时间，01 是 bcd ，02 是 dec，默认 dec
 * 发送 03 或 04 开头设置时间，格式 02 秒 分 时 dow 日 月 年，dow自动计算填入 00 即可，03 是 bcd ，04 是 dec
 * 设置成功返回时间
 * 查询或者设置失败返回 FF FF 错误码
 */

void DS3231_USART1_Recieve_Processor(void)
{
	
	int i = 0;
	
	int16_t year = 0;
	int16_t month = 0;
	int16_t day = 0;

	uint8_t data[7] = {0,0,0,0,0,0,0};
	
	unsigned int USART1_Read_Len = 0;
	unsigned char USART1_Buff[DS3231_USART1_Data_Node_Char_Len];
	
	while(1)
	{
		USART1_Read_Len = USART_Read_From_Buffer(&_m_DS3231_USART1_Data,USART1_Buff);

		//没有数据则退出
		if(USART1_Read_Len == 0)
		{
			break;
		}


		if(USART1_Buff[0] == 0x03 || USART1_Buff[0] == 0x04)
		{
			memcpy(data,USART1_Buff + 1,7);
			
			if(USART1_Buff[0] == 0x04)
			{
				for(i=0;i<7;i++)
				{
					data[i] = dec_to_bcd(data[i]);
				}
			}
			
			year = 2000 + bcd_to_dec(data[6]);
			month = bcd_to_dec(data[5]);
			day = bcd_to_dec(data[4]);
			
			data[3] = dayOfWeek(year,month,day);
			
			DS3231_Write(&DS3231_I2C1_Conf,_m_DS3231_Chip_Address,_m_DS3231_Sub_Address,data);
		}
		

		if(DS3231_Read(&DS3231_I2C1_Conf,_m_DS3231_Chip_Address,_m_DS3231_Sub_Address,data))
		{
			for(i=0;i<7;i++)
			{
				if(USART1_Buff[0] != 0x01 && USART1_Buff[0] != 0x03)
				{
					data[i] = bcd_to_dec(data[i]);
				}
				USART_Send_Byte(USART1,data[i]);					
			}
			
				
			if(USART1_Buff[0] != 0x01 && USART1_Buff[0] != 0x03)
			{
				USART_Send_Byte(USART1,'\n');
			}
		}
		
		
		if(DS3231_I2C_Error_Code() != 0x00)
		{
			USART_Send_Byte(USART1,0xFF);
			USART_Send_Byte(USART1,0xFF);
			USART_Send_Byte(USART1,DS3231_I2C_Error_Code());
		}

	}
	
	
}

void DS3231_USART_Received_Data(USART_TypeDef* USARTx,unsigned char c)
{
	if(USARTx == USART1)
	{
		USART_Received_To_Buffer_Ignore_Error(&_m_DS3231_USART1_Data,c);
	}
	
}



void ds3231_test(void)
{
	
	
	
	TIMx_With_NVIC_Config(_m_DS3231_USART_Callback_TIMx,7199,99,NVIC_PriorityGroup_0,0,0); 
	
		USART_Config(USART1,_m_DS3231_USART1_BaudRate,
		_m_DS3231_USART1_Tx_GPIOx,_m_DS3231_USART1_Tx_GPIO_Pin,_m_DS3231_USART1_Rx_GPIOx,_m_DS3231_USART1_Rx_GPIO_Pin); //USART1 配置 	
	
	I2C1_Conf_Init(&DS3231_I2C1_Conf);
	
	Register_USART_Callback(USART1,DS3231_USART_Received_Data);
	
	Register_TIMx_Callback(_m_DS3231_USART_Callback_TIMx,DS3231_USART1_Recieve_Processor);
	
	_m_DS3231_USART1_Eof_Len = sizeof(_m_DS3231_USART1_Data_Eof) / sizeof(_m_DS3231_USART1_Data_Eof[0]);

	USART_Data_Init(&_m_DS3231_USART1_Data,
		_m_DS3231_USART1_Data_Nodes,
		DS3231_USART1_Data_Node_Len,
		_m_DS3231_USART1_Data_Node_Chars,
		DS3231_USART1_Data_Node_Char_Len,
		_m_DS3231_USART1_Data_Eof,
		_m_DS3231_USART1_Eof_Len);
	
	DS3231_Init(_m_DS3231_Chip_Address);
	if(DS3231_I2C_Error_Code() != 0x00)
	{
		USART_Send_Byte(USART1,0xFF);
		USART_Send_Byte(USART1,0xFF);
		USART_Send_Byte(USART1,DS3231_I2C_Error_Code());
	}
	else
	{
		USART_Send_Byte(USART1,0xFF);
		USART_Send_Byte(USART1,0xFF);
	}
	
	
	
	
}





#endif 
