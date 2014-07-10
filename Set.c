/* Implementation of Sets, including unions, intersections, difference
 * Created on 30 June 2014
 * Jamie Buckley
*/

#include "Set.h"
#include <string.h>

dlinklist* set_union(dlinklist* setA, dlinklist* setB, comparator comp)
{
	dlinklist* result = newlist();

	node* current = setA->start;
	while(current != NULL)
	{
		append(result, current->data);
		current = current->next;
	}

	current = setB->start;
	while(current != NULL)
	{
		node* currentA = setA->start;
		while(currentA != NULL)
		{
			currentA = currentA->next;
		}
		append(result, current->data);
		current = current->next;
	}

	return result;
}

dlinklist* set_intersection(dlinklist* setA, dlinklist* setB, comparator comp)
{
	dlinklist* result = newlist();

	node* currentA = setA->start;
	while(currentA != NULL)
	{
		node* currentB = setB->start; 
		while(currentB != NULL)
		{	
			if(comp(currentA->data, currentB->data))
			{
				append(result, currentA->data);
				break;
			}
			currentB = currentB->next;
		}
		currentA = currentA->next;
	}
}

dlinklist* set_difference(dlinklist* setA, dlinklist* setB, comparator comp)
{
	dlinklist* result = newlist();

	node* currentA = setA->start;
	while(currentA != NULL)
	{
		node* currentB = setB->start; 
		int found=0;
		while(currentB != NULL)
		{	
			if(comp(currentA->data, currentB->data))
			{
				found = 1;
				break;
			}

			currentB = currentB->next;
		}

		if(!found)	
			append(result, currentA->data);

		currentA = currentA->next;
	}
}
