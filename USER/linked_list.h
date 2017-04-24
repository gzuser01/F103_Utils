
#ifndef __LINKED_LIST_H
#define	__LINKED_LIST_H


struct Linked_List_Node
{
	unsigned char *c;
	struct Linked_List_Node *next;
	
};



/**
 * 把节点加到链表最后一个
 * 如果 head 为空,那么会设置新节点为头节点,因此需要设置指针的指针
 */
void linked_add_last(struct Linked_List_Node **head,struct Linked_List_Node **new_node,struct Linked_List_Node **var_node);


/**
 * 删除链表第一个
 */
void linked_remove_first(struct Linked_List_Node **head,struct Linked_List_Node **var_node);

/**
 * 链表大小
 */
unsigned int linked_size(struct Linked_List_Node *head,struct Linked_List_Node *var_node);



#endif

