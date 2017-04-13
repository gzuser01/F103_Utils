
#ifndef __LINKED_LIST_H
#define	__LINKED_LIST_H


struct Node
{
	unsigned char *c;
	struct Node *next;
	int state ; //0: 空闲； 1: 写入； 2：写完可读
	
};



/**
 * 把节点加到链表最后一个
 * 如果 head 为空,那么会设置新节点为头节点,因此需要设置指针的指针
 */
void linked_add_last(struct Node **head,struct Node **new_node)
{

	struct Node *last_node = NULL;


	//On first add node, head is NULL
	if(*head == NULL){
		*head = *new_node;
	}else{
		last_node = *head;
		while((last_node)->next != NULL)
		{
			last_node = (last_node)->next;
		}
		(last_node)->next = *new_node;
	}
	

}

/**
 * 返回链表大小
 */
int linked_list_size(struct Node *head)
{
	int i=0;
	struct Node *n = head;
	while(n != NULL)
	{
		n = n -> next;
		i++;
	}
	return i;
}

/**
 * 删除链表第一个
 */
void linked_remove_first(struct Node **head)
{
	struct Node *ptr = NULL;
	
	if(head == NULL || (*head) == NULL)
	{
		return;
	}
	
	(*head)->state = 0;//重置为空闲（读完，或写入时节点不足需要删除老的节点）
	
	if((*head)->next == NULL)
	{
		*head = NULL;
		return;
	}
	ptr = (*head)->next;
	(*head)->next = NULL;
	*head = ptr;
}

/**
 * 返回链表第一个
 * 如果没有返回 0
 */
unsigned char * linked_first(struct Node *head)
{
	if(head == NULL){
		return NULL;
	}
	return head->c;
}


/**
 * 如果没有空闲节点,需要删除第一个节点,因此需要设置指针的指针
 */
struct Node * linked_pick_up_node(struct Node **head,struct Node *nodes,int linked_len)
{
	int i;
	struct Node * ptr = NULL;
	
	for(i = 0;i< linked_len ;i++)
	{
		if(nodes[i].state == 0)
		{
			nodes[i].state = 1;
			ptr = & nodes[i];
			break;
		}
	}
	
	if(ptr == NULL)
	{
		ptr = *head;
		
		//释放 head,重置状态为 0 空闲
		linked_remove_first(head);
		
		if(ptr != NULL)
		{
			ptr->state = 1;//重置为写入状态
		}
		
		
	}
	return ptr;
	
}

#endif

