/* Header file for LiveAnalyse.c
 * Created on 30 June 2014
 * Jamie Buckley
*/

#ifndef LIVEANALYSEHEADER
#define LIVEANALYSEHEADER

#include "List.h"
#include "ILCLinear.h"

typedef struct LivenessList
{
	int numTemporaries;
	int numInstructions;
	dlinklist** KILL;
	dlinklist** OUT;
	LIR* lir;
} LivenessList;

LivenessList* LiveAnalyse(LIR* lir);
int processSUCC(int i, LIRNode* l);
int processGEN(int i, LIRNode* l);
int processKILL(int i, LIRNode* l);
int processOUT(int i, LIRNode* l);
int processIN(int i, LIRNode* l);
#endif
