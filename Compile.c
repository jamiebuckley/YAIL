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
#include "Compile.h"
#include <string.h>


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

	symbolTable=newHashMap(64);

	dlinklist* AST;
	AST = parse(argv[1]);
	processAST(AST);

	if(verbose)
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

	if(verbose)
	{
		printSymbolTable(symbolTable);
		printf("\n");
	}
	
	if(verbose)
		printf("ILC OUTPUT\n");

	char* outputFileName = getNameWithoutExtension(argv[1]);
	if(outputFileName == NULL)
	{
		printf("Error generating output file name\n");
	}

	FILE* f = fopen(strcat(outputFileName, ".yavm"), "w");
	//free(outputFileName);

	if(f==NULL)
	{
		printf("Error opening output file\n");
	}
	else
	{
		if(verbose)
			printf("Opened file for writing\n");
	}

	makeILC(AST, f);



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

int processAST(dlinklist* AST)
{
	node* thisNode = AST->start;

	while(thisNode != NULL)
	{
		BinaryTreeNode* currentNode=thisNode->data;
		makeSymTab(currentNode);
		thisNode = thisNode->next;
	}
	return 0;
}

int makeSymTab(BinaryTreeNode* AST)
{
	//Walk tree and look for all variable declarations
	//If currentnode isn't leaf (and isn't a while loop)
	//Check type of left and right tree, and set current type to (operation between TYPEA and TYPEB)
	//if current is leaf, return that type
	//if current is EQ, set (or create) Var with name (lefttree) and type (righttree)
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

int getOpType(BinaryTreeNode* treeNode)
{
	//if tree is a loop or conditional, ignore it
	//otherwise it's an expression
	//if its a leaf, return the type
	//if its not, return the result of the left leaf OP the right leaf

	ASTNode* currentData = treeNode->data;

	if (treeNode->isLeaf)
	{
		if(currentData->type==NUM)
			return INTEGER;

		else if(currentData->type==VAR)
		{
			HashMapEntry* varResult = hashMap_get(symbolTable, currentData->value);	

			if(varResult==NULL)
			{
				//Error, found undefined value
				printf("Error: %s is undefined\n", currentData->value);
				return 0;
			}

			SymTabEntry* entry = varResult->data;
			return entry->type;
		}
	}

	int leftOpType=getOpType(treeNode->left);
	int rightOpType=getOpType(treeNode->right);

	if (leftOpType==rightOpType)
		return leftOpType;

	//Will need to convert a value to a higher order value (i.e. int to float)
	printf("Mismatch between values\n");
	return 0;
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
