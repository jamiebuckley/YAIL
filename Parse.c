/*
 * Recursive descent parser for YAIL.
 * Builds AST
 * Created 18 April 2014
 * Jamie Buckley
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "Parse.h"

/* no header guards atm.  Fix this */


node* listnode;
node* nextnode;

int tabindex = 0;


void printXML(BinaryTreeNode* thisNode, int index)
{
	if(!thisNode)
	{
		printf("thisNode = NULL\n");
		return;
	//	exitWithError("Cannot print null node", -1);
	}
	if(!thisNode->data)
	{
		printf("thisNode->data = NULL\n");
		return;
	//	exitWithError("Node has no data", -2);
	}
	ASTNode* thisData=thisNode->data;
	if(thisData->type==WHI || thisData->type==IF)
	{
		for(int i = 0; i < index; i++) printf("\t");
		printf("<%s>\n", thisData->textType);	
		printXML(thisNode->left, index+1);

		dlinklist* stmtList = thisNode->right->data;
		node* n = stmtList->start;

		while(n != NULL)
		{
			printXML((BinaryTreeNode*)n->data, index+1);
			n=n->next;
		}
		
		for(int i = 0; i < index; i++) printf("\t");	
		printf("</%s>\n", thisData->textType);
		return;
	}	

	if(!thisNode->isLeaf)
	{
		for(int i = 0; i < index; i++) printf("\t");
		printf("<%s>\n", thisData->textType);
		printXML(thisNode->left, index+1);
		printXML(thisNode->right, index+1);
	}
	else
	{
		for(int i = 0; i < index; i++) printf("\t");
		printf("<%s value=%s />\n", thisData->textType, thisData->value);
		return;
	}
	
	for(int i = 0; i < index; i++) printf("\t");
	printf("</%s>\n", thisData->textType);
}

/* Takes a filename and an uninitialized dlinklist pointer, sets pointer to AST for file, returns 0 on succes */
dlinklist* parse(char* filename)
{
    dlinklist* list=lexfile(filename);
	listnode=list->start;
	nextnode=listnode->next;

	dlinklist* AST=statements();
	return AST;
}

/*
 *
 Test entry point.  Delete later
int main(int argc, char** argv)
{
	lexfile(argv[1]);
	listnode=list->start;
	nextnode=listnode->next;
	dlinklist* statementsNode=statements();

	node* current = statementsNode->start;	
	while(current!=NULL)
	{
		printXML((BinaryTreeNode*)current->data, 0);
		current=current->next;
	}
}
*/

dlinklist* statements()
{
	dlinklist* statementsList = newlist();

	while (!match(EOI))
	{
		append(statementsList, statement());
	}
	return statementsList;
}

BinaryTreeNode* statement()
{
	BinaryTreeNode* statementNode;

	if(match(WHI))
	{
		advance();
		return while_statement();
	}
	else if(match(IF))
	{
		advance();
		return if_statement();
	}
	else if(match(VAR))
	{
		statementNode = basic_statement();

		if(match(SEMI))
			advance();
		else
			exitWithError("Expected semicolon after statement", ERROR_MISSING_SEMI);

		return statementNode;
	}
	else
	{
		exitWithError("Invalid statement beginning", ERROR_INVALID_STATEMENT);
	}
}

BinaryTreeNode* while_statement()
{
	if(!match(LP))
		exitWithError("Expected open parentheses", ERROR_MISSING_PARENTHESES);

	advance();

	BinaryTreeNode* conditionExpression = expression();

	if (!match(RP))
		exitWithError("Expected close parentheses", ERROR_MISSING_PARENTHESES);

	advance();

	if(!match(LCP))
		exitWithError("Expected open curly parentheses", ERROR_MISSING_PARENTHESES);

	advance();
	
	dlinklist* statementList = newlist();
	while(!match(RCP))
	{
		append(statementList, statement());
		if(match(EOF))
			exitWithError("Reached end of input in while loop. Check for missing parentheses", ERROR_MISSING_PARENTHESES);
	}
	advance();

	/* Create a Binary Tree that stores the Conditional Expression on the left, and the Statement list on the right */

	BinaryTreeNode* while_statementNode=newBinaryTreeNode();
	ASTNode* whileStatementData=newASTNode(WHI, "While", NULL);
	while_statementNode->data=whileStatementData;

	BinaryTreeNode* statementsNode = newBinaryTreeNode();
	statementsNode->data = statementList;

	addBTNode(while_statementNode, conditionExpression, LEFT);
	addBTNode(while_statementNode, statementsNode, RIGHT);

	return while_statementNode;

}

BinaryTreeNode* if_statement()
{
	if(!match(LP))
		exitWithError("Expected open parentheses", ERROR_MISSING_PARENTHESES);

	advance();

	BinaryTreeNode* conditionExpression = expression();

	if (!match(RP))
		exitWithError("Expected close parentheses", ERROR_MISSING_PARENTHESES);

	advance();

	if(!match(LCP))
		exitWithError("Expected open curly parentheses", ERROR_MISSING_PARENTHESES);

	advance();
	
	dlinklist* statementList = newlist();
	while(!match(RCP))
	{
		append(statementList, statement());
		if(match(EOF))
			exitWithError("Reached end of input in while loop. Check for missing parentheses", ERROR_MISSING_PARENTHESES);
	}
	advance();

	/* Create a Binary Tree that stores the Conditional Expression on the left, and the Statement list on the right */

	BinaryTreeNode* if_statementNode=newBinaryTreeNode();
	ASTNode* ifStatementData=newASTNode(IF, "If", NULL);
	if_statementNode->data=ifStatementData;

	BinaryTreeNode* statementsNode = newBinaryTreeNode();
	statementsNode->data = statementList;

	addBTNode(if_statementNode, conditionExpression, LEFT);
	addBTNode(if_statementNode, statementsNode, RIGHT);

	return if_statementNode;
}

/* 
 * Returns basic statement node of the form ASSIGN LEFTNODE RIGHTNODE 
 */
BinaryTreeNode* basic_statement()
{
	BinaryTreeNode* basicStatementNode = newBinaryTreeNode();
	BinaryTreeNode* varStatNode = varStatement();
	BinaryTreeNode* expNode = expression();

	addBTNode(basicStatementNode, varStatNode, LEFT);
	addBTNode(basicStatementNode, expNode, RIGHT);	
	
	ASTNode* basicStatementData=newASTNode(EQ, "Assign", NULL);
	basicStatementNode->data=basicStatementData;

	return basicStatementNode;
}

/* 
 * Returns node with variable name
 */
BinaryTreeNode* varStatement()
{
	BinaryTreeNode* varNode=newBinaryTreeNode();
	char* varName;

	if(match(VAR))
	{
		varName=getLex();
		advance();	
	}
	else exitWithError("Invalid statement beginning", ERROR_INVALID_STATEMENT); 

	if(match(EQ))
		advance();
	else exitWithError("Expected '=' after variable", ERROR_INVALID_STATEMENT);

	ASTNode* varNodeData=newASTNode(VAR, "Variable", varName);
	varNode->data=varNodeData;

	return varNode;
}

BinaryTreeNode* expression()
{
	BinaryTreeNode* termNode = term();
	BinaryTreeNode* expAl = expressionalpha();

	if(expAl == NULL)
		return termNode;	

	addBTNode(expAl, termNode, LEFT);
	return expAl;
}

BinaryTreeNode* expressionalpha()
{	
	ASTNode* expAlData;

	if(match(GT))
	{
		expAlData=newASTNode(GT, "GreaterThan", NULL);
		advance();
	}
	else if(match(LT))
	{
		expAlData=newASTNode(LT, "LessThan", NULL);
		advance();
	}
	else if (match(PLUS))
	{
		expAlData=newASTNode(PLUS, "Plus", NULL);
		advance();
	}
	else if (match(MINUS))
	{
		expAlData=newASTNode(MINUS, "Minus", NULL);
		advance();
	}
	else
	{
		return NULL;
	}
	BinaryTreeNode* expAl=newBinaryTreeNode();
	expAl->data=expAlData;

	BinaryTreeNode* thisTerm=term();
	
	/* if there is a +->< after first term, get repeated terms and put them on the left */
	if(legallookahead(4, GT, LT, PLUS, MINUS))
	{
		BinaryTreeNode* nextExp=expressionalpha();
		addBTNode(nextExp, thisTerm, LEFT);
		addBTNode(expAl, nextExp, RIGHT);
		return expAl;
	}
	else
	{
		addBTNode(expAl, thisTerm, RIGHT);
		return expAl;
	}
}

BinaryTreeNode* term()
{
	BinaryTreeNode* fac = factor();
	BinaryTreeNode* term = termalpha();
	
	if(term==NULL)
		return fac;

	addBTNode(term, fac, LEFT);
	return term;
}

BinaryTreeNode* termalpha()
{
	ASTNode* termAlData;

	if (match(TIMES))
	{
		termAlData=newASTNode(TIMES, "Multiply", NULL);
		advance();
	}	
	else if (match(DIV))
	{
		termAlData=newASTNode(DIV, "Divide", NULL);
		advance();
	}
	else
	{
		return NULL;
	}

	BinaryTreeNode* termAl = newBinaryTreeNode();
	termAl->data=termAlData;
	BinaryTreeNode* fac = factor();

	if(legallookahead(2, TIMES, DIV))
	{
		BinaryTreeNode* nextTerm = termalpha();
		addBTNode(nextTerm, fac, LEFT);
		addBTNode(termAl, nextTerm, RIGHT);
		return termAl;
	}
	else
	{
		addBTNode(termAl, fac, RIGHT);
		return termAl;
	}
}

BinaryTreeNode* factor()
{
	BinaryTreeNode* factorNode = newBinaryTreeNode();
	ASTNode* factorData;

	if(match(NUM))
	{
		factorData=newASTNode(NUM, "Number", getLex()); 
		advance();
	}
	else if (match(VAR))
	{
		factorData=newASTNode(VAR, "Variable", getLex());
		advance();
	}
	else
		exitWithError("Expected variable or constant value", ERROR_INVALID_STATEMENT);

	factorNode->data=factorData;
	return factorNode;
}

int legallookahead(int n, ...)
{
	va_list args;
	va_start(args, n);
	for(int i = 0; i <n;i++)
	{
		int token = va_arg(args, int);
		if(match(token))
			return 1;
	}
	va_end(args);

	return 0;
}

int lookahead()
{
	lexeme* lxm = (lexeme*)(nextnode->data);
	return lxm->type;	
}

int match(int value)
{
	lexeme* lxm = (lexeme*)(listnode->data);
	if(lxm->type == value)
	{
		return 1;
	}
	return 0;
}

char* getLex()
{
	lexeme* lxm = (lexeme*)(listnode->data);
	return lxm->value;
}

void advance()
{	
	listnode=nextnode;
	nextnode=listnode->next;
}

void exitWithError(char* message, int error)
{
	printf("%s\n", message);
	exit(error);
}

ASTNode* newASTNode(int type, char* textType, char* value)
{
	ASTNode* newNode = malloc(sizeof(ASTNode));
	newNode->type=type;
	newNode->textType=textType;
	newNode->value=value;
	newNode->hierarchyType=0;
	return newNode;
}
