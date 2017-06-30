
#include "usart1_data.h"


/**
 * ���������ǣ�������ָ��Ϊ null
 * ���ʹ�ý�����ǣ���������ǰ��ֻ����һ����������������������������
 */
unsigned char _m_USART1_Data_Eof[] = {0xEF,0xFF};

/**
 * ���ڻ�������
 */
struct Linked_List_Data USART1_Data;

struct Linked_List_Node USART1_Data_Nodes[ USART1_Data_Node_Len ];

unsigned char _m_USART1_Data_Node_Chars[ USART1_Data_Node_Len ][ USART1_Data_Node_Char_Len ];


/**
 * ��ʼ��
 * Ҫ�� main ����ִ��
 */
void USART1_Data_Init(void)
{

	int i;
	struct Linked_List_Node *node;
	
	USART1_Data.Read_Lock = 0;
	USART1_Data.Write_Lock = 0;
	USART1_Data.Lock_Buff_Len = 0;
	USART1_Data.Char_Len = USART1_Data_Node_Char_Len;
	USART1_Data.Eof = _m_USART1_Data_Eof;
	USART1_Data.Eof_Len =  (sizeof(_m_USART1_Data_Eof) / sizeof(_m_USART1_Data_Eof[0]));
	//usart1_data.eof = NULL;
	
	for(i = 0;i < USART1_Data_Node_Len;i++)
	{
		USART1_Data_Nodes[i].C = _m_USART1_Data_Node_Chars[ i ];
		
		node = &USART1_Data_Nodes[i];
		Linked_Add_Last( &(USART1_Data.Free_List) , &node ,&(USART1_Data.Var_Node));
	}
	
}

void USART1_Received_To_Buffer_Ignore_Error(unsigned char c)
{
	Linked_List_Data_Add_Char( &USART1_Data, c);
}


/**
 * �Ѵ��ڵ��ַ��ӵ�buffer�У�
 * ��ʹ�� Register_USART1_Callback ע��
 */ 
unsigned int USART1_Received_To_Buffer(unsigned char c)
{
	return Linked_List_Data_Add_Char( &USART1_Data, c);
}

/**********************************
 * ��buffer��ȡ����д������д������ݣ���ȡ��ӻ���ɾ��

	int i = 0;
	unsigned char buff[usart1_data_node_char_len];

	while(1)
	{
		i = _USART1_Read_To_Buffer(buff);
		if(i == 0)
		{
			break;
		}
		for(i = 0;i< usart1_data_node_char_len;i++)
		{
			//Flash_LED_USART1_Send_Byte(buff[i]); //
		}
		
	}

 *
 */
unsigned int USART1_Read_To_Buffer(unsigned char * buff)
{
	return Linked_List_Data_Read( &USART1_Data, buff);
}




