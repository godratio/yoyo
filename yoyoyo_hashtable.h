#pragma once
#include "yoyoyo_ascii_vector.h"
#define MAX_ALLOWED_COLLISIONS 4

//NOTE(Ray):This hash TABLE is made for storing indexes particularly however/
//you can store arbiturary pointers provided the pointers wont move for example if they
//are part of a container that could resize and not restore pointers.

//NOTE(Ray):At the moment this hashtable only allows for fixed size tables.
//resizing not implemented yet nor may ever be.
struct YoyoHashKeyEntry
{
	uint64_t value_index;
    uint64_t backing_index;
    bool indexed = false;//TODO(Ray):Try to get rid of this flag laters
	uint32_t collision_count;
    memory_index collision_head_index;
};

struct YoyoHashValueEntry
{
	void* value;
//	struct YoyoHashKeyEntry hash_collisions[MAX_ALLOWED_COLLISIONS];
};

struct YoyoHasCollisionEntry
{
    YoyoHashKeyEntry key;
    YoyoHashValueEntry value;
    uint32_t index;
};

struct YoyoHashTable
{
    YoyoVector key_backing_array;
	YoyoVector keys;
	YoyoVector values;
    YoyoVector collisions;

    memory_index table_size;
    memory_index collision_count;

    memory_index key_size;
    memory_index key_value;
};

#define YoyoHashTableInit(key_type,value_type,start_count) YoyoInitHashTable(start_count,sizeof(key_type),sizeof(value_type)
static YoyoHashTable YoyoInitHashTable(memory_index start_count,memory_index key_size,memory_index value_size)
{
	YoyoHashTable result = {};
    result.key_backing_array = YoyoInitVector(1,key_size,false);
    result.key_backing_array.allow_resize = false;
	result.keys = YoyoInitVector(start_count,YoyoHashKeyEntry,false);
	result.keys.allow_resize = false;
	result.values = YoyoInitVector(start_count,YoyoHashValueEntry,false);
	result.values.allow_resize = false;
    result.table_size = start_count;
    result.collisions = YoyoInitVector(1,YoyoHashCollisionEntry,false);
	return result;
}

static uint64_t YoyoMeowHashFunction(void* buffer,uint64_t size)
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

//NOTE(Ray):We will reserver 0 element index to be the no match index
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
    if(lu->indexed == false)
	{
		//TODO(Ray):Compare if this same keyelement is already added.
		//if yes return cant add because key already exists
		//if no add element
		YoyoSetVectorElement(&h_table->keys, hash_index, &e);
		YoyoSetVectorElement(&h_table->values, hash_index, &v);
        YoyoPushBack(&h_table->key_backing_array,key):
	}
	else
	{
//Max 10 collisions they are stored linearly
 		YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, hash_index);
        void* backing_key = YoyoGetVectorElement(&h_table->key_backing_array,lu->backing_index);
        if(memcmp(key,backing_key,h_table->key_size))
        {
            //Value already exist will not add
            return 0;
        }
        
        ++h_table->collision_count;
        YoyoHashCollisionEntry ce = {};
        ce.key = e;
        ce.value = v;
        if(lu->collision_count == 0)
        {
            //calculate a collision head index
            //and enough space for the rest of the entries
            YoyoHashCollisionEntry emptyce = {};
            lu->collision_head_index = YoyoPushBack(&h_table->collisions,emptyce);
            for(int i = 0;i < 9;++i)
            {
                YoyoPushBack(&h_table->collisions,emptyce);
            }
        }
        //add in collision entry
        Assert(lu->collision_head_index + lu->collision_count < h_table->collisions.count);
        YoyoSetVectorElement(&h_table->collisions,lu->collision_head_index + lu->collision_count,ce);
//		Assert(false);
	}

    return hash_index;
}

#define YoyoGetElementByHash(type,table,in,size) (type*)YoyoGetElementByHash_(table,in,size)
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
        if(e->)
        memory_index start_index = e->collision_head_index;
        for(int i = start_index;i < 10;++i)
        {
            YoyoCollisionEntry* ce = YoyoGetVectorElementAnyIndex(YoyoCollisionEntry,&h_table->collisions,i);
            YoyoHashValueEntry cve = ce->value;
            if(ce->index == )

        }

		//Do string comparisons on key values till we get the proper entry.
        Assert(false);//for now assert if we have collision.s
	}
	return result;
}

#define YoyoGetElementByIndex(type,table,index) (type*)YoyoGetElementByIndex_(table,index)
static void* YoyoGetElementByIndex_(YoyoHashTable* h_table,uint64_t index)
{
	void* result;
	YoyoHashKeyEntry* e = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,index);
	if(e->collision_count == 0)
	{
 		YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, index);
		result = ve->value;
	}
	else
	{
		//Do pointer comparisons on key values till we get the proper entry.
        //This works because we only store pointers never actual values and the use should
        //always be aware of this and not try to storing in pointers off of something that
        //potentionally collides
        
        Assert(false);//for now assert if we have collision.s
	}
	return result;
}

static void YoyoHashTableRemoveElementByIndex(YoyoHashTable* h_table,uint64_t index)
{
	void* result;
	YoyoHashKeyEntry* e = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,index);
    e->indexed = false;
	e->collision_count = 0;
    YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, index);
    ve->value = 0;
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

static bool YoyoHashKeyContains(YoyoHashTable* h_table,uint64_t hash_index)
{
    YoyoHashKeyEntry* e = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,hash_index);
    if(e->indexed)
    {
        return true;
    }
    return false;
}

