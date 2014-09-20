/* Initial Compiler FrontEnd for YAIL
 * Created 10th May 2014
 * Jamie Buckley
 *
 * These functions are responsible for taking the Parser constructed AST
 * and performing symbol table construction, and type checking on the AST.
 *
 * The first is done by looking at each EQ node, and calculating the type of the RHS of said node.
 * If the LHS node already exists in the Symbol table, it will need to be checked whether the new
 * assignment type matches the previous type.  If this is true, assignment can be performed normally
 *
 * If the type does not match, the type of the variable will need to be modified from this point on.
 *
 * For this to be possible, each value in the symbol table will need to be a list of values, representing
 * the type and value of the variable at the current point in time (The current statement number)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BinaryTree.h"
#include "Compile.h"
#include "LiveAnalyse.h"
#include "Colouring.h"


HashMap* symbolTable;
int nextAddress = 0;

int verbose=0;

int main(int argc, char** argv)
{	
	/* perform basic checks on input filename */
	/* DO THIS LATER */
	/*  */
	
	for(int i = 0; i < (argc); i++)
	{
		if(!strcmp("-v", argv[i]))
		{
			printf("Verbose Mode Enabled\n");
			verbose=1;	
		}
	}

	/* Create the symbol table HashMap */
	symbolTable=newHashMap(64);


	/* Load the Abstract Syntax Tree into the AST linked list */
	dlinklist* AST;
	AST = parse(argv[1]);
	processAST(AST);

	/* if verbose mode, print the tree and symbol table */
	if(verbose) 
	{ 
		printAST(AST);
		printSymbolTable(symbolTable);
	}	

	LIR* lir = createLinearIR(symbolTable, AST);	
    LivenessList* liveness = LiveAnalyse(lir);
	ColourInfo* colourInfo = Colour(liveness, 8);

	if (colourInfo == NULL)
	{
		printf("Spill\n");
	}

	node* current = lir->IRList->start;
	while(current != NULL)
	{
		LIRNode* lirNode = current->data;	
		LIROperand* op1 = lirNode->operand1;
		LIROperand* op2 = lirNode->operand2;
		
		if(op1 != NULL && op1->type == TEMP)
		{
			op1->type = REGISTER;
			op1->value = colourInfo->tempColours[op1->value];
		}
		if(op2 != NULL && op2->type == TEMP)
		{
			op2->type = REGISTER;
			op2->value = colourInfo->tempColours[op2->value];
		}
		current = current->next;
	}

	if(verbose)
	{
		printf("LIR:\n");
		printLIR(lir->IRList, stdout, 1);
	}

	char* outputFileName = getNameWithoutExtension(argv[1]);
	if(outputFileName == NULL)
		printf("Error generating output file name\n");

	FILE* f = fopen(strcat(outputFileName, ".yavm"), "w");
	if(f==NULL)	printf("Error opening output file\n");
	else
		printLIR(lir->IRList, f, 0);

}

int printAST(dlinklist* AST)
{
	printf("AST TREE AS XML:\n");
	node* current = AST->start;	
	while(current!=NULL)
	{
		printXML((BinaryTreeNode*)current->data, 0);
		current=current->next;
	}
	printf("\n");

}

int printSymbolTable(HashMap* symbolTable)
{
	printf("SYMBOL TABLE:\n");
	dlinklist* keys = symbolTable->keys;

	node* keyNode = keys->start;

	while(keyNode != NULL)
	{
		HashMapEntry* hme = hashMap_get(symbolTable, (char*)(keyNode->data));	
		SymTabEntry* ste = hme->data;
		printf("Variable: %s	Type: %d    Address: %d\n", ste->name, ste->type, ste->address);

		keyNode=keyNode->next;
	}
	printf("\n");
	return 0;
}

int doOp(int leftVal, int rightVal, int operation)
{
	switch(operation)
	{
		case PLUS:
			return leftVal + rightVal;
		case MINUS:
			return leftVal - rightVal;
		case TIMES:
			return leftVal * rightVal;
		case DIV:
			return leftVal / rightVal;
		case GT:
			return leftVal > rightVal;
		case LT:
			return leftVal < rightVal;
	}
}

//if a nodes hierarchy type is NUM, and it has left and right nodes, it can be collapsed
//if a hierarchy type is not NUM, 
int collapseNodes(BinaryTreeNode* thisNode)
{
	ASTNode* currentData = thisNode->data;
	
	if(thisNode->isLeaf)
	{
		if(currentData->type == NUM)
			return 1;
		else
			return 0;	
	}
	else
	{
		BinaryTreeNode* leftNode = thisNode->left;
		BinaryTreeNode* rightNode = thisNode->right;
		ASTNode* leftData = leftNode->data;
		ASTNode* rightData = rightNode->data;

		if(leftData->type == NUM && rightData->type == NUM)
		{
			int val = atoi(leftData->value);
			int val2 = atoi(rightData->value);
			int result = doOp(val, val2, currentData->type);

			deleteBTNode(thisNode->left, freeASTNode);
			deleteBTNode(thisNode->right, freeASTNode);
			thisNode->isLeaf = 1;

			currentData->type = NUM;
			free(currentData->value);

			currentData->value = malloc(10*sizeof(char));
			sprintf(currentData->value, "%d", result);
			currentData->textType = "Number";
			return 1;
		}
		else
		{
			int succ = 0;
			succ += collapseNodes(leftNode);
			succ += collapseNodes(rightNode);
			
			if(succ >= 2)
			{
				collapseNodes(leftNode);
				collapseNodes(rightNode);
			}

			return succ;
		}
	}
}



int processAST(dlinklist* AST)
{
	node* thisNode = AST->start;

	while(thisNode != NULL)
	{
		BinaryTreeNode* currentNode=thisNode->data;
		makeSymTab(currentNode);
		collapseNodes(thisNode->data);
		thisNode = thisNode->next;
	}
	return 0;
}
/*
 * Walk tree and look for all variable declarations
 * If currentnode isn't leaf (and isn't a while loop)
 * Check type of left and right tree, and set current type to (operation between TYPEA and TYPEB)
 * if current is leaf, return that type
 * if current is EQ, set (or create) Var with name (lefttree) and type (righttree)
 */
int makeSymTab(BinaryTreeNode* AST)
{
	ASTNode* currentData = AST->data;

	if(currentData->type==WHI || currentData->type == IF)
	{
		processAST(AST->right->data);
	}

	//If current isn't leaf, process left and right
	if(currentData->type==EQ)
	{
		BinaryTreeNode* varNode = AST->left;
		BinaryTreeNode* valNode = AST->right;

		ASTNode* varData = varNode->data;
		ASTNode* valData = valNode->data;

		HashMapEntry* retrievedEntry = hashMap_get(symbolTable, varData->value);

		if(retrievedEntry == NULL)
		{
			SymTabEntry* entry = malloc(sizeof(SymTabEntry));

			entry->type=getType(valNode);
			entry->name=varData->value;
			entry->ASTRef=AST;
			entry->address=nextAddress;
			entry->regaddress=-1;
			nextAddress+=4;

			hashMap_put(symbolTable, varData->value, entry);
		}
	}	
	return 0;
}

int getType(BinaryTreeNode* treeNode)
{
	ASTNode* currentData = treeNode->data;
	
	if(currentData->hierarchyType)
		return currentData->hierarchyType;

	int nodeType = getOpType(treeNode);
	currentData->hierarchyType=nodeType;
	return nodeType;
}

/*
 * Get the type at the current point of the tree
 * If it's a leaf, return the simple type
 * Otherwise, work out what the result of leftNode OP rightNode will be
 */
int getOpType(BinaryTreeNode* treeNode)
{
	ASTNode* currentData = treeNode->data;

	if (treeNode->isLeaf)
	{
		if(currentData->type==NUM)
			return INTEGER;

		else if(currentData->type==VAR)
		{
			return TVAR;
		}
	}

	int leftOpType=getOpType(treeNode->left);
	int rightOpType=getOpType(treeNode->right);

	if (leftOpType==rightOpType)
		return leftOpType;

	if (leftOpType == TVAR || rightOpType == TVAR)
		return TVAR;

	return INTEGER;
}

char* getNameWithoutExtension(char* filename)
{
	char* result;
	if((result = malloc(strlen(filename)+1))==NULL)
		return NULL;

	strcpy(result, filename);

	char* dot = strrchr(result, '.');

	if(dot!=NULL)
		*dot='\0';

	return result;
}
