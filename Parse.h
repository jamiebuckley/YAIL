/*
 * Header for Parse.h
 * Created 18 April 2014
 * Jamie Buckley
*/

#ifndef PARSEFILE
#define PARSEFILE

#include "Lex.h"

#include "List.h"
#include "BinaryTree.h"

#define ERROR_MISMATCHED_PAREN 500
#define ERROR_INVALID_STATEMENT 501
#define ERROR_MISSING_SEMI 502
#define ERROR_MISSING_PARENTHESES 503

typedef struct ASTNode
{
	int type;
	char* textType;
	char* value;
	int hierarchyType;
} ASTNode;

dlinklist* parse(char* filename);

void printXML(BinaryTreeNode* thisNode, int index);

dlinklist* statements();
BinaryTreeNode* statement();
BinaryTreeNode* while_statement();
BinaryTreeNode* if_statement();
BinaryTreeNode* basic_statement();
BinaryTreeNode* varStatement();
BinaryTreeNode* expression();
BinaryTreeNode* expressionalpha();
BinaryTreeNode* term();
BinaryTreeNode* termalpha();
BinaryTreeNode* factor();
int lookahead();
int match(int value);
void advance();
int legallookahead(int, ...);
char* getLex();
void exitWithError(char* message, int error);
ASTNode* newASTNode(int type, char* textType, char* value);
void freeASTNode(void* this);
#endif
