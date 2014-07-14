/* Performs graph colouring on all temporary variables to reduce number of temporaries to max register allowance.
 * If graph cannot be coloured, will determine which temporaries must be spilled (not including temporaries loaded from constants).  
 * These will then be loaded each time from memory before use, and written back to memory after assignment.
 * Created on 8 July 2014
 * Jamie Buckley
*/

#include "Graph.h"
#include <string.h>

GraphNode* newGraphNode()
{
	GraphNode* result = malloc(sizeof(GraphNode));
	result->edges=newlist();
	return result;
}

int graph_connect(GraphNode* A, GraphNode* B)
{
	if(!memcmp(A, B, sizeof(GraphNode)))
		return 0;

	if(!list_contains(A->edges, B, sizeof(GraphNode)))
		append(A->edges, B);

	if(!list_contains(B->edges, A, sizeof(GraphNode)))
		append(B->edges, A);

	return 0;
}

int graph_disconnect(GraphNode* A, GraphNode* B)
{
	list_delete(A->edges, B, sizeof(GraphNode));
	list_delete(B->edges, A, sizeof(GraphNode));
	return 0;
}

int removeGraphNode(dlinklist* graph, GraphNode* thisNode)
{
	node* current = thisNode->edges->start;
	
	while(current != NULL)
	{
		GraphNode* connectedNode = current->data;
		graph_disconnect(thisNode, connectedNode);
		current = current->next;
	}

	list_delete(graph, thisNode, sizeof(GraphNode));
	return 0;
}

int popGraphNode(dlinklist* graph, GraphNode* thisNode)
{
	node* current = thisNode->edges->start;

	while(current != NULL)
	{
		GraphNode* connectedNode = current->data;
		list_delete(connectedNode->edges, current->data, sizeof(GraphNode));
		current = current->next;
	}

	list_delete(graph, thisNode, sizeof(GraphNode));
	return 0;
}

int pushGraphNode(dlinklist* graph, GraphNode* thisNode)
{
	if(!list_contains(graph, thisNode, sizeof(GraphNode)))
	{
		append(graph, thisNode);	
		node* current = thisNode->edges->start;
		while(current != NULL)
		{
			graph_connect(thisNode, current->data);
			current = current->next;
		}
	}
}
