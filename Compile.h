/* Initial Compiler FrontEnd header for YAIL
 * Created 10th May 2014
 * Jamie Buckley
*/

#ifndef COMPILE_HEADER
#define COMPILE_HEADER

#include "Parse.h"

#include "HashMap.h"

#include "ILCLinear.h"

#define INTEGER 1
#define FLOAT	2
#define STRING  3

typedef struct SymTabEntry
{
	int type;
	char* name;	
	BinaryTreeNode* ASTRef;
	int address;
	int regaddress;
	LIRNode* lastDefine;
} SymTabEntry;

int processAST(dlinklist* AST);
int makeSymTab(BinaryTreeNode* AST);
int getType(BinaryTreeNode* treeNode);
int getOpType(BinaryTreeNode* treeNode);
int printAST(dlinklist* AST);
int printSymbolTable(HashMap* symbolTable);
char* getNameWithoutExtension(char* filename);
#endif
