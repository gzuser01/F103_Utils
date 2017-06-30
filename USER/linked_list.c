
#include "linked_list.h"
#include <stddef.h>




/**
 * 把节点加到链表最后一个
 * 如果 head 为空,那么会设置新节点为头节点,因此需要设置指针的指针
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
 * 删除链表第一个
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
 * 链表大小
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

