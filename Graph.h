/* Header for Graph.c
 * Created on 8 July 2014
 * Jamie Buckley
*/

#include <stdlib.h>
#include <stdio.h>
#include "List.h"

typedef struct GraphNode
{
	dlinklist* edges;
} GraphNode;
