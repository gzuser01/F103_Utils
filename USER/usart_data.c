
#include "usart_data.h"



void USART_Data_Init(struct Linked_List_Data *USART_Data,
		struct Linked_List_Node *USART_Data_Nodes,
		unsigned short int USART_Data_Node_Len,
		unsigned char *USART_Data_Node_Chars,
		unsigned short int USART_Data_Node_Char_Len,
		unsigned char *USART_Data_Eof,unsigned short int USART_Eof_Len)
{

	int i;
	struct Linked_List_Node *node;

	
	USART_Data->Read_Lock = 0;
	USART_Data->Write_Lock = 0;
	USART_Data->Lock_Buff_Len = 0;
	USART_Data->Char_Len = USART_Data_Node_Char_Len;
	USART_Data->Eof = USART_Data_Eof;
	USART_Data->Eof_Len = USART_Eof_Len;
	
	
	for(i = 0;i < USART_Data_Node_Len;i++)
	{
		/**
		 * 初始化数据存储的地址
		 */
		USART_Data_Nodes[i].C = &USART_Data_Node_Chars[ i*USART_Data_Node_Char_Len ];
		
		node = &USART_Data_Nodes[i];
		Linked_Add_Last( &(USART_Data->Free_List) , &node ,&(USART_Data->Var_Node));
	}
	
}

void USART_Received_To_Buffer_Ignore_Error(struct Linked_List_Data *USART_Data,unsigned char c)
{
	Linked_List_Data_Add_Char( USART_Data, c);
}



unsigned int USART_Received_To_Buffer(struct Linked_List_Data *USART_Data,unsigned char c)
{
	return Linked_List_Data_Add_Char( USART_Data, c);
}


unsigned int USART_Read_From_Buffer(struct Linked_List_Data *USART_Data,unsigned char * buff)
{
	return Linked_List_Data_Read( USART_Data, buff);
}




