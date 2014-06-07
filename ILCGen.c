/*
 * Functions to generate Intermediate Location Code - A linear IR that better suits being translated to
 * Register code
 *
 * Created on 12th May 2014
 * Jamie Buckley
 *
 *
 * This code should take an AST, and turn it into ILC.  ILC is specified below:
 *
 * LOAD R <@x | CONSTANT>
 * STORE R <@x>
 * ADD | SUB | MUL | DIV R R
 * COMPEQ R R
 * COMPGT R R
 * JUMP #
 * JUMPIF #
*/

#include <stdio.h>
#include <stdlib.h>
#include "ILCGen.h"

int lastNum = 0;

int printILC(dlinklist* AST)
{
	node* current = AST->start;

	while(current != NULL)
	{
		outputNode(current->data);	
		current=current->next;
	}
}

int outputNode(BinaryTreeNode* thisNode)
{
	BinaryTreeNode* left = thisNode->left;
	BinaryTreeNode* right = thisNode->right;

	ASTNode* thisData = thisNode->data;
	int type = thisData->type;
	
	if(type==EQ)
	{
		ASTNode* varNodeData = left->data;
		char* varName = varNodeData->value;
		int res = outputNode(thisNode->right);		
		outputStoreVar(res, varName); 
	}
	else if(type==PLUS || type == MINUS || type == TIMES || type == DIV)
	{
		int l = outputNode(left); 
		int r = outputNode(right);
		return operand(thisData->value, l, r);
	}
	else if(type==VAR)
	{
		return outputLoadVar(thisData->value);	
	}
	else if(type==NUM)
	{
		return outputLoadCon(thisData->value);
	}
}

int outputLoadVar(char* varName)
{
	int num = lastNum;
	printf("#%d LOAD @%s\n", num, varName);
	lastNum++;
	return num;
}

int outputLoadCon(char* val)
{
	int num = lastNum;
	printf("%d LOAD %s\n", num, val);
	lastNum++;
	return num;
}

int outputStoreVar(int inputNum, char* varName)
{
	int num = lastNum;
	printf("#%d STORE %d @%s\n", num, inputNum, varName); 
	lastNum++;
	return num;
}

int operand(char* type, int R1, int R2)
{
	int num = lastNum;
	printf("#%d %s %d %d\n", num, type, R1, R2);
	lastNum++;
	return num;
}

int jump(char* type, int number)
{
	int num = lastNum;
	printf("%d %s %d\n", num, type, number);
   	lastNum++;   
	return num;
}
