/* Performs graph colouring on all temporary variables to reduce number of temporaries to max register allowance.
 * If graph cannot be coloured, will determine which temporaries must be spilled (not including temporaries loaded from constants).  
 * These will then be loaded each time from memory before use, and written back to memory after assignment.
 * Created on 8 July 2014
 * Jamie Buckley
*/

#include "Graph.h"


GraphNode* newGraphNode()
{
	GraphNode* result = malloc(sizeof(GraphNode));
	result->edges=newlist();
	return result;
}
