/* 
 * Linked list test implementation for YAIL
 * Created on 16 April 2014
 * Jamie Buckley
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main(int argc, char** argv)
{
    dlinklist* list = newlist();

    int* num = malloc(sizeof(int));
    int* num2 = malloc(sizeof(int));

    *num = 5;
    *num2 = 10;

    append(list, num);
    append(list, num2);

    node* current = list->start;
    while(current!=NULL)
    {
	printf("%d\n", *(current->thisnode));
	current=current->next;
    }
}
