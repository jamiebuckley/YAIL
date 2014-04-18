/*
 * Header for for Lex.c
 * Created 14 April 2014
 * Jamie Buckley
*/

int loadFile(char* filename);
int lex();
void addLexeme(int nodeType, char* srcdata, int ndata);

typedef struct s_lexeme
{
    int type;
    char* value;
} lexeme;
