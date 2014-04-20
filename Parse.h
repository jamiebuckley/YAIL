/*
 * Header for Parse.h
 * Created 18 April 2014
 * Jamie Buckley
*/

#ifndef PARSEFILE
#define PARSEFILE

#include "Lex.h"

#define DATA_TYPE lexeme

#include "list.h"



int statements();
int statement();
int while_statement();
int if_statement();
int basic_statment();
int leftarg();
int expression();
int term();
int factor();
lexeme* lookahead();
void advance();

#endif
