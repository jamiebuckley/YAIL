#include <stdio.h>
#include <stdlib.h>
#include "Tree.h"

/*
int main(int argc, char** argv)
{
	TreeNode* tree = newNode();
	tree->type="ROOT";
	tree->value="Statement";

	TreeNode* firstChild = newNode();
	firstChild->type="OPERAND";
	firstChild->value="This";

	TreeNode* secondChild = newNode();
	secondChild->type="OPERAND";
	secondChild->value="Should";

	TreeNode* thirdChild = newNode();
	thirdChild->type="OPERAND";
	thirdChild->value="Work";

	tree->isLeaf=0;	
	append(tree->children, firstChild);	
	append(tree->children, secondChild);
	append(tree->children, thirdChild);

	printxml(tree);

}
*/

TreeNode* newNode()
{
	TreeNode* node = malloc(sizeof(TreeNode));
	node->isLeaf = 1;
	node->type="NULL";
	node->value="EMPTY";
	node->children=newlist();
	return node;
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

	if(node->isLeaf)
		return 0;

	struct node* current;
    current	= node->children->start;

	while(current!=NULL)
	{
		print(current->data);
		current=current->next;
	}
}

int printxml(TreeNode* node)
{
	if(node==NULL)
		return-1;

	printf("<%s value=%s>\n", node->type, node->value);
	
	struct node* current;
	current = node->children->start;

	while(current!=NULL)
	{
		printxml(current->data);
		current=current->next;
	}

	printf("</%s>\n", node->type);
}
