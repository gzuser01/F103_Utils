
#include "linked_list.h"
#include <stddef.h>




/**
 * �ѽڵ�ӵ��������һ��
 * ��� head Ϊ��,��ô�������½ڵ�Ϊͷ�ڵ�,�����Ҫ����ָ���ָ��
 */
void Linked_Add_Last(struct Linked_List_Node **Head,struct Linked_List_Node **New_Node,struct Linked_List_Node **Var_Node)
{

	*Var_Node = NULL;


	//On first add node, head is NULL
	if(*Head == NULL){
		*Head = *New_Node;
	}else{
		*Var_Node = *Head;
		while((*Var_Node)->Next != NULL)
		{
			*Var_Node = (*Var_Node)->Next;
		}
		(*Var_Node)->Next = *New_Node;
	}
	

}



/**
 * ɾ�������һ��
 */
void Linked_Remove_First(struct Linked_List_Node **Head,struct Linked_List_Node **Var_Node)
{
	
	if(Head == NULL || (*Head) == NULL)
	{
		return;
	}
	
	if((*Head)->Next == NULL)
	{
		*Head = NULL;
		return;
	}
	*Var_Node = (*Head)->Next;
	(*Head)->Next = NULL;
	*Head = *Var_Node;
}

/**
 * �����С
 */
unsigned int Linked_Size(struct Linked_List_Node *Head,struct Linked_List_Node *Var_Node)
{
	unsigned int i;
	if(Head == NULL)
	{
		return 0;
	}
	
	i = 1;
	Var_Node = Head;
	while(Var_Node->Next != NULL)
	{
		Var_Node = (Var_Node)->Next;
		i++;
	}
	
	return i;
}

