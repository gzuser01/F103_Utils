
#ifndef __USART1_DATA_H
#define	__USART1_DATA_H

#include "usart1.h"
#include "linked_list.h"
#include "linked_list_data.h"

/**
 * 链表中缓存节点的个数
 */
#define usart1_data_node_len 16

/**
 * 一个节点里面字节长度
 */
#define usart1_data_node_char_len 8

/**
 * 初始化
 * 要在 main 函数执行
 */
void USART1_Data_Init(void);

void USART1_Received_To_Buffer_Ignore_Error(unsigned char c);


/**
 * 把串口的字符加到buffer中，
 * 可使用 Register_USART1_Callback 注册
 */ 
unsigned int USART1_Received_To_Buffer(unsigned char c);

/**********************************
 * 从buffer读取串口写满或者写完的数据

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


