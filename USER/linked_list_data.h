

#ifndef __LINKED_LIST_DATA_H
#define	__LINKED_LIST_DATA_H

#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

struct Linked_List_Data
{
	/** 数据链接的头节点 **/
	struct Linked_List_Node *data_list;
	/** 空闲链接的头节点 **/
	struct Linked_List_Node *free_list;
	/** 正在写入数据的节点 **/
	struct Linked_List_Node *current_node;
	/** 节点正在写入数据的序号 **/
	int current_node_write_idx;
	
	/** 节点可写入数据的长度 **/
	int char_length;
	/** 节点结束标记 **/
	unsigned char *eof; //如果为 NULL,则不判断结束标记,节点写满才可读
};

/**
 * 向当前写入数据的节点写入一个字符
 * 如果节点写满或者字符是结束标记，那么写入数据的节点会添加到数据链表
 */
void linked_list_data_add_char(struct Linked_List_Data *data_buffer,unsigned char c)
{

	int i;
	if(data_buffer == NULL)
	{
		return;
	}
	
	if(data_buffer->current_node == NULL)
	{
		//如果空闲的链表有节点，那么取空闲链接的老节点，否则取数据链表的老节点
		if(data_buffer->free_list == NULL)
		{
			if(data_buffer->data_list != NULL)
			{
				data_buffer->current_node = data_buffer->data_list;
				linked_remove_first( &data_buffer->data_list );
			}
		}
		else
		{
			data_buffer->current_node = data_buffer->free_list;
			linked_remove_first( &data_buffer->free_list );
		}
		
		//没有可用的节点
		if(data_buffer->current_node == NULL)
		{
			return;
		}
		else
		{
			//重置数据
			data_buffer->current_node_write_idx = 0;
			
			for(i = 0;i<data_buffer->char_length;i++)
			{
				data_buffer->current_node->c[i] = 0x00;
			}
		}
	}
	
	
	if(data_buffer->current_node_write_idx < data_buffer->char_length)
	{
		data_buffer->current_node->c[ data_buffer->current_node_write_idx ] = c;
		data_buffer->current_node_write_idx ++;
	}
	
	
	//满或者 c 为 0，则添加到链表
	if(data_buffer->current_node_write_idx == data_buffer->char_length || 
		(data_buffer->eof != NULL && c == *data_buffer->eof))
	{

		//把当前节点加到数据链表
		linked_add_last( &data_buffer->data_list, &data_buffer->current_node );
		
		
		//释放当前写入的节点
		data_buffer->current_node = NULL;
	}

}



/**
 * 从数据链表中读出一个节点的数据，
 * 如果有数据返回 1，否则返回 0，
 * 由于data_buffer读取后会被覆盖，因此数据会memcpy到 buff
 */
int linked_list_data_read(struct Linked_List_Data *data_buffer,unsigned char * buff)
{
	struct Linked_List_Node *readed_node;
	
	if(data_buffer == NULL || data_buffer->data_list == NULL)
	{
		return 0;
	}
	//把数据复制出来
	memcpy(buff,data_buffer->data_list->c,data_buffer->char_length);
	
	//记录已读节点的指针
	readed_node = data_buffer->data_list;
	
	//从数据节点删除第一个(已读节点)
	linked_remove_first( &data_buffer->data_list );
	//把已读节点加到空闲链表
	linked_add_last( &data_buffer->free_list, &readed_node );
	
	return 1;

}



#endif

