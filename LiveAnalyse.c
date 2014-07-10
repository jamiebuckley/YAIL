/* Processes the linear ILC produced by ILCLinear, and computes liveness analysis
 * produces lists of IN/OUT/KILL for register collision
 *
 * Created on 30 June 2014
 * Jamie Buckley
*/

#include "LiveAnalyse.h"
#include <stdio.h>
#include <stdlib.h>

dlinklist** SUCC;
dlinklist** GEN;
dlinklist** KILL;
dlinklist** IN;
dlinklist** OUT;

int numInstructions;

int printNum(void* data)
{
	int* a = data;
	printf("%d,", *a);
}

void printTables(int numInstructions)
{
	for(int i = 0; i < numInstructions; i++)
	{
		printf("#%d:\t", i);
		foreach(SUCC[i], &printNum);
		printf("\t");
		foreach(GEN[i], &printNum);
		printf("\t");
		foreach(KILL[i], &printNum);
		printf("\t");
		foreach(OUT[i], &printNum);
		printf("\t\t");
		foreach(IN[i], &printNum);
		printf("\n");
	}	
}

LivenessList* LiveAnalyse(dlinklist* ILCLinear)
{	
	numInstructions = ILCLinear->size;

	SUCC = malloc(numInstructions * sizeof(dlinklist));
	GEN = malloc(numInstructions * sizeof(dlinklist));
	KILL = malloc(numInstructions * sizeof(dlinklist));
	IN = malloc(numInstructions * sizeof(dlinklist));
	OUT = malloc(numInstructions * sizeof(dlinklist));

	LIRNode** IR = (LIRNode**)listToArray(ILCLinear);
	//delete original list

	for(int i = numInstructions-1; i >=0; i--)
	{
		SUCC[i] = newlist();
		GEN[i] = newlist();
		KILL[i] = newlist();
		IN[i] = newlist();
		OUT[i] = newlist();

		processSUCC(i, IR[i]);
		processGEN(i, IR[i]);
		processKILL(i, IR[i]);
	}

	while(1)
	{
		int numAdded = 0;
		for(int i = numInstructions-1; i >=0; i--)
		{
			numAdded += processOUT(i, IR[i]);
			numAdded += processIN(i, IR[i]);
		}
		if(numAdded==0) break;
	}

	printTables(numInstructions);

	LivenessList* result = malloc(sizeof(LivenessList));
	result->KILL = KILL;
	result->OUT = OUT;
	return result;
}

int processSUCC(int i, LIRNode* l)
{
	int* nextDest,* jumpDest;	
	nextDest = malloc(sizeof(int));
	*nextDest = i+1;

	switch(l->type)
	{
		case CJUMP:
		case CJUMPIF:
		case CJUMPNIF:;
			jumpDest = malloc(sizeof(int));
			*jumpDest = l->operand1->value;
			append(SUCC[i], jumpDest);
			break;
		default:
			break;
	}	
	append(SUCC[i], nextDest);
}

void addOpsGEN(int i, LIRNode* l)
{
	int* a = malloc(sizeof(int));
	int* b = malloc(sizeof(int));
	if(l->operand1 != NULL)
	{
		*a = l->operand1->value;
		append(GEN[i], a);
		append(KILL[i], a);
	}
	if(l->operand2 != NULL)
	{
		*b = l->operand2->value;
		append(GEN[i], b);
	}
}

void addLoadGEN(int i, LIRNode* l)
{
	int* a = malloc(sizeof(int));
	*a = l->operand1->value;
	append(KILL[i], a);
}

int processGEN(int i, LIRNode* l)
{
	switch(l->type)
	{
			case CADD:
			case CSUB:
			case CMUL:
			case CDIV:
			case CCOMPEQ:
			case CCOMPGT:
				addOpsGEN(i, l);
				break;
			case CLOAD:
				addLoadGEN(i, l);
				break;
			default:
				break;
	}
}

int processKILL(int i, LIRNode* l)
{
	if(l->type != CSTORE)
		return 0;

	int* a = malloc(sizeof(int));
	*a = l->operand2->value;
	append(GEN[i], a);
	append(KILL[i], a);
	return 0;
}

int processOUT(int i, LIRNode* l)
{
	node* current = SUCC[i]->start;
	int numAdded = 0;

	while(current != NULL)
	{	
		int* j = current->data;	
		if(*j==numInstructions)
		{
			current = current->next;
			continue;
		}
		node* currentJIN = IN[*j]->start;
		while(currentJIN != NULL)
		{
			int* jInVal = currentJIN->data;	
			if(!list_contains(OUT[i], jInVal, sizeof(int)))
			{
				append(OUT[i], jInVal);
				numAdded++;
			}

			currentJIN = currentJIN->next;
		}

		current = current->next;
	}
	return numAdded;
}

int processIN(int i, LIRNode* l)
{
	node* currentGen = GEN[i]->start;

	int numAdded = 0;
	while(currentGen != NULL)
	{
		int* currentGenData = currentGen->data;
		if(!list_contains(IN[i], currentGenData, sizeof(int)))
		{
			append(IN[i], currentGenData);
			numAdded++;
		}

		currentGen = currentGen->next;
	}

	node* currentOut = OUT[i]->start;
	while(currentOut != NULL)
	{
		int* currentOutData = currentOut->data;

		if(!list_contains(KILL[i], currentOutData, sizeof(int)) && !list_contains(IN[i], currentOutData, sizeof(int)))
		{
			append(IN[i], currentOutData);
			numAdded++;
		}

		currentOut=currentOut->next;
	}
	return numAdded;
}

