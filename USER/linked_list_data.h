

#ifndef __LINKED_LIST_DATA_H
#define	__LINKED_LIST_DATA_H

#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

#define LOCK_BUFFER_LENGTH 8


/**
 * 链表数据
 */
struct Linked_List_Data
{
	/** 数据链接的头节点 **/
	struct Linked_List_Node *data_list;
	/** 空闲链接的头节点 **/
	struct Linked_List_Node *free_list;
	/** 正在写入数据的节点 **/
	struct Linked_List_Node *current_node;
	/** 节点正在写入数据的序号 **/
	unsigned int current_node_write_idx;
	
	/** 节点可写入数据的长度 **/
	int char_length;
	/** 节点结束标记 **/
	unsigned char *eof; //如果为 NULL,则不判断结束标记,节点写满才可读
	/** 节点结束标记长度 **/
	unsigned short int eof_length;
	
	/** 临时变量 
	 *  linked_list_data_add_char 或 linked_list_data_read 会使用的在 linked_add_last 或 linked_remove_first 操作时记录最后一个节点的指针
	 *  或 从数据链表读取数据后,记录数据链表的第一个节点的指针,从数据链表删除后,添加到空闲链表
	 */
	struct Linked_List_Node *var_node;
	
	/** 写锁 **/
	unsigned short int write_lock;
	/** 读锁 **/
	unsigned short int read_lock;
	
	/** 如果写锁，那么会加到这个buffer **/
	unsigned char lock_buffer[LOCK_BUFFER_LENGTH];
	/** 已记录lock buffer的字符个数 **/
	short int lock_buffer_length;
};

/**
 * 向当前写入数据的节点写入一个字符
 * 如果节点写满或者字符是结束标记，那么写入数据的节点会添加到数据链表
 * 如果数据写入成功返回 1，否则返回 0，如果被锁定写或者读则不写入而丢失，返回0
 */
unsigned int linked_list_data_add_char0(struct Linked_List_Data *data_buffer,unsigned char c)
{
	
	unsigned int i;
	unsigned short int new_node;
	
	if(data_buffer->current_node == NULL)
	{
		//如果空闲的链表有节点，那么取空闲链接的老节点，否则取数据链表的老节点
		if(data_buffer->free_list == NULL)
		{
			if(data_buffer->data_list != NULL)
			{
				data_buffer->current_node = data_buffer->data_list;
				linked_remove_first( &data_buffer->data_list,&data_buffer->var_node );
			}
		}
		else
		{
			data_buffer->current_node = data_buffer->free_list;
			linked_remove_first( &data_buffer->free_list,&data_buffer->var_node );
		}
		
		//没有可用的节点
		if(data_buffer->current_node == NULL)
		{
			return 0;
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
	
	//写入数据
	if(data_buffer->current_node_write_idx < data_buffer->char_length)
	{
		data_buffer->current_node->c[ data_buffer->current_node_write_idx ] = c;
		data_buffer->current_node_write_idx ++;
	}
	
	new_node = 0;

	//写满，无论是否有结束标记
	if(data_buffer->current_node_write_idx == data_buffer->char_length)
	{
		new_node = 1;
	}
	else
	{	
		//有结束标记并且写了足够的字符
		if(data_buffer->eof != NULL && data_buffer->eof_length != 0 &&
			data_buffer->current_node_write_idx >= data_buffer->eof_length)
		{
			//先设置为可以到下一个节点
			new_node = 1;
			for(i = 0;i < data_buffer->eof_length;i++)
			{
				//只有字符不匹配
				if(data_buffer->current_node->c[ data_buffer->current_node_write_idx - i - 1 ] != data_buffer->eof[ data_buffer->eof_length - i - 1 ] )
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
		linked_add_last( &data_buffer->data_list, &data_buffer->current_node,&data_buffer->var_node );

		//释放当前写入的节点
		data_buffer->current_node = NULL;
	}

	return 1;

}


unsigned int linked_list_data_add_char(struct Linked_List_Data *data_buffer,unsigned char c)
{
	int i;

	if(data_buffer == NULL)
	{
		return 0;
	}
	
		//被锁
	if(data_buffer->write_lock | data_buffer->read_lock)
	{
		
		//锁BUFF有空间，那么存到锁BUFF，否则丢弃数据
		if(data_buffer->lock_buffer_length < LOCK_BUFFER_LENGTH)
		{
			data_buffer->lock_buffer[data_buffer->lock_buffer_length] = c;//从0开始
			data_buffer->lock_buffer_length ++;//添加一个后加1
			return 1;
		}
		else
		{
			return 0;
		}
	}
	
	//加锁
	data_buffer->write_lock = 1;
	
	//先处理锁缓存的数据
	if(data_buffer->lock_buffer_length > 0) //有缓存个数从1开始，为0表示没有
	{
		for(i = 0; i< data_buffer->lock_buffer_length;i++)
		{
			linked_list_data_add_char0(data_buffer,data_buffer->lock_buffer[i]);
		}
		data_buffer->lock_buffer_length = 0; //重置为0
	}
	
	linked_list_data_add_char0(data_buffer,c);
		
	//解锁
	data_buffer->write_lock = 0;
	
	return 1;
}



/**
 * 从数据链表中读出一个节点的数据，
 * 如果有数据返回 1，否则返回 0，如果被锁定写或者读，也返回0
 * 由于data_buffer读取后会被覆盖，因此数据会memcpy到 buff
 *
 * 
 */
unsigned int linked_list_data_read(struct Linked_List_Data *data_buffer,unsigned char * buff)
{

	
	if(data_buffer == NULL || data_buffer->data_list == NULL)
	{
		return 0;
	}
	
	//被锁
	if(data_buffer->write_lock | data_buffer->read_lock)
	{
		return 0;
	}
	//加锁
	data_buffer->read_lock = 1;
		
	//把数据复制出来
	memcpy(buff,data_buffer->data_list->c,data_buffer->char_length);
	
	
	//记录已读节点的指针
	//data_buffer->var_node = data_buffer->data_list;
	//把数据节点加到空闲链表
	linked_add_last( &data_buffer->free_list, &data_buffer->data_list,&data_buffer->var_node );
	
	//从数据节点删除第一个(已读节点)
	linked_remove_first( &data_buffer->data_list,&data_buffer->var_node );
	
	//解锁
	data_buffer->read_lock = 0;
	
	return 1;

}



#endif

