
#ifndef __USART1_DATA_H
#define	__USART1_DATA_H

#include "usart1.h"
#include "linked_list.h"
#include "linked_list_data.h"

/**
 * �����л���ڵ�ĸ���
 */
#define usart1_data_node_len 16

/**
 * һ���ڵ������ֽڳ���
 */
#define usart1_data_node_char_len 8

/**
 * ��ʼ��
 * Ҫ�� main ����ִ��
 */
void USART1_Data_Init(void);

void USART1_Received_To_Buffer_Ignore_Error(unsigned char c);


/**
 * �Ѵ��ڵ��ַ��ӵ�buffer�У�
 * ��ʹ�� Register_USART1_Callback ע��
 */ 
unsigned int USART1_Received_To_Buffer(unsigned char c);

/**********************************
 * ��buffer��ȡ����д������д�������

	int i = 0;
	unsigned char buff[usart1_data_node_char_len];

	while(1)
	{
		i = USART1_Read_To_Buffer(buff);
		if(i == 0)
		{
			return;
		}
		for(i = 0;i< usart1_data_node_char_len;i++)
		{
			//USART1_Send_Byte(buff[i]); //
		}
		
	}

 *
 */
unsigned int USART1_Read_To_Buffer(unsigned char * buff);













#endif


