#ifndef ILCGEN_HEADER
#define ILCGEN_HEADER

#include "Parse.h"
#include "BinaryTree.h"
#include "List.h"

int printILC(dlinklist* AST);
int outputNode(BinaryTreeNode* thisNode);
int outputLoadVar(char* varName);
int outputLoadCon(char* val);
int outputStoreVar(int inputNum, char* varName);
int operand(char* type, int R1, int R2);
int jump(char* type, int number);

#endif
