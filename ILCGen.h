#ifndef ILCGEN_HEADER
#define ILCGEN_HEADER

#include "Parse.h"
#include "BinaryTree.h"
#include "List.h"

#define CLOAD   0
#define CSTORE  1
#define CADD    2
#define CSUB    3
#define CMUL    4
#define CDIV    5
#define CCOMPEQ 6
#define CCOMPGT 7
#define CJUMP   8
#define CJUMPIF 9
#define CJUMPNIF 10
#define END     11


#define LOCATIONOFVAR 404

typedef struct ILCLine
{
	int operation;
	int var1;
	int var2;
	int lnum;
} ILCLine;

int makeILC(dlinklist* AST, FILE* f);
ILCLine* outputNode(BinaryTreeNode* thisNode);
ILCLine* handleNum(BinaryTreeNode* thisNode);
ILCLine* handleVar(BinaryTreeNode* thisNode);
ILCLine* handleAssign(BinaryTreeNode* thisNode);
ILCLine* handleOp(BinaryTreeNode* thisNode, int opType);
ILCLine* handleIf(BinaryTreeNode* thisNode);
ILCLine* handleWhile(BinaryTreeNode* thisNode);
ILCLine* handleGreater(BinaryTreeNode* thisNode);
ILCLine* handleLess(BinaryTreeNode* thisNode);

int nextReg();
int nextLine();

#endif
