/* header for Colouring.c
 * Created on 9 July 2014
 * Jamie Buckley
*/

#ifndef COLOURING_HEADER
#define COLOURING_HEADER


#include <stdlib.h>
#include <stdio.h>
#include "LiveAnalyse.h"
#include "List.h"
#include "Graph.h"

typedef struct ColourInfo
{
	dlinklist* colourGraph;
	LivenessList* livenessList;
	int* tempColours;
} ColourInfo;

typedef struct ColourNode
{
	int tempNum;
	int colour;
} ColourNode;

ColourInfo* Colour(LivenessList* livenessList, int MAX_COLOUR);

#endif
