/* Initial Compiler FrontEnd for YAIL
 * Created 10th May 2014
 * Jamie Buckley
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include "Compile.h"

int main(int argc, char** argv)
{
	
	/* perform basic checks on input filename */
	/* DO THIS LATER */
	/*  */

	dlinklist* AST;
	AST = parse(argv[1]);
	makeSymTab(AST);

	if(AST == NULL)
	{
		printf("Ees null\n");
	}

	node* current = AST->start;	
	while(current!=NULL)
	{
		printXML((BinaryTreeNode*)current->data, 0);
		current=current->next;
	}

}

int makeSymTab(dlinklist* AST)
{

}
