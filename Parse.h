/*
 * Header for Parse.h
 * Created 18 April 2014
 * Jamie Buckley
*/

#ifndef PARSEFILE
#define PARSEFILE

#include "Lex.h"

#include "List.h"
#include "Tree.h"


TreeNode* statements();
TreeNode* statement();
TreeNode* while_statement();
TreeNode* if_statement();
TreeNode* basic_statement();
TreeNode* leftarg();
TreeNode* expression();
TreeNode* expressionalpha();
TreeNode* term();
TreeNode* termalpha();
TreeNode* factor();
int lookahead();
int match(int value);
void advance();
int legallookahead(int, ...);
char* getLex();

#endif
