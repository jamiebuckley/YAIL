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
extern dlinklist* list;

int tabindex = 0;

/* Test entry point.  Delete later */
int main(int argc, char** argv)
{
	lexfile(argv[1]);
	listnode=list->start;
	nextnode=listnode->next;
	TreeNode* statementsNode=statements();
	printxml(0, statementsNode);
}

TreeNode* statements()
{
	TreeNode* statementsNode = newNode();
	statementsNode->type="Statements";

	append(statementsNode->children, statement());

	while (!match(EOI))
	{
		append(statementsNode->children, statement());
	}
	return statementsNode;
}

TreeNode* statement()
{
	TreeNode* statementNode = newNode();
	statementNode->type="Statement";

	if(match(WHI))
	{
		advance();
		while_statement();
	}
	if(match(IF))
	{
		advance();
		if_statement();
	}
	else
	{
		append(statementNode->children,	basic_statement());
	}

	if(match(SEMI))
		advance();

	return statementNode;
}

TreeNode* while_statement()
{
	return NULL;
	if(match(LP))
	{
		advance();
		expression();
		if(match(RP))
			advance();
		if(match(LCP))
			advance();
		
		while(!match(RCP))
		{
			statement();
		}

		if (match(RCP))
			advance();
	}
}

TreeNode* if_statement()
{
	return NULL;
}

TreeNode* basic_statement()
{
	TreeNode* leftArg =	leftarg();
	append(leftArg->children, expression());
	return leftArg;
}

TreeNode* leftarg()
{

	TreeNode* leftArg=newNode();
	leftArg->type="Assign";	
	if(match(VAR))
	{
		leftArg->value=getLex();
		advance();
		if(match(EQ))
		{
			advance();
		}
	}
	return leftArg;
}

TreeNode* expression()
{
	TreeNode* termNode = term();
	TreeNode* expAl = expressionalpha();
	if(expAl != NULL)
	{
		prepend(expAl->children, termNode);
		return expAl;
	}
	else
	{
		return termNode;	
	}
}

TreeNode* expressionalpha()
{	
	TreeNode* expAl=newNode();
	expAl->type="Expression";
	if(match(GT))
	{
		expAl->value=getLex();
		advance();
	}
	else if(match(LT))
	{
		expAl->value=getLex();
		advance();
	}
	else if (match(PLUS))
	{
		expAl->value=getLex();
		advance();
	}
	else if (match(MINUS))
	{
		expAl->value=getLex();
		advance();
	}
	else
	{
		free(expAl);
		return NULL;
	}
	append(expAl->children, term());
	if(legallookahead(4, GT, LT, PLUS, MINUS))
		append(expAl->children, expressionalpha());

	return expAl;
}

TreeNode* term()
{
	TreeNode* fac = factor();
	TreeNode* term = termalpha();
	prepend(term->children, fac);
	return term;
}

TreeNode* termalpha()
{
	TreeNode* termAl = newNode();
	termAl->type="Term";
	if (match(TIMES))
	{
		termAl->value=getLex();
		advance();
	}	
	else if (match(DIV))
	{
		termAl->value=getLex();
		advance();
	}
	TreeNode* factorNode = factor();
	if(factorNode!=NULL)
	{
		append(termAl->children, factorNode);
	}

	if(legallookahead(2, TIMES, DIV))
		append(termAl->children, termalpha());

	return termAl;
}

TreeNode* factor()
{
	TreeNode* factorNode = newNode();
	factorNode->type="Factor";
	factorNode->isLeaf=1;
	if(match(NUM))
	{
		factorNode->type = "Number";
		factorNode->value = getLex(); 
		advance();
	}
	else if (match(VAR))
	{
		factorNode->type = "Variable";
		factorNode->value = getLex();
		advance();
	}
	else
	{
		return NULL;
	}
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
