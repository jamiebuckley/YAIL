/* HashMap header for YAIL
 * Created 8th May 2014
 * Jamie Buckley
*/

#ifndef HASHMAP_HEADER
#define HASHMAP_HEADER

#include "List.h"

typedef struct HashMapEntry
{
	char* value;
	void* data;	
} HashMapEntry;

typedef struct HashMap
{
	int numOfBuckets;
	dlinklist* buckets;
} HashMap;

HashMap* newHashMap(int size);
int hashMap_hash(char* data, int size);
int hashMap_put(HashMap*, char* value, void* data);
HashMapEntry* hashMap_get(HashMap*, char* data);
int hashMap_remove(HashMap*, char* data, nodeOp f);

#endif
