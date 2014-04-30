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
	statements();
}

int statements()
{
	statement();

	if (!match(EOI))
	{
		statement();
	}
}

int statement()
{
	tabindex++;
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
		basic_statement();
	}

	if(match(SEMI))
		advance();
	tabindex--;
}

int while_statement()
{
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

int if_statement()
{

}

int basic_statement()
{
	leftarg();
	expression();
}

int leftarg()
{
	if(match(VAR))
	{
		advance();
		if(match(EQ))
		{
			advance();
		}
	}
}

int expression()
{
	term();
	expressionalpha();
}

int expressionalpha()
{	
	if(match(GT))
	{
		advance();
	}
	else if(match(LT))
	{
		advance();
	}
	else if (match(PLUS))
	{
		advance();
	}
	else if (match(MINUS))
	{
		advance();
	}
	term();
	if(legallookahead(4, GT, LT, PLUS, MINUS))
		expressionalpha();
}

int term()
{
	factor();
	termalpha();
}

int termalpha()
{
	if (match(TIMES))
	{
		advance();
	}	
	else if (match(DIV))
	{
		advance();
	}
	factor();

	if(legallookahead(2, TIMES, DIV))
		termalpha();
}

int factor()
{
	if(match(NUM))
	{
		advance();
	}
	else if (match(VAR))
	{
		advance();
	}
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
