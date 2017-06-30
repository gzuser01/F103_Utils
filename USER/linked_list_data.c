
#include "linked_list_data.h"


/**
 * 向当前写入数据的节点写入一个字符
 * 如果节点写满或者字符是结束标记，那么写入数据的节点会添加到数据链表
 * 如果数据写入成功返回 1，否则返回 0，如果被锁定写或者读则不写入而丢失，返回0
 */
unsigned int Linked_List_Data_Add_Char0(struct Linked_List_Data *Data_Buff,unsigned char c)
{
	
	unsigned int i;
	unsigned short int new_node;
	
	if(Data_Buff->Current_Node == NULL)
	{
		//如果空闲的链表有节点，那么取空闲链接的老节点，否则取数据链表的老节点
		if(Data_Buff->Free_List == NULL)
		{
			if(Data_Buff->Data_List != NULL)
			{
				Data_Buff->Current_Node = Data_Buff->Data_List;
				Linked_Remove_First( &Data_Buff->Data_List,&Data_Buff->Var_Node );
			}
		}
		else
		{
			Data_Buff->Current_Node = Data_Buff->Free_List;
			Linked_Remove_First( &Data_Buff->Free_List,&Data_Buff->Var_Node );
		}
		
		//没有可用的节点
		if(Data_Buff->Current_Node == NULL)
		{
			return 0;
		}
		else
		{
			//重置数据
			Data_Buff->Current_Node_Write_Idx = 0;
			
			for(i = 0;i<Data_Buff->Char_Len;i++)
			{
				Data_Buff->Current_Node->C[i] = 0x00;
			}
		}
	}
	
	//写入数据
	if(Data_Buff->Current_Node_Write_Idx < Data_Buff->Char_Len)
	{
		Data_Buff->Current_Node->C[ Data_Buff->Current_Node_Write_Idx ] = c;
		Data_Buff->Current_Node_Write_Idx ++;
	}
	
	new_node = 0;

	//写满，无论是否有结束标记
	if(Data_Buff->Current_Node_Write_Idx == Data_Buff->Char_Len)
	{
		new_node = 1;
	}
	else
	{	
		//有结束标记并且写了足够的字符
		if(Data_Buff->Eof != NULL && Data_Buff->Eof_Len != 0 &&
			Data_Buff->Current_Node_Write_Idx >= Data_Buff->Eof_Len)
		{
			//先设置为可以到下一个节点
			new_node = 1;
			for(i = 0;i < Data_Buff->Eof_Len;i++)
			{
				//只有字符不匹配
				if(Data_Buff->Current_Node->C[ Data_Buff->Current_Node_Write_Idx - i - 1 ] != Data_Buff->Eof[ Data_Buff->Eof_Len - i - 1 ] )
				{
					new_node = 0;
				}
			}
		}
		
	}
	
	//添加到链表	
	if(new_node == 1)
	{
		//把当前节点加到数据链表
		Linked_Add_Last( &Data_Buff->Data_List, &Data_Buff->Current_Node,&Data_Buff->Var_Node );

		//释放当前写入的节点
		Data_Buff->Current_Node = NULL;
	}

	return 1;

}


unsigned int Linked_List_Data_Add_Char(struct Linked_List_Data *Data_Buff,unsigned char c)
{
	int i;

	if(Data_Buff == NULL)
	{
		return 0;
	}
	
		//被锁
	if(Data_Buff->Write_Lock | Data_Buff->Read_Lock)
	{
		
		//锁BUFF有空间，那么存到锁BUFF，否则丢弃数据
		if(Data_Buff->Lock_Buff_Len < LOCK_BUFFER_LENGTH)
		{
			Data_Buff->Lock_Buff[Data_Buff->Lock_Buff_Len] = c;//从0开始
			Data_Buff->Lock_Buff_Len ++;//添加一个后加1
			return 1;
		}
		else
		{
			return 0;
		}
	}
	
	//加锁
	Data_Buff->Write_Lock = 1;
	
	//先处理锁缓存的数据
	if(Data_Buff->Lock_Buff_Len > 0) //有缓存个数从1开始，为0表示没有
	{
		for(i = 0; i< Data_Buff->Lock_Buff_Len;i++)
		{
			Linked_List_Data_Add_Char0(Data_Buff,Data_Buff->Lock_Buff[i]);
		}
		Data_Buff->Lock_Buff_Len = 0; //重置为0
	}
	
	Linked_List_Data_Add_Char0(Data_Buff,c);
		
	//解锁
	Data_Buff->Write_Lock = 0;
	
	return 1;
}



/**
 * 从数据链表中读出一个节点的数据，
 * 如果有数据返回 1，否则返回 0，如果被锁定写或者读，也返回0
 * 由于Data_Buff读取后会被覆盖，因此数据会memcpy到 buff
 *
 * 
 */
unsigned int Linked_List_Data_Read(struct Linked_List_Data *Data_Buff,unsigned char * Buff)
{

	
	if(Data_Buff == NULL || Data_Buff->Data_List == NULL)
	{
		return 0;
	}
	
	//被锁
	if(Data_Buff->Write_Lock | Data_Buff->Read_Lock)
	{
		return 0;
	}
	//加锁
	Data_Buff->Read_Lock = 1;
		
	//把数据复制出来
	memcpy(Buff,Data_Buff->Data_List->C,Data_Buff->Char_Len);
	
	
	//记录已读节点的指针
	//Data_Buff->var_node = Data_Buff->data_list;
	//把数据节点加到空闲链表
	Linked_Add_Last( &Data_Buff->Free_List, &Data_Buff->Data_List,&Data_Buff->Var_Node );
	
	//从数据节点删除第一个(已读节点)
	Linked_Remove_First( &Data_Buff->Data_List,&Data_Buff->Var_Node );
	
	//解锁
	Data_Buff->Read_Lock = 0;
	
	return 1;

}
