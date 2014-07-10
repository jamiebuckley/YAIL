/* Header file for ILCLinear.c
 * Created 28th June 2014
 * Jamie Buckley
*/

#ifndef ILCLINEARHEADER
#define ILCLINEARHEADER

#include "BinaryTree.h"
#include "Parse.h"
#include "List.h"
#include "HashMap.h"

#define MEMADD   0
#define TEMP     1
#define CONSTANT 2
#define LINE	 3
typedef struct LIROperand
{
	int type;
	int value;
} LIROperand;


#define CLOAD   0
#define CSTORE  1
#define CADD    2
#define CSUB    3
#define CMUL    4
#define CDIV    5
#define CCOMPEQ 6
#define CCOMPGT 7
#define CJUMP   8
#define CJUMPIF 9
#define CJUMPNIF 10
#define END     11

typedef struct LIRNode
{
	int linenum;
	int type;
	LIROperand* operand1;
	LIROperand* operand2;
} LIRNode;

dlinklist* createLinearIR(HashMap* symTable, dlinklist* AST);
LIRNode* IRCurrentNode(BinaryTreeNode* statementNode);

LIRNode* handleAssign(BinaryTreeNode* assignNode);
LIRNode* handleNum(BinaryTreeNode* thisNode);
LIRNode* handleVar(BinaryTreeNode* thisNode);
LIRNode* handleOperation(int optype, BinaryTreeNode* thisNode);
LIRNode* handleComp(int greater, BinaryTreeNode* thisNode);
LIRNode* handleIf(BinaryTreeNode* thisNode);
LIRNode* handleWhile(BinaryTreeNode* thisNode);
int nextTemp();
int nextLine();

/* Convenience Constructors */
LIROperand* newLIROperand(int type, int value);
LIRNode* newLIRNode(int type, LIROperand* operand1, LIROperand* operand2);
#endif
