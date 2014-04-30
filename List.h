/*
 * Linked list header for YAIL
 * Created on 16 April 2014
 * Jamie Buckley
 *
*/

#ifndef LISTFILE
#define LISTFILE

typedef struct node
{
    void* data;
	struct node* prev;
	struct node* next;
} node;

typedef struct dlinklist
{
	int size;
	size_t elementSize;
	node* start;
	node* end;
} dlinklist;

typedef int(*nodeOp)(void*);

dlinklist* newlist();

int append(dlinklist* list, void* data);
int insert(dlinklist* list, node* thisNode, void* data, int before);
int foreach(dlinklist* list, nodeOp callback);

#endif
