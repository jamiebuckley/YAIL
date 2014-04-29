#ifndef TREE_HEADER
#define TREE_HEADER

#include "List.h"

#define OPERATOR 0
#define OPERAND  1 
#define ROOT 	 2;

typedef struct TreeNode
{
	int isLeaf;
	int type;
	char* value;
	
	int numChildren;
	TreeNode** children;
} TreeNode;

TreeNode* newNode();
int add(TreeNode*, TreeNode*, int);
int print(TreeNode*);

#endif
