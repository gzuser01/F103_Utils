
#include "usart1_data.h"


/**
 * ���������ǣ�������ָ��Ϊ null
 * ���ʹ�ý�����ǣ���������ǰ��ֻ����һ����������������������������
 */
unsigned char usart1_data_eof[] = {0xEF,0xFF};

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
void USART1_Data_Init(void)
{

	int i;
	struct Linked_List_Node *node;
	
	usart1_data.read_lock = 0;
	usart1_data.write_lock = 0;
	usart1_data.lock_buffer_length = 0;
	usart1_data.char_length = usart1_data_node_char_len;
	usart1_data.eof = usart1_data_eof;
	usart1_data.eof_length =  (sizeof(usart1_data_eof) / sizeof(usart1_data_eof[0]));
	//usart1_data.eof = NULL;
	
	for(i = 0;i<usart1_data_node_len;i++)
	{
		usart1_data_nodes[i].c = usart1_data_node_chars[ i ];
		
		node = &usart1_data_nodes[i];
		linked_add_last( &(usart1_data.free_list) , &node ,&(usart1_data.var_node));
	}
	
}

void USART1_Received_To_Buffer_Ignore_Error(unsigned char c)
{
	linked_list_data_add_char( &usart1_data, c);
}


/**
 * �Ѵ��ڵ��ַ��ӵ�buffer�У�
 * ��ʹ�� Register_USART1_Callback ע��
 */ 
unsigned int USART1_Received_To_Buffer(unsigned char c)
{
	return linked_list_data_add_char( &usart1_data, c);
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
	return linked_list_data_read( &usart1_data, buff);
}




