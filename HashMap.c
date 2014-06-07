/* HashMap for YAIL
 * Created 8th May 2014
 * Jamie Buckley
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashMap.h"

/*
int main(int argc, char** argv)
{
	char* arrayofstrings[] = {"Hello", "this", "is", "a", "simple", "hash", "map"};
	char* arrayofdata[] = {"Goodbye", "that", "isn't", "several", "complex", "perfect", "compass"};

	HashMap* map = newHashMap(128);

	for(int i = 0; i < 7; i++)
	{
		hashMap_put(map, arrayofstrings[i], arrayofdata[i]);	
	}
	char* val = (char*)hashMap_get(map, "Hello")->data;
	printf("retrieved %s from hashmap\n", val);

	int num = hashMap_remove(map, "Hello", NULL);
	void* data = hashMap_get(map, "Hello");
	if(data == NULL)
	{
		printf("Success!\n");
	}
}
*/

HashMap* newHashMap(int size)
{
	if(size==0 || (size & (size - 1)))
	{
		printf("Error, invalid size\n");
		return NULL;	
	}

	HashMap* result = malloc(sizeof(HashMap));

	result->numOfBuckets = size;
	result->buckets = calloc(size, sizeof(dlinklist));

	result->keys=newlist();

	return result;
}

int hashMap_hash(char* data, int size)
{
	int result = 0;
	char* current = data;
	while(current!= NULL && *current != 0)
	{
		result+=*current;
		result<<2;
		result = result & (size-1);
		current++;
	}
	return result;
}

int hashMap_put(HashMap* hashMap, char* value, void* data)
{
	int bucketIndex = hashMap_hash(value, hashMap->numOfBuckets);

	HashMapEntry* entry = malloc(sizeof(HashMapEntry));
	entry->value = value;
	entry->data = data;
	append(hashMap->buckets+bucketIndex, entry);

	append(hashMap->keys, value);
}

HashMapEntry* hashMap_get(HashMap* hashMap, char* data)
{
	int bucketIndex = hashMap_hash(data, hashMap->numOfBuckets);
	
	node* current = (hashMap->buckets+bucketIndex)->start;

	while(current != NULL)
	{
		HashMapEntry* thisEntry = (HashMapEntry*)(current->data);

		if(!strcmp(thisEntry->value, data))
		{
			return thisEntry;
		}
		else
		{
			current=current->next;
		}
	}
	return NULL;
}

int hashMapCompare(node* thisNode, void* data)
{
	HashMapEntry* entry = thisNode->data;
	return !strcmp(entry->value, (char*)data); 
}

int hashMap_remove(HashMap* hashMap, char* data, nodeOp f)
{
	int bucketIndex = hashMap_hash(data, hashMap->numOfBuckets);
	dlinklist* thislist = hashMap->buckets+bucketIndex;
	return list_remove_comparator(thislist, data, f, hashMapCompare, data);	
}
