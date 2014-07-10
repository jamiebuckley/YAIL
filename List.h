/*
 * Linked list header for YAIL
 * Created on 16 April 2014
 * Jamie Buckley
 *
*/

#ifndef LISTFILE
#define LISTFILE

#include <stdlib.h>

typedef struct node
{
    void* data;
	struct node* prev;
	struct node* next;
} node;

typedef struct dlinklist
{
	int size;
	node* start;
	node* end;
} dlinklist;

typedef int(*nodeOp)(void*);
typedef int(*nodeCompareOp)(node*, void*);

dlinklist* newlist();

int append(dlinklist* list, void* data);
int prepend(dlinklist* list, void* data);
int insert(dlinklist* list, node* thisNode, void* data, int before);
int foreach(dlinklist* list, nodeOp callback);
int list_remove(dlinklist* list, void* data, nodeOp removeFunc);
int list_remove_comparator(dlinklist* list, void* data, nodeOp removeFunc, nodeCompareOp comparator, void* comparison);
int list_remove_node(dlinklist* list, node* thisNode, nodeOp removeFunc);

int prepend_all(dlinklist* list, dlinklist* addList);
int append_all(dlinklist* list, dlinklist* addList);

typedef int (*deleteItemPtr)(void* item);
int list_delete(dlinklist* list, deleteItemPtr deleteFunc);

int list_contains(dlinklist* list, void* item, size_t size);

void** listToArray(dlinklist* list);

#endif
