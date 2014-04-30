#ifndef TREE_HEADER
#define TREE_HEADER

#define DATA_TYPE struct TreeNode
#include "List.h"

#define OPERATOR 0
#define OPERAND  1 
#define ROOT 	 2

typedef struct TreeNode
{
	int isLeaf;
	char* type;
	char* value;
	struct TreeNode* parent;
	struct dlinklist* children;	
} TreeNode;

TreeNode* newNode();
int print(TreeNode*);
int printxml(TreeNode*);

#endif
