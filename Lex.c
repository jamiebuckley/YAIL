/*
 * Lexer program for YAIL.  Retrieves tokens from source file.
 * Created April 14 2014
 * Jamie Buckley
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LEXFILE
#define DATA_TYPE lexeme
#include "Lex.h"
#define LEXFILE
#endif

#include "list.h"

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

char* data;
char* current;


dlinklist* list;


/* Test main function*/ 
int lexfile(char* filename)
{
    if(loadFile(filename))
    {
	    printf("Error opening file\n");
	    return -1;
    }

	list = newlist();

    int num=0;
	int res = 0;
    do
    {
		res = lex();
    }
    while(!res);

	node* current = list->start;

	while(current != NULL)
	{
		printf("%d: %s = %d\n", num, current->thisnode->value, current->thisnode->type);
		current = current->next;
		num++;
	}
}

/* Load <filename> into the data buffer */
int loadFile(char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (!file) return 1;
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    data = malloc(fsize+1);
    fread(data, fsize, 1, file);
    fclose(file);

    data[fsize] = 0;
    current=data;
    return 0;
}

/* Retrieve the next token from the buffer */
int lex()
{
   while(1)
   {
	while(isspace(*current))
		current++;

	/* Single Character Matching */
    switch(*current)
	{
        case '\0': addLexeme(EOI, current, 1); current++; return 1;
        case ';': addLexeme(SEMI, current, 1); current++; return 0; 
		case '+': addLexeme(PLUS, current, 1); current++; return 0;
		case '-': addLexeme(MINUS, current, 1); current++; return 0; 
		case '*': addLexeme(TIMES, current, 1); current++; return 0;
		case '/': addLexeme(DIV, current, 1); current++; return 0;
		case '(': addLexeme(LP, current, 1); current++; return 0;
		case ')': addLexeme(RP, current, 1); current++; return 0;
		case '{': addLexeme(LCP, current, 1); current++; return 0;
		case '}': addLexeme(RCP, current, 1); current++; return 0;
	    case '=': addLexeme(EQ, current, 1); current++; return 0;
	    case '<': addLexeme(LT, current, 1); current++; return 0;
	    case '>': addLexeme(GT, current, 1); current++; return 0;
		case '\n':
		case '\t':
		case ' ' : current++; continue;
	}
	
	/* Multi char matching */
	if(strncmp(current, "do", 2)==0)
	{
		addLexeme(DO, current, 2);
		current+=2;
		return 0;
	}

	if(strncmp(current, "if", 2)==0)
	{
		addLexeme(IF, current, 2);
		current+=2;
		return 0; 
	}

	if(strncmp(current, "while", 5)==0)
	{
		addLexeme(WHI, current, 5);
		current+=5;
		return 0; 
	}

	int llen=0;
	/* Check for letter and not punctuation - VARIABLE */
	if(isalpha(*current) && !ispunct(*current))
	{
		current++; llen++;
		while(isalnum(*current) && !ispunct(*current))
		{
			current++; llen++;
		}
		addLexeme(VAR, current-llen, llen);
		return 0;
	}

	/* Check for number - CONSTANT */
	if(isdigit(*current))
	{
		while(isdigit(*current))
		{
			current++; llen++;
		}
		addLexeme(NUM, current-llen, llen);
		return 0;
   	} 

	return ERROR_NO_MATCH;
   }
}

/* adds a lexeme according to the input type*/
void addLexeme(int nodeType, char* srcdata, int ndata)
{
	lexeme* word = malloc(sizeof(lexeme));
	word->type=nodeType;
	word->value = malloc((ndata+1) * sizeof(char));
	strncpy(word->value, srcdata, ndata*sizeof(char)); 
	word->value[ndata]='\0';
	
	append(list, word);
}
