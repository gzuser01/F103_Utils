

#ifndef __LINKED_LIST_DATA_H
#define	__LINKED_LIST_DATA_H

#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

#define LOCK_BUFFER_LENGTH 8


/**
 * ��������
 */
struct Linked_List_Data
{
	/** �������ӵ�ͷ�ڵ� **/
	struct Linked_List_Node *data_list;
	/** �������ӵ�ͷ�ڵ� **/
	struct Linked_List_Node *free_list;
	/** ����д�����ݵĽڵ� **/
	struct Linked_List_Node *current_node;
	/** �ڵ�����д�����ݵ���� **/
	unsigned int current_node_write_idx;
	
	/** �ڵ��д�����ݵĳ��� **/
	int char_length;
	/** �ڵ������� **/
	unsigned char *eof; //���Ϊ NULL,���жϽ������,�ڵ�д���ſɶ�
	/** �ڵ������ǳ��� **/
	unsigned short int eof_length;
	
	/** ��ʱ���� 
	 *  linked_list_data_add_char �� linked_list_data_read ��ʹ�õ��� linked_add_last �� linked_remove_first ����ʱ��¼���һ���ڵ��ָ��
	 *  �� �����������ȡ���ݺ�,��¼��������ĵ�һ���ڵ��ָ��,����������ɾ����,��ӵ���������
	 */
	struct Linked_List_Node *var_node;
	
	/** д�� **/
	unsigned short int write_lock;
	/** ���� **/
	unsigned short int read_lock;
	
	/** ���д������ô��ӵ����buffer **/
	unsigned char lock_buffer[LOCK_BUFFER_LENGTH];
	/** �Ѽ�¼lock buffer���ַ����� **/
	short int lock_buffer_length;
};

/**
 * ��ǰд�����ݵĽڵ�д��һ���ַ�
 * ����ڵ�д�������ַ��ǽ�����ǣ���ôд�����ݵĽڵ����ӵ���������
 * �������д��ɹ����� 1�����򷵻� 0�����������д���߶���д�����ʧ������0
 */
unsigned int linked_list_data_add_char0(struct Linked_List_Data *data_buffer,unsigned char c)
{
	
	unsigned int i;
	unsigned short int new_node;
	
	if(data_buffer->current_node == NULL)
	{
		//������е������нڵ㣬��ôȡ�������ӵ��Ͻڵ㣬����ȡ����������Ͻڵ�
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
		
		//û�п��õĽڵ�
		if(data_buffer->current_node == NULL)
		{
			return 0;
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
	
	//д������
	if(data_buffer->current_node_write_idx < data_buffer->char_length)
	{
		data_buffer->current_node->c[ data_buffer->current_node_write_idx ] = c;
		data_buffer->current_node_write_idx ++;
	}
	
	new_node = 0;

	//д���������Ƿ��н������
	if(data_buffer->current_node_write_idx == data_buffer->char_length)
	{
		new_node = 1;
	}
	else
	{	
		//�н�����ǲ���д���㹻���ַ�
		if(data_buffer->eof != NULL && data_buffer->eof_length != 0 &&
			data_buffer->current_node_write_idx >= data_buffer->eof_length)
		{
			//������Ϊ���Ե���һ���ڵ�
			new_node = 1;
			for(i = 0;i < data_buffer->eof_length;i++)
			{
				//ֻ���ַ���ƥ��
				if(data_buffer->current_node->c[ data_buffer->current_node_write_idx - i - 1 ] != data_buffer->eof[ data_buffer->eof_length - i - 1 ] )
				{
					new_node = 0;
				}
			}
		}
		
	}
	
	//��ӵ�����	
	if(new_node == 1)
	{
		//�ѵ�ǰ�ڵ�ӵ���������
		linked_add_last( &data_buffer->data_list, &data_buffer->current_node,&data_buffer->var_node );

		//�ͷŵ�ǰд��Ľڵ�
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
	
		//����
	if(data_buffer->write_lock | data_buffer->read_lock)
	{
		
		//��BUFF�пռ䣬��ô�浽��BUFF������������
		if(data_buffer->lock_buffer_length < LOCK_BUFFER_LENGTH)
		{
			data_buffer->lock_buffer[data_buffer->lock_buffer_length] = c;//��0��ʼ
			data_buffer->lock_buffer_length ++;//���һ�����1
			return 1;
		}
		else
		{
			return 0;
		}
	}
	
	//����
	data_buffer->write_lock = 1;
	
	//�ȴ��������������
	if(data_buffer->lock_buffer_length > 0) //�л��������1��ʼ��Ϊ0��ʾû��
	{
		for(i = 0; i< data_buffer->lock_buffer_length;i++)
		{
			linked_list_data_add_char0(data_buffer,data_buffer->lock_buffer[i]);
		}
		data_buffer->lock_buffer_length = 0; //����Ϊ0
	}
	
	linked_list_data_add_char0(data_buffer,c);
		
	//����
	data_buffer->write_lock = 0;
	
	return 1;
}



/**
 * �����������ж���һ���ڵ�����ݣ�
 * ��������ݷ��� 1�����򷵻� 0�����������д���߶���Ҳ����0
 * ����data_buffer��ȡ��ᱻ���ǣ�������ݻ�memcpy�� buff
 *
 * 
 */
unsigned int linked_list_data_read(struct Linked_List_Data *data_buffer,unsigned char * buff)
{

	
	if(data_buffer == NULL || data_buffer->data_list == NULL)
	{
		return 0;
	}
	
	//����
	if(data_buffer->write_lock | data_buffer->read_lock)
	{
		return 0;
	}
	//����
	data_buffer->read_lock = 1;
		
	//�����ݸ��Ƴ���
	memcpy(buff,data_buffer->data_list->c,data_buffer->char_length);
	
	
	//��¼�Ѷ��ڵ��ָ��
	//data_buffer->var_node = data_buffer->data_list;
	//�����ݽڵ�ӵ���������
	linked_add_last( &data_buffer->free_list, &data_buffer->data_list,&data_buffer->var_node );
	
	//�����ݽڵ�ɾ����һ��(�Ѷ��ڵ�)
	linked_remove_first( &data_buffer->data_list,&data_buffer->var_node );
	
	//����
	data_buffer->read_lock = 0;
	
	return 1;

}



#endif

