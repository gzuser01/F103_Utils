
#ifndef __USART_DATA_H
#define	__USART_DATA_H

/**
 * 可以对串口接收到数据进行缓存，通过定义结束标记，可以整串读取，而不需要单个字符处理，
 * 适合接收特定格式的命令，也可避免处理不及时而阻塞
 *
 * 通过 Register_USART1_Callback 注册 USART_Received_To_Buffer，把串口接收到的数据放入缓存中
 * 在主循环中通过 USART_Read_From_Buffer 整串读取
 */

#include "usart1.h"
#include "linked_list.h"
#include "linked_list_data.h"



/**
 * 初始化函数
 * 
 * 
 * USART_Data 串口缓存数据
 * USART_Data_Nodes 串口缓存数据链表节点
 * USART_Data_Node_Len 链表中缓存节点的个数
 * USART_Data_Node_Chars 串口缓存数据的字符数组
 * USART_Data_Node_Char_Len 一个节点里面字节长度
 * USART_Data_Eof 缓存结束标记，可以让指针为 null
 * 						如果使用结束标记，发送命令前，只发送一个或多个结束标记有助于先清除缓存
 * USART_Eof_Len 缓存结束标记长度，如果没有结束标记那么为0
 *
 * 定义在栈中
 * unsigned char _m_USART_Data_Eof[] = {0xEF,0xFF};
 * unsigned short int USART_Eof_Len;
 * struct Linked_List_Data _m_USART_Data;
 * struct Linked_List_Node _m_USART_Data_Nodes[ USART_Data_Node_Len ];
 * unsigned char _m_USART_Data_Node_Chars[ USART_Data_Node_Len * USART_Data_Node_Char_Len ];
 *
 * 在配置或初始化函数中
 * _m_USART_Eof_Len = sizeof(_m_USART_Data_Eof) / sizeof(_m_USART_Data_Eof[0]);
 * USART_Data_Init(&_m_USART_Data,_m_USART_Data_Nodes,_m_USART_Data_Node_Chars,_m_USART_Data_Eof,_m_USART_Eof_Len);
 */
void USART_Data_Init(struct Linked_List_Data *USART_Data,
		struct Linked_List_Node *USART_Data_Nodes,
		unsigned short int USART_Data_Node_Len,
		unsigned char *USART_Data_Node_Chars,
		unsigned short int USART_Data_Node_Char_Len,
		unsigned char *USART_Data_Eof,unsigned short int USART_Eof_Len);

void USART_Received_To_Buffer_Ignore_Error(struct Linked_List_Data *USART_Data,unsigned char c);


/**
 * 把串口接收到的字符加到buffer中，
 * 可使用 Register_USART_Callback 注册
 */ 
unsigned int USART_Received_To_Buffer(struct Linked_List_Data *USART_Data,unsigned char c);

/**********************************
 * 从buffer读取串口写满或者写完的数据，读取后的数据从缓存中删除

	int i = 0;
	unsigned char buff[USART_data_node_char_len];

	while(1)
	{
		i = USART_Read_From_Buffer(&USART_Data,buff);
		if(i == 0)
		{
			return;
		}
		for(i = 0;i< USART_data_node_char_len;i++)
		{
			//USART_Send_Byte(&USART_Data,buff[i]); //
		}
		
	}

 *
 */
unsigned int USART_Read_From_Buffer(struct Linked_List_Data *USART_Data,unsigned char * buff);













#endif


