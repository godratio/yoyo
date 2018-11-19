#pragma once
#include "yoyoyo_ascii_vector.h"
#define MAX_ALLOWED_COLLISIONS 4

//NOTE(Ray):At the moment this hashtable only allows for fixed size tables.
//resizing not implemented yet nor may ever be.
struct YoyoHashKeyEntry
{
	uint64_t value_index;
    bool indexed = false;//TODO(Ray):Try to get rid of this flag laters
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
    uint64_t table_size;
};

static YoyoHashTable YoyoInitHashTable(uint32_t start_count)
{
	YoyoHashTable result;
	result.keys = YoyoInitVector(start_count,YoyoHashKeyEntry,false);
	result.keys.allow_resize = false;
	result.values = YoyoInitVector(start_count,YoyoHashValueEntry,false);
	result.values.allow_resize = false;
    result.table_size = start_count;
	return result;
}

uint64_t YoyoMeowHashFunction(void* buffer,uint64_t size)
{
    meow_hash hash = MeowHash_Accelerated(0, size, buffer);
    return MeowU64From(hash,0);
}

static uint64_t YoyoHashFunction(YoyoHashTable* h_table,void* buffer,uint64_t size)
{
	uint64_t result;
	result = YoyoMeowHashFunction(buffer,size);
    return result % h_table->table_size;
}

static uint64_t YoyoAddElementToHashTable(YoyoHashTable* h_table,void* key,uint64_t key_size,void* element)
{
	uint64_t hash_index = YoyoHashFunction(h_table,key,key_size);
	YoyoHashKeyEntry e;
	e.collision_count = 0;
    e.indexed = true;
	e.value_index = hash_index;
	YoyoHashValueEntry v;
	v.value = element;

	YoyoHashKeyEntry* lu = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry, &h_table->keys, hash_index);
    Assert(lu);
    if(lu->indexed == true)
	{
		//TODO(Ray):Handle collisions.//ignore for now.
        //The expected rate for collision depends on your start count.
        //The smaller the count the higher the collision rate.  Typically we would make as large as
        //a start count as possible as we only store pointers to data and not actual copies in the
        //key value pairs memory footprint is minimal.  
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
    return hash_index;
}

#define YoyoGetElementByHash(Type,table,in,size) (Type*)YoyoGetElementByHash_(table,in,size)
static void* YoyoGetElementByHash_(YoyoHashTable* h_table,void* in,uint64_t size)
{
	void* result;
	uint64_t hash_index = YoyoHashFunction(h_table,in,size);
	YoyoHashKeyEntry* e = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,hash_index);
	if(e->collision_count == 0)
	{
		YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, hash_index);
		result = ve->value;
	}
	else
	{
		//Do string comparisons on key values till we get the proper entry.
        Assert(false);//for now assert if we have collision.s
	}
	return result;
}

static bool YoyoHashContains(YoyoHashTable* h_table,void* key,uint64_t size)
{
	uint64_t hash_index = YoyoHashFunction(h_table,key,size);
    YoyoHashKeyEntry* e = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,hash_index);
    if(e->indexed)
    {
        return true;
    }
    return false;
}

