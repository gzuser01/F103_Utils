

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
	struct Linked_List_Node *Data_List;
	/** �������ӵ�ͷ�ڵ� **/
	struct Linked_List_Node *Free_List;
	/** ����д�����ݵĽڵ� **/
	struct Linked_List_Node *Current_Node;
	/** �ڵ�����д�����ݵ���� **/
	unsigned int Current_Node_Write_Idx;
	
	/** �ڵ��д�����ݵĳ��� **/
	int Char_Len;
	/** �ڵ������� **/
	unsigned char *Eof; //���Ϊ NULL,���жϽ������,�ڵ�д���ſɶ�
	/** �ڵ������ǳ��� **/
	unsigned short int Eof_Len;
	
	/** ��ʱ���� 
	 *  linked_list_data_add_char �� linked_list_data_read ��ʹ�õ��� linked_add_last �� linked_remove_first ����ʱ��¼���һ���ڵ��ָ��
	 *  �� �����������ȡ���ݺ�,��¼��������ĵ�һ���ڵ��ָ��,����������ɾ����,��ӵ���������
	 */
	struct Linked_List_Node *Var_Node;
	
	/** д�� **/
	unsigned short int Write_Lock;
	/** ���� **/
	unsigned short int Read_Lock;
	
	/** ���д������ô��ӵ����buffer **/
	unsigned char Lock_Buff[LOCK_BUFFER_LENGTH];
	/** �Ѽ�¼lock buffer���ַ����� **/
	short int Lock_Buff_Len;
};

/**
 * ��ǰд�����ݵĽڵ�д��һ���ַ�
 * ����ڵ�д�������ַ��ǽ�����ǣ���ôд�����ݵĽڵ����ӵ���������
 * �������д��ɹ����� 1�����򷵻� 0�����������д���߶���д�����ʧ������0
 */
unsigned int Linked_List_Data_Add_Char(struct Linked_List_Data *Data_Buff,unsigned char c);


/**
 * �����������ж���һ���ڵ�����ݣ�
 * ��������ݷ��� 1�����򷵻� 0�����������д���߶���Ҳ����0
 * ����data_buffer��ȡ��ᱻ���ǣ�������ݻ�memcpy�� buff
 *
 * 
 */
unsigned int Linked_List_Data_Read(struct Linked_List_Data *Data_Buff,unsigned char * Buff);



#endif

