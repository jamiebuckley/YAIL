/*
 * Header for Parse.h
 * Created 18 April 2014
 * Jamie Buckley
*/

#ifndef PARSEFILE
#define PARSEFILE

#include "Lex.h"

#define DATA_TYPE lexeme

#include "List.h"



int statements();
int statement();
int while_statement();
int if_statement();
int basic_statement();
int leftarg();
int expression();
int expressionalpha();
int term();
int termalpha();
int factor();
int lookahead();
int match(int value);
void advance();
int legallookahead(int, ...);

#endif
