
#include "usart1_data.h"


/**
 * 缓冲结束标记，可以让指针为 null
 * 如果使用结束标记，发送命令前，只发送一个或多个结束标记有助于先清除缓存
 */
unsigned char _m_USART1_Data_Eof[] = {0xEF,0xFF};

/**
 * 串口缓存数据
 */
struct Linked_List_Data USART1_Data;

struct Linked_List_Node USART1_Data_Nodes[ USART1_Data_Node_Len ];

unsigned char _m_USART1_Data_Node_Chars[ USART1_Data_Node_Len ][ USART1_Data_Node_Char_Len ];


/**
 * 初始化
 * 要在 main 函数执行
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
 * 把串口的字符加到buffer中，
 * 可使用 Register_USART1_Callback 注册
 */ 
unsigned int USART1_Received_To_Buffer(unsigned char c)
{
	return Linked_List_Data_Add_Char( &USART1_Data, c);
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
	return Linked_List_Data_Read( &USART1_Data, buff);
}




