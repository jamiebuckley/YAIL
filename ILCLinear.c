/* Converts the Abstract Syntax tree into a linear IR
 * Produces from an AST a linked list of LIRNodes
 * Each LIR node contains the operation type, and two operands
 * An operand can either be a memory address, a temporary variable or NULL
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

	/* Process the root node of each AST statement */
	while(current != NULL)
	{
		ProcessCurrentNode(current->data);
		current = current->next;
	}

	/* Append the END node to the list */
	LIRNode* endNode=newLIRNode(END, NULL, NULL);
	endNode->linenum=nextLine();
	append(IRList, endNode);

	/* If verbose, print the list */
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

LIROperand* ProcessCurrentNode(BinaryTreeNode* statementNode)
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
/* CSTORE MEM REGISTER/CONSTANT */
LIROperand* handleAssign(BinaryTreeNode* assignNode)
{
	/* Get variable info */
	ASTNode* leftNode = assignNode->left->data;
	SymTabEntry* varEntry = hashMap_get(symbolTable, leftNode->value)->data;
	int varAddress = varEntry->address;

	LIROperand* leftOperand = newLIROperand(MEMADD, varAddress);	
	LIROperand* rightOperand = ProcessCurrentNode(assignNode->right);

	LIRNode* result = newLIRNode(CSTORE, leftOperand, rightOperand);
	result->linenum = nextLine();

	varEntry->regaddress=rightOperand->value;

	append(IRList, result);
	return leftOperand;
}

/* Returns a constant value */
LIROperand* handleNum(BinaryTreeNode* thisNode)
{
	ASTNode* thisData = thisNode->data;
	int value = atoi(thisData->value);

	LIROperand* op1 = newLIROperand(CONSTANT, value);
	return op1;
}

LIROperand* loadNum(BinaryTreeNode* thisNode)
{
	ASTNode* thisData = thisNode->data;

	int tempNum = nextTemp();
	int value = atoi(thisData->value);

	LIROperand* op1 = newLIROperand(TEMP, tempNum);
	LIROperand* op2 = newLIROperand(CONSTANT, value); 

	LIRNode* result = newLIRNode(CLOAD, op1, op2);
	result->linenum = nextLine();

	append(IRList, result);
	return op1;
}

/* Handles references to a variable */
LIROperand* handleVar(BinaryTreeNode* thisNode)
{
	char* varName = ((ASTNode*)thisNode->data)->value;	
	HashMapEntry* hashEntry = hashMap_get(symbolTable, varName);

	if(hashEntry==NULL)
		printf("Error: %s has not been initialized\n", varName);

	SymTabEntry* varEntry = hashEntry->data;
	int varAddress = varEntry->address;
	int tempNum = nextTemp();

	LIROperand* leftOperand = newLIROperand(TEMP, tempNum);
	LIROperand* rightOperand = newLIROperand(MEMADD, varAddress); 

	LIRNode* result = newLIRNode(CLOAD, leftOperand, rightOperand);
	result->linenum = nextLine();
	append(IRList, result);
	return leftOperand;
}

/* Handles any arithmetic operation */
LIROperand* handleOperation(int optype, BinaryTreeNode* thisNode)
{
	/* If one or both nodes are temporaries, do the following */
	ASTNode* leftData,* rightData;

	leftData = thisNode->left->data;
	rightData = thisNode->right->data;

	printf("OPS %d: %d %d\n", optype, leftData->type, rightData->type);
	int lNC = (leftData->type == NUM);
	int rNC = (rightData->type == NUM);

	printf("%d %d\n", lNC, rNC);
	LIROperand* leftOperand, *rightOperand;
	if(lNC)
		leftOperand = loadNum(thisNode->left);
	else
		leftOperand = ProcessCurrentNode(thisNode->left);

	rightOperand = ProcessCurrentNode(thisNode->right);
	/* if both are constants, load one of them into a temporary, and that is the left operand */

	LIRNode* result = newLIRNode(optype, leftOperand, rightOperand);
	result->linenum = nextLine();
	append(IRList, result);
	return leftOperand;
}

/* Handles comparisons */
LIROperand* handleComp(int greater, BinaryTreeNode* thisNode)
{
	LIROperand* leftOperand = ProcessCurrentNode(thisNode->left);
	LIROperand* rightOperand = ProcessCurrentNode(thisNode->right);

	LIRNode* result;
	if(greater)
		result = newLIRNode(CCOMPGT, leftOperand, rightOperand); 
	else
		result = newLIRNode(CCOMPGT, rightOperand, leftOperand);

	result->linenum = nextLine();
	append(IRList, result);
	return result->operand1;
}

/* Handles if statements, and the contained statements */
LIROperand* handleIf(BinaryTreeNode* thisNode)
{
	LIROperand* comparison = ProcessCurrentNode(thisNode->left);
	LIRNode* jumpIf=newLIRNode(CJUMPIF, NULL, NULL);
	jumpIf->linenum=nextLine();
	append(IRList, jumpIf);

	dlinklist* statements = thisNode->right->data;
	node* current = statements->start;	

	while(current != NULL)
	{
		ProcessCurrentNode(current->data);
		current = current->next;
	}

	LIROperand* lineOperand = newLIROperand(LINE, linenum);
	jumpIf->operand1=lineOperand;
	return lineOperand;
}

/* Handles while statements, and the contained statements */
LIROperand* handleWhile(BinaryTreeNode* thisNode)
{
	LIROperand* comparison = ProcessCurrentNode(thisNode->left);	
	LIRNode* jumpIf=newLIRNode(CJUMPIF, NULL, NULL);
	int startLine = jumpIf->linenum = nextLine();
	append(IRList, jumpIf);

	dlinklist* statements = thisNode->right->data;
	node* current = statements->start;	

	while(current != NULL)
	{
		ProcessCurrentNode(current->data);
		current = current->next;
	}

	LIROperand* lineOperand = newLIROperand(LINE, linenum+2);
	jumpIf->operand1=lineOperand;

	LIROperand* jumpOp1 = newLIROperand(LINE, startLine-1);
	LIRNode* jump = newLIRNode(CJUMP, jumpOp1, NULL);
	jump->linenum=nextLine();
	append(IRList, jump);
	return jumpOp1;
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
