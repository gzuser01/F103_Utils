
#include "usart1_data.h"


/**
 * 缓冲结束标记，可以让指针为 null
 * 如果使用结束标记，发送命令前，只发送一个或多个结束标记有助于先清除缓存
 */
unsigned char usart1_data_eof[] = {0xEF,0xFF};

/**
 * 串口缓存数据
 */
struct Linked_List_Data usart1_data;

struct Linked_List_Node usart1_data_nodes[ usart1_data_node_len ];

unsigned char usart1_data_node_chars[ usart1_data_node_len ][ usart1_data_node_char_len ];


/**
 * 初始化
 * 要在 main 函数执行
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
 * 把串口的字符加到buffer中，
 * 可使用 Register_USART1_Callback 注册
 */ 
unsigned int USART1_Received_To_Buffer(unsigned char c)
{
	return linked_list_data_add_char( &usart1_data, c);
}

/**********************************
 * 从buffer读取串口写满或者写完的数据，读取后从缓存删除

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




