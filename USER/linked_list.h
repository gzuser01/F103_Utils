
#ifndef __LINKED_LIST_H
#define	__LINKED_LIST_H


struct Linked_List_Node
{
	unsigned char *C;
	struct Linked_List_Node *Next;
	
};



/**
 * �ѽڵ�ӵ��������һ��
 * ��� head Ϊ��,��ô�������½ڵ�Ϊͷ�ڵ�,�����Ҫ����ָ���ָ��
 */
void Linked_Add_Last(struct Linked_List_Node **Head,struct Linked_List_Node **New_Node,struct Linked_List_Node **Var_Node);


/**
 * ɾ�������һ��
 */
void Linked_Remove_First(struct Linked_List_Node **Head,struct Linked_List_Node **Var_Node);

/**
 * �����С
 */
unsigned int Linked_Size(struct Linked_List_Node *Head,struct Linked_List_Node *Var_Node);



#endif

