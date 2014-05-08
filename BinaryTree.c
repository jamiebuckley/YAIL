/*
 * Binary Tree Implementation For YAIL
 * Created on 8 May 2014
 * Jamie Buckley
*/

#include <stdio.h>
#include <stdlib.h>
#include "BinaryTree.h"

/*
 * Testing Functions
void testPrintFunction(BinaryTreeNode* thisNode)
{
	if(thisNode->isLeaf)
	{
		printf("%s\n", (char*)thisNode->data);
	}
}

int main(int argc, char** argv)
{
	BinaryTreeNode* tree = newBinaryTreeNode();
	BinaryTreeNode* leftChild = newBinaryTreeNode();
	leftChild->data = "Left";

	BinaryTreeNode* rightChild = newBinaryTreeNode();
	rightChild->data = "Right";

	addBTNode(tree, leftChild, 1);
	addBTNode(tree, rightChild, 0);

	printTree(tree, &testPrintFunction);
}
*/


BinaryTreeNode* newBinaryTreeNode()
{
	BinaryTreeNode* newBTNode = malloc(sizeof(BinaryTreeNode));
	newBTNode->isLeaf=1;
	newBTNode->left=NULL;
	newBTNode->right=NULL;
	return newBTNode;
}

void addBTNode(BinaryTreeNode* parent, BinaryTreeNode* child, int left)
{
	if(left)
		parent->left = child;
	else
		parent->right = child;

	parent->isLeaf = 0;
}
