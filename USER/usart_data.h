
#ifndef __USART_DATA_H
#define	__USART_DATA_H

/**
 * ���ԶԴ��ڽ��յ����ݽ��л��棬ͨ�����������ǣ�����������ȡ��������Ҫ�����ַ�����
 * �ʺϽ����ض���ʽ�����Ҳ�ɱ��⴦����ʱ������
 *
 * ͨ�� Register_USART1_Callback ע�� USART_Received_To_Buffer���Ѵ��ڽ��յ������ݷ��뻺����
 * ����ѭ����ͨ�� USART_Read_From_Buffer ������ȡ
 */

#include "usart1.h"
#include "linked_list.h"
#include "linked_list_data.h"



/**
 * ��ʼ������
 * 
 * 
 * USART_Data ���ڻ�������
 * USART_Data_Nodes ���ڻ�����������ڵ�
 * USART_Data_Node_Len �����л���ڵ�ĸ���
 * USART_Data_Node_Chars ���ڻ������ݵ��ַ�����
 * USART_Data_Node_Char_Len һ���ڵ������ֽڳ���
 * USART_Data_Eof ���������ǣ�������ָ��Ϊ null
 * 						���ʹ�ý�����ǣ���������ǰ��ֻ����һ����������������������������
 * USART_Eof_Len ���������ǳ��ȣ����û�н��������ôΪ0
 *
 * ������ջ��
 * unsigned char _m_USART_Data_Eof[] = {0xEF,0xFF};
 * unsigned short int USART_Eof_Len;
 * struct Linked_List_Data _m_USART_Data;
 * struct Linked_List_Node _m_USART_Data_Nodes[ USART_Data_Node_Len ];
 * unsigned char _m_USART_Data_Node_Chars[ USART_Data_Node_Len * USART_Data_Node_Char_Len ];
 *
 * �����û��ʼ��������
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
 * �Ѵ��ڽ��յ����ַ��ӵ�buffer�У�
 * ��ʹ�� Register_USART_Callback ע��
 */ 
unsigned int USART_Received_To_Buffer(struct Linked_List_Data *USART_Data,unsigned char c);

/**********************************
 * ��buffer��ȡ����д������д������ݣ���ȡ������ݴӻ�����ɾ��

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


