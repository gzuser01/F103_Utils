
#ifndef __LINKED_LIST_H
#define	__LINKED_LIST_H


struct Linked_List_Node
{
	unsigned char *c;
	struct Linked_List_Node *next;
	
};



/**
 * �ѽڵ�ӵ��������һ��
 * ��� head Ϊ��,��ô�������½ڵ�Ϊͷ�ڵ�,�����Ҫ����ָ���ָ��
 */
void linked_add_last(struct Linked_List_Node **head,struct Linked_List_Node **new_node,struct Linked_List_Node **var_node);


/**
 * ɾ�������һ��
 */
void linked_remove_first(struct Linked_List_Node **head,struct Linked_List_Node **var_node);

/**
 * �����С
 */
unsigned int linked_size(struct Linked_List_Node *head,struct Linked_List_Node *var_node);



#endif

