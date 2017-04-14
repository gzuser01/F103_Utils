
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
void linked_add_last(struct Linked_List_Node **head,struct Linked_List_Node **new_node)
{

	struct Linked_List_Node *last_node = NULL;


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
 * ɾ�������һ��
 */
void linked_remove_first(struct Linked_List_Node **head)
{
	struct Linked_List_Node *ptr = NULL;
	
	if(head == NULL || (*head) == NULL)
	{
		return;
	}
	
	if((*head)->next == NULL)
	{
		*head = NULL;
		return;
	}
	ptr = (*head)->next;
	(*head)->next = NULL;
	*head = ptr;
}





#endif

