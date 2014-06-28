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
	node* addnode = malloc(sizeof(node));
	addnode->data = data;		
	list->size++;
	
	if(list->start == NULL)	
	{
		list->start = addnode;
		list->end = addnode;
		return 0;
	}

	list->end->next = addnode;
	addnode->prev = list->end;

	list->end = addnode;

	return 0;
}

int prepend(dlinklist* list, void* data)
{
	node* addnode = malloc(sizeof(node));
	addnode->data = data;
	list->size++;

	if (list->start == NULL)
	{
		list->start = addnode;
		list->end = addnode;
		return 0;
	}

	list->start->prev = addnode;
	addnode->next = list->start;
	list->start = addnode;
	return 0;
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
	return 0;
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

int list_remove(dlinklist* list, void* data, nodeOp removeFunc)
{
	node* thisNode;	
	node* current = list->start;

	while(current!=NULL)
	{
		if(current->data == data)
		{
			if(list->start == current) list->start=current->next;
			else if(list->end == current) list->end = current->prev;

			if(current->prev != NULL) current->prev->next = current->next;
			if(current->next != NULL) current->next->prev = current->prev;
			thisNode=current;

			if(removeFunc != NULL) removeFunc(thisNode->data);	
			free(thisNode);	

			list->size--;

			return 0;	
		}
		current=current->next;
	}		
	return -1;
}

int list_remove_comparator(dlinklist* list, void* data, nodeOp removeFunc, nodeCompareOp comparator, void* comparison)
{
	node* thisNode;	
	node* current = list->start;

	while(current!=NULL)
	{
		if(comparator(current, comparison))
		{
			if(list->start == current) list->start=current->next;
			else if(list->end == current) list->end = current->prev;

			if(current->prev != NULL) current->prev->next = current->next;
			if(current->next != NULL) current->next->prev = current->prev;
			thisNode=current;

			if(removeFunc != NULL) removeFunc(thisNode->data);	
			free(thisNode);	

			list->size--;
			if(list->size==0)
			{
				list->start=NULL;
				list->end=NULL;
			}

			return 0;	
		}
		current=current->next;
	}		
	return -1;
}

int list_remove_node(dlinklist* list, node* node, nodeOp removeFunc)
{
	list->size--;

	if(node->prev != NULL) node->prev->next = node->next;
	if(node->next != NULL) node->next->prev = node->prev;

	if(removeFunc != NULL) removeFunc(node->data);
	free(node);

	return 0;
}

int prepend_all(dlinklist* list, dlinklist* addList)
{
	node* current = addList->end;
	
	while(current != NULL)
	{
		prepend(list, current->data);
		current = current->prev;
	}

	return 0;
}

int append_all(dlinklist* list, dlinklist* addList)
{
	node* current = addList->start;

	while(current != NULL)
	{
		append(list, current->data);
		current = current->next;
	}

	return 0;
}

int list_delete(dlinklist* list)
{
	node* current = list->start;

	while(current != NULL)
	{
		free(current->data);
		node* old = current;
		current=current->next;
		free(old);
	}

	return 0;
}
