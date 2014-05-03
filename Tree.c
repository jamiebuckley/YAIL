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
	TreeNode* tnode = malloc(sizeof(TreeNode));
	tnode->isLeaf = 0;
	tnode->type="NULL";
	tnode->value=NULL;
	tnode->children=newlist();
	return tnode;
}

int print(TreeNode* tnode)
{
	if(tnode==NULL)
	{
		printf("Error: Cannot print null node\n");
		return -1;
	}	

	char* nodeType = (tnode->type==OPERATOR)? "Operator" : "Operand";

	printf("%s node, value: %s \n", nodeType, tnode->value);

	if(tnode->isLeaf)
		return 0;

	struct node* current;
    current	= tnode->children->start;

	while(current!=NULL)
	{
		print(current->data);
		current=current->next;
	}
}

int printxml(int tabnum, TreeNode* tnode)
{
	if(tnode==NULL)
	{
		printf("Error: Cannot print null node\n");
		return-1;
	}

	char* singleTag = (tnode->children->size == 0)? " /" : "";

	for(int i = 0; i < tabnum; i++) printf("  ");

	if(tnode->value==NULL)
		printf("<%s%s>\n", tnode->type, singleTag);
	else
		printf("<%s value=%s%s>\n", tnode->type, tnode->value, singleTag);

	if(tnode->children->size==0)return 0;

	struct node* current;
	current = tnode->children->start;

	while(current!=NULL)
	{
		printxml(tabnum+1, current->data);
		current=current->next;
	}

	for(int i = 0; i < tabnum; i++) printf("  ");
	printf("</%s>\n", tnode->type);
}
