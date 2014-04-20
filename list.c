/* 
 * Linked list test implementation for YAIL
 * Created on 16 April 2014
 * Jamie Buckley
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

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
	l->start = NULL;
	l->end = NULL;
	return l;	
}

int append(dlinklist* list, DATA_TYPE* data)
{
	node* node = malloc(sizeof(node));
	node->thisnode = data;	

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
