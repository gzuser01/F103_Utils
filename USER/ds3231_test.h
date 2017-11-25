#ifndef __DS3231_TEST_H
#define	__DS3231_TEST_H

#include "ds3231.h"
#include "usart.h"
#include "usart_data.h"
#include "timx.h"


/** ���ڵĻص����� **/
TIM_TypeDef* _m_DS3231_USART_Callback_TIMx = TIM3;
/** ����1������ **/
uint32_t _m_DS3231_USART1_BaudRate = 115200;	
GPIO_TypeDef *_m_DS3231_USART1_Tx_GPIOx = GPIOA;
uint16_t _m_DS3231_USART1_Tx_GPIO_Pin = GPIO_Pin_9;
GPIO_TypeDef *_m_DS3231_USART1_Rx_GPIOx = GPIOA;
uint16_t _m_DS3231_USART1_Rx_GPIO_Pin = GPIO_Pin_10;
/** ����1�����ݽ���ʱ�������� **/
#define DS3231_USART1_Data_Node_Len 4 //�����л���ڵ�ĸ���
#define DS3231_USART1_Data_Node_Char_Len 8 //һ���ڵ������ֽڳ���
unsigned char _m_DS3231_USART1_Data_Eof[] = {0xEF,0xFF};
unsigned short int _m_DS3231_USART1_Eof_Len;
struct Linked_List_Data _m_DS3231_USART1_Data;
struct Linked_List_Node _m_DS3231_USART1_Data_Nodes[ DS3231_USART1_Data_Node_Len ];
unsigned char _m_DS3231_USART1_Data_Node_Chars[ DS3231_USART1_Data_Node_Len * DS3231_USART1_Data_Node_Char_Len ];
/** ����1�����ݽ���ʱ�������� **/




/**
 * ���� 01 �� 02 �򷵻�ʱ�䣬01 �� bcd ��02 �� dec��Ĭ�� dec
 * ���� 03 �� 04 ��ͷ����ʱ�䣬��ʽ 02 �� �� ʱ dow �� �� �꣬dow�Զ��������� 00 ���ɣ�03 �� bcd ��04 �� dec
 * ���óɹ�����ʱ��
 * ��ѯ��������ʧ�ܷ��� FF FF ������
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

		//û���������˳�
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
		_m_DS3231_USART1_Tx_GPIOx,_m_DS3231_USART1_Tx_GPIO_Pin,_m_DS3231_USART1_Rx_GPIOx,_m_DS3231_USART1_Rx_GPIO_Pin); //USART1 ���� 	
	
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
	
	//�����ʼ�ɹ����ڴ�ӡ 01 02������ֻ�� 01
	USART_Send_Byte(USART1,0x01);
	
	DS3231_Init(_m_DS3231_Chip_Address);
	
	USART_Send_Byte(USART1,0x02);
	USART_Send_Byte(USART1,0xFF);
	USART_Send_Byte(USART1,0xFF);
	if(DS3231_I2C_Error_Code() != 0x00)
	{
		USART_Send_Byte(USART1,DS3231_I2C_Error_Code());
	}
	
	
	
}





#endif 
