
#ifndef __LINKED_LIST_H
#define	__LINKED_LIST_H

#include <stdlib.h>

struct Node
{
	unsigned char *c;
	struct Node *next;
};



/**
 * �Ѳ����ӵ������һ��
 */
void addFirst(struct Node *head,unsigned char *c)
{
	struct Node *new_node = (struct Node * )malloc( sizeof(struct Node) );
	new_node -> c = c;
	new_node -> next = head;
	//Set the first to head
	head = new_node;
}

/**
 * �Ѳ����ӵ��������һ��
 */
void addLast(struct Node *head,unsigned char *c)
{
	struct Node *new_node = (struct Node * )malloc( sizeof(struct Node) );
	struct Node *last_node = NULL;
	
	
	new_node->c = c;
	new_node->next = NULL;

	//On first add node, head is NULL
	if(head == NULL){
		head = new_node;
	}else{
		last_node = head;
		while(last_node->next != NULL)
		{
			last_node = last_node->next;
			
		}
		last_node->next = new_node;
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
void removeFirst(struct Node *head)
{
	struct Node *n = NULL;
	
	if(head == NULL)
	{
		return;
	}

	if(head -> next == NULL)
	{
		n = head;
		head = NULL;
		free(n->c);
		free(n);
		return;
	}

	n = head;
	head = head->next;
	free(n->c);
	free(n);
}

/**
 * ���������һ��
 * ���û�з��� 0
 */
unsigned char * first(struct Node *head)
{
	if(head == NULL){
		return NULL;
	}
	return head->c;
}

/**
 * �����������һ��
 * ���û�з��� 0
 */
unsigned char * last(struct Node *head)
{
	struct Node *n = head;
	//n is not NULL and the next is not NULL
	while(n != 0 && n -> next != NULL)
	{
		n = n -> next;
	}
	if(n == NULL){
		return NULL;
	}
	return n->c;
}

#endif

