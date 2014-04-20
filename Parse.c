/*
 * Recursive descent parser for YAIL.
 * Builds AST
 * Created 18 April 2014
 * Jamie Buckley
*/

#include <stdio.h>
#include <stdlib.h>

#include "Parse.h"

/* no header guards atm.  Fix this */


node* currentword;
extern dlinklist* list;

/* Test entry point.  Delete later */
int main(int argc, char** argv)
{
	lexfile(argv[1]);
	currentword=list->start;
	printf("Start node is %s\n", currentword->thisnode->value);
}

int statements()
{

}

int statement()
{

}

int while_statement()
{

}

int if_statement()
{

}

int basic_statement()
{

}

int leftarg()
{

}

int expression()
{

}

int term()
{

}

int factor()
{

}

lexeme* lookahead()
{

}

void advance()
{

}
