/*
 * Binary Tree Header for YAIL
 * Created on 8 May 2014
 * Jamie Buckley
*/

#ifndef BINARY_TREE_HEADER
#define BINARY_TREE_HEADER

#define LEFT 1
#define RIGHT 0

typedef struct BinaryTreeNode
{
	int isLeaf;
	struct BinaryTreeNode* left;
	struct BinaryTreeNode* right;
	void* data;
} BinaryTreeNode;

typedef void (*printFunction)(BinaryTreeNode*, int level);

BinaryTreeNode* newBinaryTreeNode();
void addBTNode(BinaryTreeNode* parent, BinaryTreeNode* child, int left);

typedef void (*deleteFunction)(void* data);
void deleteBTNode(BinaryTreeNode* this, deleteFunction func);
#endif
