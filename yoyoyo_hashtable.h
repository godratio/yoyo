#if !defined(YOYOYO_HASHTABLE_H)
//NOTE(Ray):
/*
 * This hashtabe didnt turn out like I wanted and a replacement will be created
 in the future but for now it works even though its a lil piggy and ineffecient
 */
#include "yoyoyo_ascii_vector.h"
#define MAX_ALLOWED_COLLISIONS 4
//NOTE(Ray):Keys are required to have fixed size.
//For arbiturary sized keys see differents style hashtable.k

//The index returned here is 2^60 bit hash index 2^4 bits collision index 
//so look ups can be acheived through a key or an index directly.
//Which give a behavior similar to a dynamic associative array.

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
    bool tombstoned = false;//TODO(Ray):Indexed and tombstoned should be in a state index variable
	uint32_t collision_count;
    int collision_head_index;
    struct YoyoHashCollisionEntry* collision_head;
};

struct YoyoHashValueEntry
{
	void* value;
//	struct YoyoHashKeyEntry hash_collisions[MAX_ALLOWED_COLLISIONS];
};

struct YoyoHashCollisionEntry
{
    YoyoHashKeyEntry key;
    YoyoHashValueEntry value;
//    YoyoHashCollisionEntry* n;//next
    memory_index index;
    int next_index;//next index rather than a pointer for resizing of the backing array without patching pointerns
};

struct YoyoHashAddElementResult
{
    bool is_succeed;
    uint64_t result;
};

struct YoyoHashTable
{
    YoyoVector key_backing_array;
	YoyoVector keys;
	YoyoVector values;
    YoyoVector collisions;
    YoyoVector collision_free_list;
    
    uint64_t table_size;
    uint64_t collision_count;

    uint64_t key_size;
    uint64_t key_value;
};

#define YoyoInitHashTableKeyType(start_count,key_type) YoyoInitHashTable(start_count,sizeof(key_type))
static YoyoHashTable YoyoInitHashTable(uint64_t start_count,uint64_t key_size)
{
	YoyoHashTable result = {};
    result.collision_free_list = YoyoInitVector(1, uint64_t, false);
    result.key_backing_array = YoyoInitVectorSize(1,key_size,false);
    result.key_size = key_size;
    Assert(key_size == result.key_backing_array.unit_size);
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

static YoyoHashAddElementResult YoyoAddElementToHashTable(YoyoHashTable* h_table,void* key,uint64_t key_size,void* element)
{
    YoyoHashAddElementResult result = {};
    result.is_succeed = true;
    
	uint64_t hash_index = YoyoHashFunction(h_table,key,key_size);
    
    YoyoHashKeyEntry e = {};
    e.collision_count = 0;
    e.indexed = true;
    e.tombstoned = false;
    e.value_index = hash_index;
    e.collision_head = 0;
    e.collision_count = 0;
    e.collision_head_index = -1;
    YoyoHashValueEntry v = {};
    v.value = element;
//NOTE(Ray):TODO(Ray):Vlaues are empty in manycases FIX THIS NOW! 
	YoyoHashKeyEntry* lu = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry, &h_table->keys, hash_index);
    Assert(lu);
    if(lu->indexed == false || lu->tombstoned == true)
	{
        if(lu->tombstoned)
        {
            e.collision_count = lu->collision_count;
            e.collision_head_index = lu->collision_head_index;
            e.collision_head = lu->collision_head;
        }
        
		YoyoSetVectorElement(&h_table->keys, hash_index, &e);
		YoyoSetVectorElement(&h_table->values, hash_index, &v);
        YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, hash_index);
        //result = ve->value;
        Assert(key_size == h_table->key_backing_array.unit_size);
        uint64_t index = YoyoStretchPushBackVoidPtr(&h_table->key_backing_array,key);
        lu->backing_index = index;
        Assert(index < h_table->key_backing_array.count);
	}
	else
	{
        void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,lu->backing_index);
        if(!memcmp(key,backing_key,h_table->key_size))
        {
            //If the key exist we skip not adding.
            //Value already exist will not add
            result.is_succeed = false;
            result.result = 0;            
            return result;
        }
        else
        {
            YoyoHashCollisionEntry* head_entry = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,lu->collision_head_index);
            YoyoHashCollisionEntry* at = head_entry;
            int prev_at_index = -1;
            bool has_head = false;
            if(head_entry)has_head = true;
            while(at)
            {
                YoyoHashKeyEntry key_entry = at->key;
                void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,key_entry.backing_index);
                if(!memcmp(key,backing_key,h_table->key_size))
                {
                    //If the key exist we skip not adding.
                    //Value already exist will not add
                    result.is_succeed = false;
                    result.result = 0;
                    return result;
                }
                prev_at_index = at->index;
                at = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,at->next_index);
            }

            ++h_table->collision_count;//meaningless
        
            YoyoHashCollisionEntry ce = {};
            ce.key = e;
            ce.value = v;
            ce.next_index = -1;

            Assert(key_size == h_table->key_backing_array.unit_size);
            ce.key.backing_index = YoyoStretchPushBackVoidPtr(&h_table->key_backing_array,key);

            uint64_t new_coll_index;
            if(h_table->collision_free_list.count == 0)
            {
                new_coll_index = YoyoStretchPushBack(&h_table->collisions,ce);
                if(has_head)
                {
                    YoyoHashCollisionEntry* prev_at = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,prev_at_index);
                    //prev_at->next_index = new_coll_index;
                    Assert(prev_at->index != new_coll_index);
                }
            }
            else
            {
                Assert(h_table->collision_free_list.count > 0);
                new_coll_index = *YoyoPeekVectorElement(uint64_t, &h_table->collision_free_list);
                Assert(new_coll_index >= 0 && new_coll_index <= h_table->table_size);
                YoyoSetVectorElement(&h_table->collisions,new_coll_index,&ce);
                YoyoPopVectorElement(&h_table->collision_free_list);
            }

            YoyoHashCollisionEntry* new_coll_entry = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,new_coll_index);
            Assert(new_coll_index >= 0 && new_coll_index <= h_table->table_size);
            new_coll_entry->index = new_coll_index;
        
            if(has_head && prev_at_index != 0)
            {
                YoyoHashCollisionEntry* prev_at = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,prev_at_index);
                prev_at->next_index = new_coll_index;
                Assert(prev_at->next_index != prev_at->index);
            }
            else
            {
                lu->collision_head_index = new_coll_index;
            }
            
            ++lu->collision_count;            
        }
	}
    result.result = hash_index;
    return result;
}

#define YoyoGetElementByHash(type,table,in,size) (type*)YoyoGetElementByHash_(table,in,size)
static void* YoyoGetElementByHash_(YoyoHashTable* h_table,void* key,uint64_t size)
{
	void* result = 0;
	uint64_t hash_index = YoyoHashFunction(h_table,key,size);
	YoyoHashKeyEntry* lu = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,hash_index);
	if(lu->collision_count == 0)
	{
        if(!lu->tombstoned)
        {
            YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, hash_index);
            result = ve->value;
        }
	}
	else
	{
        YoyoHashKeyEntry* backing_key = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->key_backing_array,lu->backing_index);
        if(!memcmp(key,(void*)backing_key,h_table->key_size))
        {
            YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, hash_index);
            result = ve->value;
        }
        else
        {
            YoyoHashCollisionEntry* head_entry = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,lu->collision_head_index);
            YoyoHashCollisionEntry* at = head_entry;
            while(at)
            {
                YoyoHashKeyEntry key_entry = at->key;
                void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,key_entry.backing_index);
                if(!memcmp(key,backing_key,h_table->key_size))
                {
                    if(!key_entry.tombstoned)
                        result = at->value.value;
                }
                at = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,at->next_index);
            }
        }
	}
	return result;
}

static void YoyoHashTableRemoveElement(YoyoHashTable* h_table,void* key)
{
    uint64_t hash_index = YoyoHashFunction(h_table,key,h_table->key_size);
	YoyoHashKeyEntry* lu = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,hash_index);
    if(lu->collision_count == 0 && lu->indexed)
    {
        lu->tombstoned = true;
        YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, hash_index);
        ve->value = 0;
        YoyoClearVectorElement(&h_table->key_backing_array,lu->backing_index);
    }
    else
    {
        uint64_t prev_index = 0;
        void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,lu->backing_index);
        if(!memcmp(key,backing_key,h_table->key_size))
        {
            YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, hash_index);
            ve->value = 0;
        }
        else
        {
            YoyoHashCollisionEntry* head_entry = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,lu->collision_head_index);
            YoyoHashCollisionEntry* at = head_entry;
            YoyoHashCollisionEntry* prev_at = head_entry;
            YoyoHashCollisionEntry* history_stack[2] = {0,0};
            uint64_t index = 0;
            uint64_t last_history_index = 0;
            while(at)
            {
                YoyoHashKeyEntry key_entry = at->key;
                void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,key_entry.backing_index);
                if(!memcmp(key,(void*)backing_key,h_table->key_size))
                {
                    at->key.tombstoned = true;
                    at->value.value = 0;
                    YoyoClearVectorElement(&h_table->key_backing_array,key_entry.backing_index);
                    break;
                }
                prev_at = at;
                at = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,at->next_index);
                ++index; 
                last_history_index = (index - 1) % 2;//keep wrapping around keeping at least two levels of history
                history_stack[last_history_index] = prev_at;
            }
            uint64_t free_index = prev_at->index;
            Assert(free_index >= 0 && free_index <= h_table->table_size);
            if(free_index == 0)
            {
                int a = 0;
            }
            uint64_t index_result = YoyoStretchPushBack(&h_table->collision_free_list,free_index);
            //we will want one back from where we were
            uint64_t prev_prev_at_index =  (last_history_index - 1) % 2;
            YoyoHashCollisionEntry* prev_prev_at = history_stack[prev_prev_at_index];
            if(prev_prev_at)
            {
                Assert(prev_at->next_index >= 0 && prev_at->next_index <= h_table->table_size);
                prev_prev_at->next_index = prev_at->next_index;
            }
        }
    }
}

static bool YoyoHashContains(YoyoHashTable* h_table,void* key,uint64_t size)
{
    uint64_t hash_index = YoyoHashFunction(h_table,key,size);
    YoyoHashKeyEntry* lu = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,hash_index);
    if(lu->indexed && lu->collision_count == 0)
    {
        if(lu->tombstoned)
        {
            return false;            
        }
        else
        {
            void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,lu->backing_index);
            //Assert(backing_key);
            if(!memcmp(key,(void*)backing_key,h_table->key_size))
            {
                return true;
            }
        }
    }
    else if(lu->indexed)//collision
    {
        YoyoHashCollisionEntry* head_entry = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,lu->collision_head_index);
        YoyoHashCollisionEntry* at = head_entry;
        while(at)
        {
            YoyoHashKeyEntry key_entry = at->key;
            void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,key_entry.backing_index);
            if(!memcmp(key,(void*)backing_key,h_table->key_size))
            {
                if(key_entry.tombstoned)
                    return false;
                else
                    return true;
            }
            at = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,at->next_index);
        }
    }
    return false;
}

#define YOYOYO_HASHTABLE_H
#endif
