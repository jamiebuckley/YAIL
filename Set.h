/* Header file for Set.c
 * Created on 30 June 2014
 * Jamie Buckley
*/


#ifndef SETHEADER
#define SETHEADER

#include "List.h"
typedef int (*comparator)(void* a, void* b);

dlinklist* set_union(dlinklist* setA, dlinklist* setB, comparator comp);
dlinklist* set_intersection(dlinklist* setA, dlinklist* setB, comparator comp);
dlinklist* set_difference(dlinklist* setA, dlinklist* setB, comparator comp);

#endif
