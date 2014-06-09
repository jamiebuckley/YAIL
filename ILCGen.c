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
extern int verbose;

dlinklist* ILCList;

int makeILC(dlinklist* AST, FILE* f)
{
	ILCList = newlist();	

	node* current = AST->start;

	while(current != NULL)
	{
		outputNode(current->data, f);	
		current=current->next;
	}
}

int outputNode(BinaryTreeNode* thisNode, dlinklist* list, FILE* f)
{
	BinaryTreeNode* left = thisNode->left;
	BinaryTreeNode* right = thisNode->right;

	ASTNode* thisData = thisNode->data;
	int type = thisData->type;

	switch(type)
	{
		case EQ:
				break;
		case PLUS:
		case MINUS:
		case TIMES:
		case DIV:
				break;
		case VAR:
				break;
		case NUM:
				break;
		case IF:
				break;
		case WHI:
				break;
	}
}

int handleWhile(BinaryTreeNode* thisNode)
{
	//get the comparison statement
	//get the middle statements
	//prepend a conditional jump to the end based on the comparison statement
	
	dlinklist* statementList = thisNode->right->data; 
	node* n = statementList->start;

	int lastCommand = -1;
	while(n != NULL)
	{
		lastCommand = outputNode(n->data);
		n=n->next;
	}

}

int outputNodeTwo(BinaryTreeNode* thisNode, FILE* f)
{
	BinaryTreeNode* left = thisNode->left;
	BinaryTreeNode* right = thisNode->right;

	ASTNode* thisData = thisNode->data;
	int type = thisData->type;
	
	if(type==EQ)
	{
		ASTNode* varNodeData = left->data;
		char* varName = varNodeData->value;
		int res = outputNode(thisNode->right, f);		
		outputStoreVar(res, varName, f); 
	}
	else if(type==PLUS || type == MINUS || type == TIMES || type == DIV)
	{
		int l = outputNode(left, f); 
		int r = outputNode(right, f);
		return operand(thisData->textType, l, r, f);
	}
	else if(type==VAR)
	{
		return outputLoadVar(thisData->value, f);	
	}
	else if(type==NUM)
	{
		return outputLoadCon(thisData->value, f);
	}	
	else if(type==IF)
	{	
		dlinklist* stmtList = thisNode->right->data;
		node* n = stmtList->start;

		printf("IF\n");

		while(n != NULL)
		{
			outputNode(n->data, f);
			n=n->next;
		}

		printf("ENDIF\n");
	}
	else if(type==WHI)
	{
		dlinklist* stmtList = thisNode->right->data;
		node* n = stmtList->start;

		printf("WHILE\n");

		while(n != NULL)
		{
			outputNode(n->data, f);
			n=n->next;
		}

		printf("ENDWHILE\n");

	}

}

int outputLoadVar(char* varName, FILE* f)
{
	int num = lastNum;
	fprintf(f, "#%d LOAD @%s\n", num, varName);

	if(verbose)	
		printf("#%d LOAD @%s\n", num, varName);

	lastNum++;
	return num;
}

int outputLoadCon(char* val, FILE* f)
{
	int num = lastNum;
	fprintf(f, "#%d LOAD %s\n", num, val);
	
	if(verbose)
		printf("#%d LOAD %s\n", num, val);

	lastNum++;
	return num;
}

int outputStoreVar(int inputNum, char* varName, FILE* f)
{
	int num = lastNum;
	fprintf(f, "#%d STORE %d @%s\n", num, inputNum, varName); 

	if(verbose)
		printf("#%d STORE %d @%s\n", num, inputNum, varName); 

	lastNum++;
	return num;
}

int outputOperand(char* type, int R1, int R2, FILE* f)
{
	int num = lastNum;
	fprintf(f, "#%d %s %d %d\n", num, type, R1, R2);

	if(verbose)
	printf("#%d %s %d %d\n", num, type, R1, R2);

	lastNum++;
	return num;
}

int outputJump(char* type, int number, FILE* f)
{
	int num = lastNum;
	fprintf(f, "#%d %s %d\n", num, type, number);

	if(verbose)	
		printf("#%d %s %d\n", num, type, number);

   	lastNum++;   
	return num;
}
