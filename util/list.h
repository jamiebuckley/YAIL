/*
 * Linked list header for YAIL
 * Created on 16 April 2014
 * Jamie Buckley
 *
*/

#ifndef DATA_TYPE
#define DATA_TYPE int
#endif

typedef struct node
{
	struct node* prev;
	struct node* next;
	DATA_TYPE* thisnode;
} node;

typedef struct dlinklist
{
	node* start;
	node* end;
} dlinklist;

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
