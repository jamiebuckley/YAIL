/* Takes a struct containing KILL and OUT lists and creates an interference graph
 * Created on 9 July 2014
 * Jamie Buckley
*/

#include "Colouring.h"
#include <string.h>

/* For each item in the KILL list at position i, connect it to each of the OUT nodes at position i
 * Colour the graph
 * If the graph cannot be coloured, choose a node to be spilled.  Insert spill code, and redo the live analysis
 */

dlinklist* colourGraph;
int numTemporaries;

int readGraph(ColourInfo* colourInfo)
{	
	printf("GRAPH:\n");
	node* current = colourGraph->start;
	
	while(current != NULL)
	{
		GraphNode* thisNode = current->data;
		ColourNode* tN = thisNode->data;

		printf("Node %d Colour %d Edges: ", tN->tempNum, tN->colour);
		colourInfo->tempColours[tN->tempNum] = tN->colour;

		node* currentEdge = thisNode->edges->start;
		while(currentEdge != NULL)
		{
			GraphNode* gN = currentEdge->data;
			ColourNode* cN = gN->data;
			printf("%d ", cN->tempNum);
			currentEdge = currentEdge->next;
		}
		printf("\n");
		current = current->next;
	}
}

int connectKILLandOUT(GraphNode* thisNode, dlinklist* OUT)
{
	node* current = OUT->start;
	while(current != NULL)
	{
		int* thatTemp = current->data; 
		graph_connect(thisNode, elementAt(colourGraph, *thatTemp));
		current = current->next;
	}
}

int destroyColourGraph()
{
	node* current = colourGraph->start;

	while(current != NULL)
	{
		node* next = current->next;
		
		GraphNode* g = current->data;
		ColourNode* c = g->data;

		free(c); c=NULL;
		free(g); g=NULL;
		free(current); current=NULL;
		current = next;
	}

	free(colourGraph);
	colourGraph = NULL;
}

int BuildGraph(LivenessList* livenessList)
{
	numTemporaries=livenessList->numTemporaries;
	colourGraph = newlist();
	for(int i = 0; i < livenessList->numTemporaries; i++)
	{
		GraphNode* g = newGraphNode();	
		ColourNode* n = malloc(sizeof(ColourNode));
		n->tempNum = i;
		n->colour=-1;
		g->data=n;
		append(colourGraph, g);
	}

	/* For each instruction i in the livenessList */
	for(int i = 0; i < livenessList->numInstructions; i++)
	{
		node* current = livenessList->KILL[i]->start;
		/* For each KILL variable in i */
		while(current != NULL)
		{
			int* thisTemp = current->data;
			connectKILLandOUT(elementAt(colourGraph, *thisTemp), livenessList->OUT[i]);	
			current = current->next;
		}
	}
}

int findColour(dlinklist* colourGraph, GraphNode* thisNode, int MAX_COLOUR)
{
	char* pColours = malloc(MAX_COLOUR);
	memset(pColours, -1, MAX_COLOUR);

	node* edge = thisNode->edges->start;
	while(edge != NULL)
	{
		GraphNode* gN = edge->data;
		ColourNode* cN = gN->data;	
		if(cN->colour >= 0)
			pColours[cN->colour]=1;

		edge = edge->next;
	}

	int result = -1;
	for(int i = 0; i < MAX_COLOUR; i++)
	{
		if(pColours[i] == -1)
		{
			result = i;
			break;
		}
	}

	free(pColours);
	return result;
}

ColourInfo* Colour(LivenessList* livenessList, int MAX_COLOUR)
{
	dlinklist* nodeStack = newlist();
	BuildGraph(livenessList);
	node* current;

	while(colourGraph->size > 0)
	{
		current = colourGraph->start;
		int modified = 0;

		while(current != NULL)
		{
			node* nextNode = current->next;
			GraphNode* g = current->data;
			int numEdges = g->edges->size;
		
			if(numEdges < MAX_COLOUR)
			{
				modified = 1;
				popGraphNode(colourGraph, g);
				append(nodeStack, g);
			}
			current = nextNode; 
		}

		if(modified == 0)
		{
			GraphNode* g = colourGraph->start->data;
			popGraphNode(colourGraph, g);
			append(nodeStack, g);
		}
	}

	current = nodeStack->start;
	while(current != NULL)
	{
		GraphNode* gN = current->data;
		ColourNode* cN = gN->data;
		pushGraphNode(colourGraph, current->data);
		int colour = findColour(colourGraph, current->data, MAX_COLOUR);

		if(colour == -1)
		{
			//spill the node
			//assign it a stack location
			//after each reference to this node/temporary
			//write it to the stack
		}

		cN->colour = colour;
		current = current->next;
	}

	ColourInfo* colourInfo = malloc(sizeof(ColourInfo));
	colourInfo->colourGraph = colourGraph;
	colourInfo->livenessList = livenessList;
	colourInfo->tempColours = malloc(sizeof(int) * livenessList->numTemporaries);

	readGraph(colourInfo);

	return colourInfo;
}
