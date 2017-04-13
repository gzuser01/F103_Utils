
#ifndef __LINKED_LIST_H
#define	__LINKED_LIST_H


struct Node
{
	unsigned char *c;
	struct Node *next;
	int state ; //0: ���У� 1: д�룻 2��д��ɶ�
	
};



/**
 * �ѽڵ�ӵ��������һ��
 * ��� head Ϊ��,��ô�������½ڵ�Ϊͷ�ڵ�,�����Ҫ����ָ���ָ��
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
 * ���������С
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
 * ɾ�������һ��
 */
void linked_remove_first(struct Node **head)
{
	struct Node *ptr = NULL;
	
	if(head == NULL || (*head) == NULL)
	{
		return;
	}
	
	(*head)->state = 0;//����Ϊ���У����꣬��д��ʱ�ڵ㲻����Ҫɾ���ϵĽڵ㣩
	
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
 * ���������һ��
 * ���û�з��� 0
 */
unsigned char * linked_first(struct Node *head)
{
	if(head == NULL){
		return NULL;
	}
	return head->c;
}


/**
 * ���û�п��нڵ�,��Ҫɾ����һ���ڵ�,�����Ҫ����ָ���ָ��
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
		
		//�ͷ� head,����״̬Ϊ 0 ����
		linked_remove_first(head);
		
		if(ptr != NULL)
		{
			ptr->state = 1;//����Ϊд��״̬
		}
		
		
	}
	return ptr;
	
}

#endif

