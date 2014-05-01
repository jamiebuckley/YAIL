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
	prepend(tree->children, thirdChild);

	printxml(0, tree);

}
*/

TreeNode* newNode()
{
	TreeNode* node = malloc(sizeof(TreeNode));
	node->isLeaf = 0;
	node->type="NULL";
	node->value=NULL;
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

int printxml(int tabnum, TreeNode* node)
{
	if(node==NULL)
	{
		printf("Error: Cannot print null node\n");
		return-1;
	}

	char* singleTag = (node->children->size == 0)? " /" : "";

	for(int i = 0; i < tabnum; i++) printf("  ");

	if(node->value==NULL)
		printf("<%s%s>\n", node->type, singleTag);
	else
		printf("<%s value=%s%s>\n", node->type, node->value, singleTag);

	if(node->children->size==0)return 0;

	struct node* current;
	current = node->children->start;

	while(current!=NULL)
	{
		printxml(tabnum+1, current->data);
		current=current->next;
	}

	for(int i = 0; i < tabnum; i++) printf("  ");
	printf("</%s>\n", node->type);
}
