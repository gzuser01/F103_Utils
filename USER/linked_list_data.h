

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
unsigned int linked_list_data_add_char(struct Linked_List_Data *data_buffer,unsigned char c);


/**
 * �����������ж���һ���ڵ�����ݣ�
 * ��������ݷ��� 1�����򷵻� 0�����������д���߶���Ҳ����0
 * ����data_buffer��ȡ��ᱻ���ǣ�������ݻ�memcpy�� buff
 *
 * 
 */
unsigned int linked_list_data_read(struct Linked_List_Data *data_buffer,unsigned char * buff);



#endif

