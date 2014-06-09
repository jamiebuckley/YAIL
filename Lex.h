/*
 * Header for for Lex.c
 * Created 14 April 2014
 * Jamie Buckley
*/

#ifndef LEXFILE
#define LEXFILE

#define EOI   0
#define SEMI  1 
#define PLUS  2
#define MINUS 3
#define TIMES 4 
#define DIV   5
#define LP    6
#define RP    7
#define LCP   8
#define RCP   9
#define DO    10
#define WHI   11
#define IF    12
#define NUM   13
#define VAR   14
#define EQ    15

#define LT	  16
#define GT	  17

#define ERROR_NO_MATCH 404

#include "List.h"

dlinklist* lexfile(char* filename);
int loadFile(char* filename);
int lex();
void addLexeme(int nodeType, char* srcdata, int ndata);

typedef struct s_lexeme
{
    int type;
    char* value;
} lexeme;

#endif
