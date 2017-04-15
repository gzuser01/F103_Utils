

#ifndef __LINKED_LIST_DATA_H
#define	__LINKED_LIST_DATA_H

#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

struct Linked_List_Data
{
	/** �������ӵ�ͷ�ڵ� **/
	struct Linked_List_Node *data_list;
	/** �������ӵ�ͷ�ڵ� **/
	struct Linked_List_Node *free_list;
	/** ����д�����ݵĽڵ� **/
	struct Linked_List_Node *current_node;
	/** �ڵ�����д�����ݵ���� **/
	int current_node_write_idx;
	
	/** �ڵ��д�����ݵĳ��� **/
	int char_length;
	/** �ڵ������� **/
	unsigned char *eof; //���Ϊ NULL,���жϽ������,�ڵ�д���ſɶ�
};

/**
 * ��ǰд�����ݵĽڵ�д��һ���ַ�
 * ����ڵ�д�������ַ��ǽ�����ǣ���ôд�����ݵĽڵ����ӵ���������
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
		//������е������нڵ㣬��ôȡ�������ӵ��Ͻڵ㣬����ȡ����������Ͻڵ�
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
		
		//û�п��õĽڵ�
		if(data_buffer->current_node == NULL)
		{
			return;
		}
		else
		{
			//��������
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
	
	
	//������ c Ϊ 0������ӵ�����
	if(data_buffer->current_node_write_idx == data_buffer->char_length || 
		(data_buffer->eof != NULL && c == *data_buffer->eof))
	{

		//�ѵ�ǰ�ڵ�ӵ���������
		linked_add_last( &data_buffer->data_list, &data_buffer->current_node );
		
		
		//�ͷŵ�ǰд��Ľڵ�
		data_buffer->current_node = NULL;
	}

}



/**
 * �����������ж���һ���ڵ�����ݣ�
 * ��������ݷ��� 1�����򷵻� 0��
 * ����data_buffer��ȡ��ᱻ���ǣ�������ݻ�memcpy�� buff
 */
int linked_list_data_read(struct Linked_List_Data *data_buffer,unsigned char * buff)
{
	struct Linked_List_Node *readed_node;
	
	if(data_buffer == NULL || data_buffer->data_list == NULL)
	{
		return 0;
	}
	//�����ݸ��Ƴ���
	memcpy(buff,data_buffer->data_list->c,data_buffer->char_length);
	
	//��¼�Ѷ��ڵ��ָ��
	readed_node = data_buffer->data_list;
	
	//�����ݽڵ�ɾ����һ��(�Ѷ��ڵ�)
	linked_remove_first( &data_buffer->data_list );
	//���Ѷ��ڵ�ӵ���������
	linked_add_last( &data_buffer->free_list, &readed_node );
	
	return 1;

}



#endif

