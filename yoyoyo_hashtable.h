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

struct YoyoHashTable
{
    YoyoVector key_backing_array;
	YoyoVector keys;
	YoyoVector values;
    YoyoVector collisions;
    YoyoVector collision_free_list;
    
    memory_index table_size;
    memory_index collision_count;

    memory_index key_size;
    memory_index key_value;
};

#define YoyoInitHashTableKeyType(start_count,key_type) YoyoInitHashTable(start_count,sizeof(key_type))
static YoyoHashTable YoyoInitHashTable(memory_index start_count,memory_index key_size)
{
	YoyoHashTable result = {};
    result.collision_free_list = YoyoInitVector(1, YoyoHashCollisionEntry, false);
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
/*
static uint64_t YoyoHashExtractHashIndex(uint64_t hash_index)
{
    //just mask out the collision index which is high 4 bits
    return (hash_index) & (0x0FFFFFFF);
}

static uint64_t YoyoHashExtractCollisionIndex(uint64_t hash_index)
{
    //shift bit 60 right to 0 and mask out all but bottom 4 bits
    return (hash_index >> 60) & (0x0000000F);
}

static uint64_t YoyoHashPackCollisionIndex(uint64_t collision_index, uint64_t hash_index)
{
    return (collision_index << 60) & hash_index;
}
 */
//NOTE(Ray):We will reserver 0 element index to be the no match index
//NOTE(Ray):Returned here is a 64 bit uint with the bottom 4 bits reserved for the collision index
//and 60 bits for the hash index
static uint64_t YoyoAddElementToHashTable(YoyoHashTable* h_table,void* key,uint64_t key_size,void* element)
{
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
        Assert(key_size == h_table->key_backing_array.unit_size);
        uint64_t index = YoyoStretchPushBackVoidPtr(&h_table->key_backing_array,key);
        lu->backing_index = index;
	}
	else
	{
        void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,lu->backing_index);
        if(!memcmp(key,backing_key,h_table->key_size))
        {
            //If the key exist we skip not adding.
            //Value already exist will not add
            return 0;
        }
        else
        {
            YoyoHashCollisionEntry* head_entry = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,lu->collision_head_index);
            YoyoHashCollisionEntry* at = head_entry;
            YoyoHashCollisionEntry* prev_at = head_entry;
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
                    return 0;
                }
                prev_at = at;
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
            }
            else
            {
                new_coll_index = (uint64_t)YoyoPopAndPeekVectorElement(uint64_t, &h_table->collisions);
                YoyoSetVectorElement(&h_table->collisions,new_coll_index,&ce);
            }

            YoyoHashCollisionEntry* new_coll_entry = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,new_coll_index);
            new_coll_entry->index = new_coll_index;
            //->next_index = new_coll_index;
        
            if(has_head)
            {
                prev_at = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,prev_at_index);
                prev_at->next_index = new_coll_index;
            }
            else
            {
                lu->collision_head_index = new_coll_index;
            }
        
            ++lu->collision_count;            
        }

	}
    return hash_index;
}

#define YoyoGetElementByHash(type,table,in,size) (type*)YoyoGetElementByHash_(table,in,size)
static void* YoyoGetElementByHash_(YoyoHashTable* h_table,void* key,uint64_t size)
{
	void* result = 0;
	uint64_t hash_index = YoyoHashFunction(h_table,key,size);
    
	YoyoHashKeyEntry* lu = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,hash_index);
	if(lu->collision_count == 0)
	{
 		YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, hash_index);
		result = ve->value;
	}
	else
	{
        void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,lu->backing_index);
        if(!memcmp(key,backing_key,h_table->key_size))
        {
            YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, hash_index);
            result = ve->value;
        }
        else
        {
            YoyoHashCollisionEntry* head_entry = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,lu->collision_head_index);
            YoyoHashCollisionEntry* at = head_entry;
            YoyoHashCollisionEntry* prev_at = head_entry;
            while(at)
            {
                YoyoHashKeyEntry key_entry = at->key;
                void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,key_entry.backing_index);
                if(!memcmp(key,backing_key,h_table->key_size))
                {
                    result = at->value.value;
                }
                prev_at = at;
                at = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,at->next_index);
            }
        }
	}
	return result;
}

static void YoyoHashTableRemoveElement(YoyoHashTable* h_table,void* key)
{
    //Assert(false);
    uint64_t hash_index = YoyoHashFunction(h_table,key,h_table->key_size);
	YoyoHashKeyEntry* lu = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,hash_index);
    if(lu->collision_count == 0)
    {
//        lu->indexed = false;
//NOTE(Ray):Keep collision count
//        e->collision_count = 0;
        lu->tombstoned = true;
        YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, hash_index);
        ve->value = 0;
        YoyoClearVectorElement(&h_table->key_backing_array,lu->backing_index);
    }
    else
    {
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
            int index = 0;
            int last_history_index = 0;
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
            YoyoStretchPushBack(&h_table->collision_free_list,free_index);
            //we will want one back from where we were
            int prev_prev_at_index =  (last_history_index - 1) % 2;
            YoyoHashCollisionEntry* prev_prev_at = history_stack[prev_prev_at_index];
            if(prev_prev_at)
                prev_prev_at->next_index = prev_at->next_index;
        }

    }
}

#if 0
#define YoyoGetElementByIndex(type,table,index) (type*)YoyoGetElementByIndex_(table,index)
static void* YoyoGetElementByIndex_(YoyoHashTable* h_table,uint64_t index)
{
    uint64_t hash_index = (index);
	void* result;
	YoyoHashKeyEntry* e = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,hash_index);
	if(e->collision_count == 0)
	{
 		YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, index);
		result = ve->value;
	}
	else
	{
        YoyoHashCollisionEntry* at = e->collision_head;
        YoyoHashCollisionEntry* tail = e->collision_head;
        while(at)
        {
            YoyoHashKeyEntry key_entry = at->key;
            void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,key_entry.backing_index);            
            if(!memcmp(key,backing_key,h_table->key_size))
            {
                //Key match found
                result = at->value.value;
            }
            at = at->n;
        }        
        /*
        //NOTE(ray):Since we dont have a key to check against here we need to store the collision
        //index into the hash_index returned to the client.
        //extract collision index
        uint64_t collision_index = YoyoHashExtractCollisionIndex(index);
        Assert(collision_index < e->collision_count);
        YoyoHashCollisionEntry* ce = YoyoGetVectorElementAnyIndex(YoyoHashCollisionEntry,&h_table->collisions,collision_index);
        result = ce->value.value;
        */
	}
	return result;
}
#endif

#if 0
//FOr removal we keep the collision count but zero out the backing key as it is intended to be no longer stored in the hash.
//This allows us to remove elements without doing any patching lists.
static void YoyoHashTableRemoveElementByIndex(YoyoHashTable* h_table,uint64_t index)
{
    //Assert(false);
    uint64_t hash_index = YoyoHashExtractHashIndex(index);
	YoyoHashKeyEntry* e = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,index);
    if(e->collision_count == 0)
    {
        e->indexed = false;
//NOTE(Ray):Keep collision count
//        e->collision_count = 0;
        YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, index);
        ve->value = 0;
//        YoyoClearVectorElement(&h_table->key_backing_array,e->backing_index);
    }
    else
    {
        YoyoHashCollisionEntry* prev_at = e->collision_head;
        YoyoHashCollisionEntry* at = e->collision_head;
        YoyoHashCollisionEntry* tail = e->collision_head;
        while(at)
        {
            YoyoHashKeyEntry key_entry = at->key;
            void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,key_entry.backing_index);            
            if(!memcmp(key,backing_key,h_table->key_size))
            {
//remove this.
                break;
            }
            prev_at = at;
            at = at->n;
        }
        uint64_t free_index = at->index;
        YoyoStretchPushBack(&h_table->collision_free_list,free_index);
        prev_at->n = at->n;
/*
        uint64_t collision_index = YoyoHashExtractCollisionIndex(index);
        Assert(collision_index < e->collision_count);
        YoyoHashCollisionEntry* ce = YoyoGetVectorElementAnyIndex(YoyoHashCollisionEntry,&h_table->collisions,collision_index);
        //NOTE(ray):At this point the backing can be resused.
        ce->key.indexed = false;
//        ce->key.collision_count = 0;
        ce->value.value = 0;
*/
    }
}
#endif

static bool YoyoHashContains(YoyoHashTable* h_table,void* key,uint64_t size)
{
	uint64_t hash_index = YoyoHashFunction(h_table,key,size);
    YoyoHashKeyEntry* lu = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,hash_index);
    if(lu->indexed && lu->collision_count == 0)
    {
        return true;
    }
    else if(lu->indexed)//collision
    {
        YoyoHashCollisionEntry* head_entry = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,lu->collision_head_index);
        YoyoHashCollisionEntry* at = head_entry;
        YoyoHashCollisionEntry* prev_at = head_entry;
        while(at)
        {
            YoyoHashKeyEntry key_entry = at->key;
            void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,key_entry.backing_index);
            if(!memcmp(key,backing_key,h_table->key_size))
            {
                return true;
            }
            prev_at = at;
            at = YoyoGetVectorElement(YoyoHashCollisionEntry,&h_table->collisions,at->next_index);
        }

/*
//check for head value than for all 
        void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,e->backing_index);
        if(memcmp(key,backing_key,h_table->key_size))
        {
           // YoyoHashValueEntry* ve = YoyoGetVectorElementAnyIndex(YoyoHashValueEntry,&h_table->values, hash_index);
           // result = ve->value;
            return true;
        }
        else
        {
            memory_index start_index = e->collision_head_index;
            for(int i = start_index;i < 10;++i)
            {
                YoyoHashCollisionEntry* ce = YoyoGetVectorElementAnyIndex(YoyoHashCollisionEntry,&h_table->collisions,i);
                void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,ce->index);
                if(memcmp(key,backing_key,h_table->key_size))
                {
//Found same value in key this is the one.
                   // result = ce->value.value;
                    return true;
                }
            }
        }
*/
    }
    return false;
}
/*
static bool YoyoHashKeyContains(YoyoHashTable* h_table,uint64_t hash_index)
{
    YoyoHashKeyEntry* e = YoyoGetVectorElementAnyIndex(YoyoHashKeyEntry,&h_table->keys,hash_index);
    if(e->indexed)
    {
        return true;
    }
    else if(e->indexed)//collision
    {
        YoyoHashCollisionEntry* at = lu->collision_head;
        YoyoHashCollisionEntry* tail = lu->collision_head;
        while(at)
        {
            YoyoHashKeyEntry key_entry = at->key;
            void* backing_key = YoyoGetVectorElement_(&h_table->key_backing_array,key_entry.backing_index);            
            if(memcmp(key,backing_key,h_table->key_size))
            {
                return true;
            }
            at = at->n;
        }        
    }
    return false;
}
*/

#define YOYOYO_HASHTABLE_H
#endif
