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
#include "HashMap.h"
#include "Compile.h"

int lineNum = 0;
int nextRegNum = 0;
extern int verbose;

dlinklist* ILCList;

extern HashMap* symbolTable;

char* cNames[] = {"LOAD", "STORE", "ADD", "SUB", "MUL", "DIV", "COMPEQ", "COMPGT", "JUMP", "JUMPIF", "JUMPNIF", "END"};

int makeILC(dlinklist* AST, FILE* f)
{	
	ILCList = newlist();	
	node* current = AST->start;

	while(current != NULL)
	{
		ILCLine* thisLine = outputNode(current->data);	
		current=current->next;
	}

	ILCLine* endLine = malloc(sizeof(ILCLine));
	endLine->operation=END;
	endLine->var1 = -1;
	endLine->var2 = -1;
	endLine->lnum = nextLine();

	append(ILCList, endLine);

	node* currentLine = ILCList->start;
	while(currentLine != NULL)
	{
		ILCLine* thisLine = currentLine->data;
		fprintf(f, "%d %d %d\n", thisLine->operation, thisLine->var1, thisLine->var2);	
		if(verbose)
		printf("#%d: %s %d %d\n",thisLine->lnum, cNames[thisLine->operation], thisLine->var1, thisLine->var2);	
		currentLine = currentLine->next;
	}
	return 0;
}

ILCLine* outputNode(BinaryTreeNode* thisNode)
{
	BinaryTreeNode* left = thisNode->left;
	BinaryTreeNode* right = thisNode->right;

	ASTNode* thisData = thisNode->data;
	int type = thisData->type;

	ILCLine* current;

	switch(type)
	{
		case NUM:
				current = handleNum(thisNode);
				break;
		case VAR:
				current = handleVar(thisNode);
				break;
		case EQ:
				current = handleAssign(thisNode);
				break;
		case PLUS:
				current = handleOp(thisNode, CADD);
				break;
		case MINUS:
				current = handleOp(thisNode, CSUB);
				break;
		case TIMES:
				current = handleOp(thisNode, CMUL);
				break;
		case DIV:
				current = handleOp(thisNode, CDIV);
				break;
		case IF:
				current = handleIf(thisNode);
				break;
		case WHI:
				current = handleWhile(thisNode);
				break;
		case GT:
				current = handleGreater(thisNode);
				break;
		case LT:
				current = handleLess(thisNode);
				break;
	}

	return current;
}

ILCLine* handleNum(BinaryTreeNode* thisNode)
{
	ILCLine* thisLine = malloc(sizeof(ILCLine));
	thisLine->operation=CLOAD;
	thisLine->var1 = nextReg();
	thisLine->var2 = atoi(((ASTNode*)(thisNode->data))->value);

	thisLine->lnum = nextLine();
	append(ILCList, thisLine);
	return thisLine;
}

ILCLine* handleVar(BinaryTreeNode* thisNode)
{	
	char* varName = (((ASTNode*)thisNode->data)->value);

	ILCLine* thisLine = malloc(sizeof(ILCLine));
	thisLine->operation=CLOAD;
	thisLine->var1 = nextReg();
	SymTabEntry* varEntry = hashMap_get(symbolTable, varName)->data;
	thisLine->var2 = varEntry->address;

	thisLine->lnum = nextLine();
	append(ILCList, thisLine);
	return thisLine;
}

ILCLine* handleAssign(BinaryTreeNode* thisNode)
{	
	char* varName = (((ASTNode*)thisNode->left->data)->value);

	ILCLine* thisLine = malloc(sizeof(ILCLine));
	thisLine->operation=CSTORE;

	SymTabEntry* varEntry = hashMap_get(symbolTable, varName)->data;
	thisLine->var1 = varEntry->address;
	thisLine->var2 = outputNode(thisNode->right)->var1;

	thisLine->lnum = nextLine();
	append(ILCList, thisLine);
	return thisLine;
}

ILCLine* handleOp(BinaryTreeNode* thisNode, int opType)
{
	ILCLine* thisLine = malloc(sizeof(ILCLine));
	thisLine->operation = opType;
	thisLine->var1 = outputNode(thisNode->left)->var1;
	thisLine->var2 = outputNode(thisNode->right)->var1;	

	thisLine->lnum = nextLine();
	append(ILCList, thisLine);
	return thisLine;
}

ILCLine* handleIf(BinaryTreeNode* thisNode)
{
	ILCLine* thisLine = malloc(sizeof(ILCLine));
	thisLine->operation=CJUMPIF;
	outputNode(thisNode->left);
	thisLine->var1=-1;

	thisLine->lnum = nextLine();
	append(ILCList, thisLine);

	dlinklist* statements = ((BinaryTreeNode*)(thisNode->right))->data;
	node* current = statements->start;

	int lastLine = -1;
	while(current != NULL)
	{
		ILCLine* newLine = outputNode(current->data);
		current = current->next;
		lastLine=lineNum;
	}

	thisLine->var1=lastLine;

	return thisLine;
}

ILCLine* handleWhile(BinaryTreeNode* thisNode)
{
	ILCLine* thisLine = malloc(sizeof(ILCLine));
	thisLine->operation=CJUMPIF;
	outputNode(thisNode->left);
	thisLine->var1=-1;
	thisLine->var2=-1;

	thisLine->lnum = nextLine();
	append(ILCList, thisLine);

	dlinklist* statements = ((BinaryTreeNode*)(thisNode->right))->data;
	node* current = statements->start;

	int beginningLine = lineNum;
	while(current != NULL)
	{
		ILCLine* newLine = outputNode(current->data);
		current = current->next;
	}

	ILCLine* endLine = malloc(sizeof(ILCLine));
	endLine->operation=CJUMPNIF;
	outputNode(thisNode->left);
	endLine->var1=beginningLine;
	endLine->var2=-1;
	endLine->lnum=nextLine();
	append(ILCList, endLine);

	thisLine->var1=lineNum;

	return thisLine;
}

ILCLine* handleGreater(BinaryTreeNode* thisNode)
{
	ILCLine* thisLine = malloc(sizeof(ILCLine));
	thisLine->operation=CCOMPGT;
	thisLine->var1 = outputNode(thisNode->left)->var1;
	thisLine->var2 = outputNode(thisNode->right)->var1;

	thisLine->lnum = nextLine();
	append(ILCList, thisLine);
	return thisLine;
}

ILCLine* handleLess(BinaryTreeNode* thisNode)
{
	ILCLine* thisLine = malloc(sizeof(ILCLine));
	thisLine->operation=CCOMPGT;
	thisLine->var2 = outputNode(thisNode->left)->var1;
	thisLine->var1 = outputNode(thisNode->right)->var1;

	thisLine->lnum = nextLine();
	append(ILCList, thisLine);
	return thisLine;
}

ILCLine* handleEqual(BinaryTreeNode* thisNode)
{
	return NULL;
}

int nextReg()
{
	int result = nextRegNum;
	nextRegNum++;
	return result;
}

int nextLine()
{
	int result = lineNum;
	lineNum++;
	return result;
}
