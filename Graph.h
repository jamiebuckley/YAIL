/* Header for Graph.c
 * Created on 8 July 2014
 * Jamie Buckley
*/

#include <stdlib.h>
#include <stdio.h>
#include "List.h"

typedef struct GraphNode
{
	void* data;
	dlinklist* edges;
} GraphNode;

GraphNode* newGraphNode();
int graph_connect(GraphNode* A, GraphNode* B);
int graph_disconnect(GraphNode* A, GraphNode* B);
int removeGraphNode(dlinklist* graph, GraphNode* thisNode);
int popGraphNode(dlinklist* graph, GraphNode* thisNode);
int pushGraphNode(dlinklist* graph, GraphNode* thisNode);
