/* Initial Compiler FrontEnd header for YAIL
 * Created 10th May 2014
 * Jamie Buckley
*/

#ifndef COMPILE_HEADER
#define COMPILE_HEADER

#include "Parse.h"

#include "HashMap.h"

#include "ILCGen.h"

#define INTEGER 1
#define FLOAT	2
#define STRING  3

typedef struct SymTabEntry
{
	int type;
	char* name;	
	BinaryTreeNode* ASTRef;
} SymTabEntry;

int processAST(dlinklist* AST);
int makeSymTab(BinaryTreeNode* AST);
int getType(BinaryTreeNode* treeNode);
int getOpType(BinaryTreeNode* treeNode);
int printSymbolTable(HashMap* symbolTable);

#endif
