/* 
 * Linked list test implementation for YAIL
 * Created on 16 April 2014
 * Jamie Buckley
*/

#include <stdio.h>
#include <stdlib.h>
#include "List.h"

/* Test Main Function
int main(int argc, char** argv)
{
    dlinklist* list = newlist();

    int* num = malloc(sizeof(int));
    int* num2 = malloc(sizeof(int));

    *num = 5;
    *num2 = 10;

    append(list, num);
    append(list, num2);

    node* current = list->start;
    while(current!=NULL)
    {
	printf("%d\n", *(current->thisnode));
	current=current->next;
    }
}
*/

dlinklist* newlist()
{
	dlinklist* l = malloc(sizeof(dlinklist));
	l->size = 0;
	l->start = NULL;
	l->end = NULL;
	return l;	
}

int append(dlinklist* list, void* data)
{
	node* node = malloc(sizeof(node));
	node->data = data;	
	
	list->size++;
	
	if(list->start == NULL)	
	{
		list->start = node;
		list->end = node;
		return 0;
	}

	list->end->next = node;
	node->prev = list->end;

	list->end = node;

	return 0;
}

int prepend(dlinklist* list, void* data)
{
	node* node = malloc(sizeof(node));
	node->data = data;
	list->size++;

	if (list->start == NULL)
	{
		list->start = node;
		list->end = node;
		return 0;
	}

	list->start->prev = node;
	node->next = list->start;
	list->start = node;
}

int insert(dlinklist* list, node* thisNode, void* data, int before)
{
	if(list->size==0)
	{
		append(list, data);
		return 0;
	}

	node* current = list->start;
	while(current != NULL)
	{
		if(current==thisNode)
		{
			node* newnode = malloc(sizeof(node));
			newnode->data = data;

			if(before)
			{
				current->prev->next=newnode;
				current->prev=newnode;
			}
			else
			{
				current->next->prev=newnode;
				current->next=newnode;
			}
			list->size++;
			return 0;
		}
		if(current==list->end)
			return -1;

		current=current->next;
	}
}

int foreach(dlinklist* list, nodeOp callback)
{
	if(!list->size)
		return -1;

	node* current = list->start;	
	while(current!=NULL)
	{
		callback(current->data);
		current=current->next;
	}
	return 0;
}
