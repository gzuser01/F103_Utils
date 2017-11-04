
#ifndef __LINKED_LIST_H
#define	__LINKED_LIST_H


struct Linked_List_Node
{
	/**
	 * 存储的数据，字符数组
	 */
	unsigned char *C;
	/**
	 * 指向下一个节点
	 */
	struct Linked_List_Node *Next;
	
};



/**
 * 把节点加到链表最后一个
 * 如果 head 为空,那么会设置新节点为头节点,因此需要设置指针的指针
 */
void Linked_Add_Last(struct Linked_List_Node **Head,struct Linked_List_Node **New_Node,struct Linked_List_Node **Var_Node);


/**
 * 删除链表第一个
 */
void Linked_Remove_First(struct Linked_List_Node **Head,struct Linked_List_Node **Var_Node);

/**
 * 链表大小
 */
unsigned int Linked_Size(struct Linked_List_Node *Head,struct Linked_List_Node *Var_Node);



#endif

