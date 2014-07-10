/* Converts the Abstract Syntax tree into a linear IR
 * Produces from an AST a linked list of LIRNodes
 * Each LIR node contains the operation type, and two operands
 * An operand can either be a memory address, a temporary variable or NULL
*/


/*
 * Needs to be modified to remove the pattern
 * LOAD R0 5
 * Store MEM0 R0
 * Load R1 MEM0
 *
 * This could be avoided by a symbol keeping track of where it is.
 * The store is only needed 
 *
*/

#include "ILCLinear.h"
#include "Compile.h"

#include <stdio.h>
#include <stdlib.h>

extern int verbose;
HashMap* symbolTable;

dlinklist* IRList;

int temp = 0;
int linenum = 0;



char* opWords[] = {"MEM", "TEMP", "CONSTANT", "LINE"};
char* commandWords[] = {"LOAD", "STORE", "ADD", "SUB", "MUL", "DIV", "COMPEQ", "COMPGT", "JUMP", "JUMPIF", "JUMPNIF", "END"};

dlinklist* createLinearIR(HashMap* symTable, dlinklist* AST)
{
	printf("Creating linear IR \n");
	symbolTable=symTable;

	IRList = newlist();
	node* current = AST->start;

	while(current != NULL)
	{
		IRCurrentNode(current->data);
		current = current->next;
	}

	LIRNode* endNode=newLIRNode(END, NULL, NULL);
	endNode->linenum=nextLine();
	append(IRList, endNode);

	if(verbose)
	{
		node* currentIR = IRList->start;
		while(currentIR != NULL)
		{
			LIRNode* x = currentIR->data;
			printf("%d: ", x->linenum);
			printf("%s ", commandWords[x->type]);
			if(x->operand1 != NULL) printf("%s:%d ", opWords[x->operand1->type], x->operand1->value);
			if(x->operand2 != NULL) printf("%s:%d", opWords[x->operand2->type], x->operand2->value);
			printf("\n");
			currentIR = currentIR->next;
		}
		printf("\n");
	}
	return IRList;
}

LIRNode* IRCurrentNode(BinaryTreeNode* statementNode)
{
	BinaryTreeNode* left = statementNode->left;
	BinaryTreeNode* right = statementNode->right;

	ASTNode* currentNode = statementNode->data;
	switch(currentNode->type)
	{
		case EQ:
			return handleAssign(statementNode);
		case NUM:
			return handleNum(statementNode);
		case VAR:
			return handleVar(statementNode);
		case PLUS:
			return handleOperation(CADD, statementNode);
		case MINUS:
			return handleOperation(CSUB, statementNode);
		case TIMES:
			return handleOperation(CMUL, statementNode);
		case DIV:
			return handleOperation(CDIV, statementNode);
		case GT:
			return handleComp(1, statementNode);
		case LT:
			return handleComp(0, statementNode);
		case IF:
			return handleIf(statementNode);
		case WHI:
			return handleWhile(statementNode);	
		default:
			printf("Error\n");
			return NULL;
	}
}

/* Handles the storing of values to memory */
LIRNode* handleAssign(BinaryTreeNode* assignNode)
{
	ASTNode* leftNode = assignNode->left->data;
	char* varName = leftNode->value;
	
	SymTabEntry* varEntry = hashMap_get(symbolTable, varName)->data;
	int varAddress = varEntry->address;

	LIROperand* op1 = newLIROperand(MEMADD, varAddress);	
	LIRNode* rightNode = IRCurrentNode(assignNode->right);

	LIRNode* result = newLIRNode(CSTORE, op1, rightNode->operand1);
	result->linenum = nextLine();

	varEntry->regaddress=rightNode->operand1->value;
	varEntry->lastDefine = rightNode;

	append(IRList, result);
	return result;
}

/* Loads constant values into a register */
LIRNode* handleNum(BinaryTreeNode* thisNode)
{
	ASTNode* thisData = thisNode->data;

	int tempNum = nextTemp();
	int value = atoi(thisData->value);

	LIROperand* op1 = newLIROperand(TEMP, tempNum);
	LIROperand* op2 = newLIROperand(CONSTANT, value); 

	LIRNode* result = newLIRNode(CLOAD, op1, op2);
	result->linenum = nextLine();

	append(IRList, result);
	return result;
}

/* Handles references to a variable */
LIRNode* handleVar(BinaryTreeNode* thisNode)
{
	char* varName = ((ASTNode*)thisNode->data)->value;	
	HashMapEntry* hashEntry = hashMap_get(symbolTable, varName);

	if(hashEntry==NULL)
		printf("Error: %s has not been initialized\n", varName);

	SymTabEntry* varEntry = hashEntry->data;

	if(varEntry->regaddress >= 0)
	{
		return varEntry->lastDefine;
	}
	//If the variable is not in memory, load it from memory
	int varAddress = varEntry->address;
	int tempNum = nextTemp();

	LIROperand* op1 = newLIROperand(TEMP, tempNum);
	LIROperand* op2 = newLIROperand(MEMADD, varAddress); 

	LIRNode* result = newLIRNode(CLOAD, op1, op2);
	result->linenum = nextLine();
	append(IRList, result);

	//If the variable is already in memory, return the 
	return result;
}

/* Handles any arithmetic operation */
LIRNode* handleOperation(int optype, BinaryTreeNode* thisNode)
{
	LIRNode* leftNode = IRCurrentNode(thisNode->left);
	LIRNode* rightNode = IRCurrentNode(thisNode->right);

	LIRNode* result = newLIRNode(optype, leftNode->operand1, rightNode->operand1);
	result->linenum = nextLine();
	append(IRList, result);
	return result;
}

/* Handles comparisons */
LIRNode* handleComp(int greater, BinaryTreeNode* thisNode)
{
	LIRNode* leftNode = IRCurrentNode(thisNode->left);
	LIRNode* rightNode = IRCurrentNode(thisNode->right);

	LIRNode* result;

	if(greater)
		result = newLIRNode(CCOMPGT, leftNode->operand1, rightNode->operand1); 
	else
		result = newLIRNode(CCOMPGT, rightNode->operand1, leftNode->operand1);

	result->linenum = nextLine();
	append(IRList, result);
	return result;
}

/* Handles if statements, and the contained statements */
LIRNode* handleIf(BinaryTreeNode* thisNode)
{
	LIRNode* comparison = IRCurrentNode(thisNode->left);
	LIRNode* jumpIf=newLIRNode(CJUMPIF, NULL, NULL);
	append(IRList, jumpIf);

	dlinklist* statements = thisNode->right->data;
	node* current = statements->start;	

	LIRNode* lastNode;
	while(current != NULL)
	{
		lastNode = IRCurrentNode(current->data);
		current = current->next;
	}

	LIROperand* op1 = newLIROperand(LINE, lastNode->linenum+1);

	jumpIf->operand1=op1;

	return lastNode;
}

/* Handles while statements, and the contained statements */
LIRNode* handleWhile(BinaryTreeNode* thisNode)
{
	LIRNode* comparison = IRCurrentNode(thisNode->left);
		
	LIRNode* jumpIf=newLIRNode(CJUMPIF, NULL, NULL);
	jumpIf->linenum=nextLine();
	append(IRList, jumpIf);

	dlinklist* statements = thisNode->right->data;
	node* current = statements->start;	

	LIRNode* lastNode;
	while(current != NULL)
	{
		lastNode = IRCurrentNode(current->data);
		current = current->next;
	}

	LIROperand* op1 = newLIROperand(LINE, lastNode->linenum+2);
	jumpIf->operand1=op1;

	LIROperand* jumpOp1 = newLIROperand(LINE, comparison->linenum);
	LIRNode* jump = newLIRNode(CJUMP, jumpOp1, NULL);
	jump->linenum=nextLine();
	append(IRList, jump);
	return jump;
}

/* Gives the next temporary value available */
int nextTemp()
{
	int result = temp;
	temp++;
	return result;
}

/* Produces the next line number */
int nextLine()
{
	int result = linenum;
	linenum++;
	return result;
}

/* Convenience constructors */
LIROperand* newLIROperand(int type, int value)
{
	LIROperand* result = malloc(sizeof(LIROperand));
	result->type=type;
	result->value=value;
	return result;
}

LIRNode* newLIRNode(int type, LIROperand* operand1, LIROperand* operand2)
{
	LIRNode* result = malloc(sizeof(LIRNode));
	result->type=type;
	result->operand1=operand1;
	result->operand2=operand2;
	return result;
}
