#include <stdio.h>
#include <stdlib.h>
#include "Tree.h"

int main(int argc, char** argv)
{
	TreeNode* tree = newNode();
	tree->type=ROOT;
	tree->value="ROOT";

	TreeNode* left = newNode();
	left->type=OPERAND;
	left->value="x";

	TreeNode* right = newNode();
	right->type=OPERAND;
	right->value="10";

	add(tree, left, 0);
	add(tree, right, 1);

	print(tree);
}

TreeNode* newNode()
{
	TreeNode* node = malloc(sizeof(TreeNode));
	node->isLeaf = 1;
	node->type=-1;
	node->value="EMPTY";
}

int add(TreeNode* thisNode, TreeNode* newNode, int left)
{
	thisNode->isLeaf=0;

	if(left == 0)
	{
		thisNode->left = newNode;
	}
	else
	{
		thisNode->right = newNode;
	}
}

int print(TreeNode* node)
{
	if(node==NULL)
	{
		printf("Error: Cannot print null node\n");
		return -1;
	}	

	char* nodeType = (node->type==OPERATOR)? "Operator" : "Operand";

	printf("%s node, value: %s \n", nodeType, node->value);

	if(!node->isLeaf)
	{
		print(node->left);
		print(node->right);
	}
}

