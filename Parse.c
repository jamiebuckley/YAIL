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
	printf("Start node is %s\n", listnode->thisnode->value);
	statements();
}

int statements()
{
	printf("Statements\n");
	statement();
	if(match(SEMI))
		advance();

	if (listnode->thisnode->type != EOI)
	{
		printf("More statements\n");
		statements();
	}
}

int statement()
{
	tabindex++;
	for(int i = 0; i < tabindex; i++) printf("\t");
	printf("Statement\n");
//	printf("Current is %d\n", listnode->thisnode->type);
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
	tabindex--;
}

int while_statement()
{
	for(int i = 0; i < tabindex; i++) printf("\t");
	printf("While Statement\n");
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
	for(int i = 0; i < tabindex; i++) printf("\t");
	printf("Basic statement\n");
	leftarg();
	expression();
}

int leftarg()
{
	for(int i = 0; i < tabindex; i++) printf("\t");
	printf("Leftarg\n");
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
	for(int i = 0; i < tabindex; i++) printf("\t");
	printf("Expression\n");
	term();
	expressionalpha();
}

int expressionalpha()
{	
	for(int i = 0; i < tabindex; i++) printf("\t");
	printf("Expression Alpha\n");
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
	for(int i = 0; i < tabindex; i++) printf("\t");
	printf("Term\n");
	factor();
	termalpha();
}

int termalpha()
{
	for(int i = 0; i < tabindex; i++) printf("\t");	
	printf("Term alpha\n");
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
	for(int i = 0; i < tabindex; i++) printf("\t");	
	printf("Factor\n");
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
	return nextnode->thisnode->type;	
}

int match(int value)
{
	if(listnode->thisnode->type == value)
	{
	for(int i = 0; i < tabindex; i++) printf("\t");	
		printf("Matched %s\n", listnode->thisnode->value);
		return 1;
	}
	return 0;
}

void advance()
{	
//	for(int i = 0; i < tabindex; i++) printf("\t");	
//	printf("Advance\n");
	listnode=nextnode;
	nextnode=listnode->next;
}

void exitWithError(char* message, int error)
{
	printf("%s\n", message);
	exit(error);
}
