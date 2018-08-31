#if !defined(API_HASHTABLE_H)
#include "yoyoyo_ascii_vector.h"
#define MAX_ALLOWED_COLLISIONS 4

//NOTE(Ray):At the moment this hashtable only allows for fixed size tables.
//resizing not implemented yet.
struct YoyoHashKeyEntry
{
	uint64_t value_index;
	char* key;
	uint32_t collision_count;
};

struct YoyoHashValueEntry
{
	void* value;
	struct YoyoHashKeyEntry hash_collisions[MAX_ALLOWED_COLLISIONS];
};

struct YoyoHashTable
{
	YoyoVector keys;
	YoyoVector values;
};

static YoyoHashTable YoyoInitHashTable(uint32_t start_count)
{
	YoyoHashTable result;
	result.keys = YoyoInitVector(start_count,YoyoHashKeyEntry,false);
	result.keys.allow_resize = false;
	result.values = YoyoInitVector(start_count,YoyoHashValueEntry,false);
	result.values.allow_resize = false;
	return result;
}

// Hashing function from Program 14.2 in the Sedgewick book
//http://cecilsunkure.blogspot.com/2012/07/hash-tables.html
uint64_t UHashMod(const char *Key, unsigned TableSize)
{
	uint64_t hash = 0;      // Initial value of hash
	uint64_t rand1 = 31415; // "Random" 1
	uint64_t rand2 = 27183; // "Random" 2

	while (*Key)
	{
		hash = hash * rand1;     // Multiply hash by random
		hash = (hash + *Key);    // Add in current char, keep within TableSize
		rand1 = (rand1 * rand2); // Update rand1 for next "random" number
		Key++;
	}

	return hash % TableSize;
}

static uint64_t YoyoHashFunction(YoyoHashTable* h_table,const char* in)
{
	uint64_t result;
	result = UHashMod(in, h_table->keys.total_count);
	return result;
}

static void YoyoAddElementToHashTable(YoyoHashTable* h_table,char* key,void* element)
{
	uint64_t hash_index = YoyoHashFunction(h_table,key);
	YoyoHashKeyEntry e;
	e.collision_count = 0;
	e.key = key;
	e.value_index = hash_index;
	YoyoHashValueEntry v;
	v.value = element;

	YoyoHashKeyEntry* lu = YoyoGetVectorElement(YoyoHashKeyEntry, &h_table->keys, hash_index);
	if(lu->key != nullptr)
	{
		//TODO(Ray):Handle collisions.
		Assert(false);
	}
	else
	{
		//TODO(Ray):Compare if this same keyelement is already added.
		//if yes return cant add because key already exists
		//if no add element
		YoyoSetVectorElement(&h_table->keys, hash_index, &e);
		YoyoSetVectorElement(&h_table->values, hash_index, &v);
	}
}

#define YoyoGetElementByHash(Type,table,in) (Type*)YoyoGetElementByHash_(table,in)
static void* YoyoGetElementByHash_(YoyoHashTable* h_table,char* in)
{
	void* result;
	uint64_t hash_index = YoyoHashFunction(h_table,in);
	YoyoHashKeyEntry* e = YoyoGetVectorElement(YoyoHashKeyEntry,&h_table->keys,hash_index);
	if(e->collision_count == 0)
	{
		YoyoHashValueEntry* ve = YoyoGetVectorElement(YoyoHashValueEntry,&h_table->values, hash_index);
		result = ve->value;
	}
	else
	{
		//Do string comparisons on key values till we get the proper entry.
	}
	return result;
}

#define API_HASHTABLE
#endif
