
#ifndef __USART1_DATA_H
#define	__USART1_DATA_H

#include "usart1.h"
#include "linked_list.h"
#include "linked_list_data.h"

/**
 * �����л���ڵ�ĸ���
 */
const int usart1_data_node_len = 32;

/**
 * һ���ڵ������ֽڳ���
 */
const int usart1_data_node_char_len = 8;

unsigned char usart1_data_eof = 0x00;

/**
 * ���ڻ�������
 */
struct Linked_List_Data usart1_data;

struct Linked_List_Node usart1_data_nodes[ usart1_data_node_len ];
unsigned char usart1_data_node_chars[ usart1_data_node_len ][ usart1_data_node_char_len ];

/**
 * ��ʼ��
 * Ҫ�� main ����ִ��
 */
void usart1_data_init()
{

	int i;
	struct Linked_List_Node *node;
	
	usart1_data.char_length = usart1_data_node_char_len;
	//usart1_data.eof = &usart1_data_eof;
	usart1_data.eof = NULL;
	
	for(i = 0;i<usart1_data_node_len;i++)
	{
		usart1_data_nodes[i].c = usart1_data_node_chars[ i ];
		
		node = &usart1_data_nodes[i];
		linked_add_last( &(usart1_data.free_list) , &node );
	}
	
}

/**
 * �Ѵ��ڵ��ַ��ӵ�buffer�У�
 * ��ʹ�� Register_USART1_Callback ע��
 */ 
void _UART1_Received_To_Buffer(unsigned char c)
{
	linked_list_data_add_char( &usart1_data, c);
}

/**********************************
 * ��buffer��ȡ����д������д�������

	int i = 0;
	unsigned char buff[usart1_data_node_char_len];

	while(1)
	{
		i = _UART1_Read_To_Buffer(buff);
		if(i == 0)
		{
			return;
		}
		for(i = 0;i< usart1_data_node_char_len;i++)
		{
			//UART1_Send_Byte(buff[i]); //
		}
		
	}

 *
 */
int _UART1_Read_To_Buffer(unsigned char * buff)
{
	return linked_list_data_read( &usart1_data, buff);
}















#endif


