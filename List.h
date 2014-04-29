/*
 * Linked list header for YAIL
 * Created on 16 April 2014
 * Jamie Buckley
 *
*/

#ifndef LISTFILE
#define LISTFILE

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
	int size;
	node* start;
	node* end;
} dlinklist;

dlinklist* newlist();

int append(dlinklist* list, DATA_TYPE* data);
int insert(dlinklist* list, node* thisNode, DATA_TYPE* data, int before);

#endif
