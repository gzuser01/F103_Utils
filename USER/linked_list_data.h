

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
unsigned int linked_list_data_add_char(struct Linked_List_Data *data_buffer,unsigned char c);


/**
 * 从数据链表中读出一个节点的数据，
 * 如果有数据返回 1，否则返回 0，如果被锁定写或者读，也返回0
 * 由于data_buffer读取后会被覆盖，因此数据会memcpy到 buff
 *
 * 
 */
unsigned int linked_list_data_read(struct Linked_List_Data *data_buffer,unsigned char * buff);



#endif

