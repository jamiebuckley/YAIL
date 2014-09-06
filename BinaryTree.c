/*
 * Binary Tree Implementation For YAIL
 * Created on 8 May 2014
 * Jamie Buckley
*/

#include <stdio.h>
#include <stdlib.h>
#include "BinaryTree.h"

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

void deleteBTNode(BinaryTreeNode* this, deleteFunction func)
{
	if(this->left != NULL)
	{
		deleteBTNode(this->left, func);
		this->left = NULL;
	}
	if(this->right != NULL)
	{
		deleteBTNode(this->right, func);
		this->right = NULL;
	}

	func(this->data);
	free(this);
}
