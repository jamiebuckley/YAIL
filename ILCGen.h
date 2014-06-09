#ifndef ILCGEN_HEADER
#define ILCGEN_HEADER

#include "Parse.h"
#include "BinaryTree.h"
#include "List.h"

int makeILC(dlinklist* AST, FILE* f);
int outputNode(BinaryTreeNode* thisNode, FILE* f);
int outputLoadVar(char* varName, FILE* f);
int outputLoadCon(char* val, FILE* f);
int outputStoreVar(int inputNum, char* varName, FILE* f);
int operand(char* type, int R1, int R2, FILE* f);
int jump(char* type, int number, FILE* f);

#endif
