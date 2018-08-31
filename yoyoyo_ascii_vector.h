#pragma once

#include <stdint.h>
#include <cstring>

struct YoyoVector
{
    void* base;
	uint32_t total_size;
	uint32_t max_size;
	uint32_t unit_size;
	uint32_t count;
	uint32_t total_count;
	uint32_t at_index;
	int32_t start_at;
	uint32_t** free_list;
    MemoryArena* mem_arena;
    bool pushable;
	float resize_ratio = 0.5f;//0.1 10% 1 100% default is 50% or 1/2 resizing
	bool allow_resize = true;
};

//BEGIN VECTOR LIFECYCLE
#define YoyoInitVector(start_size,type,pre_empt) YoyoInitVector_(start_size,sizeof(type),pre_empt)
static YoyoVector YoyoInitVector_(uint32_t start_size, uint32_t unit_size, bool pre_empt = false)
{
    //TIMED_BLOCK();
    Assert(start_size > 0);
    Assert(unit_size > 0);
    
    YoyoVector result;
	result.max_size = start_size * unit_size;
    result.total_size = 0;
    result.unit_size = unit_size;
    result.total_count = start_size;
    
    result.at_index = 0;
    result.start_at = -1;
    result.pushable = true;
    //TODO(ray): change this to get memory froma a pre allocated partition.
    void* starting_memory = YoyoPlatformAllocateMemory(result.max_size);
    MemoryArena* partition = (MemoryArena*)starting_memory;
    YoyoAllocateArena(partition, result.max_size,partition+sizeof(MemoryArena*));
    
    result.mem_arena = partition;
    if(pre_empt)
    {
        result.count = start_size;
        YoyoPushSize_(partition,result.max_size);
    }
    else
    {
        result.count = 0;
    }
    result.base = partition->base;
    return result;
}

static void YoyoClearVector(YoyoVector *vector)
{
	//TIMED_BLOCK();
	Assert(vector);
	vector->mem_arena->used = 0;
	vector->count = 0;
	vector->at_index = 0;
	vector->start_at = -1;
	vector->mem_arena->temp_count = 0;
}

static void YoyoFreeVectorMem(YoyoVector *vector)
{
	//TIMED_BLOCK();
	if (vector->total_size > 0)
	{
		YoyoClearVector(vector);
		vector->total_size = 0;
		vector->total_count = 0;
		YoyoPlatformDeallocateMemory(vector->mem_arena->base, vector->mem_arena->size);
		vector->base = nullptr;
	}
}
//END VECTOR LIFECYCLE

//BEGIN VECTOR GENERAL USAGE FUNCTIONS
#define YoyoPushBack(vector, element) YoyoPushBack_(vector,(void*)&element,true);
#define YoyoPushBackPtr(vector, element) YoyoPushBack_(vector,(void*)element,true);
#define YoyoPushBackCopy(vector, element, copy) YoyoPushBack_(vector,(void*)&element,copy);
#define YoyoPushBackPtrCopy(vector, element, copy) YoyoPushBack_(vector,(void*)&element,copy);
//NOTE(ray):If you use SetVectorElement Pushes will no longer work properly.
/**
 * \brief Push an element of a vector.  
 * \param vector Point to the vector to push to 
 * \param element One element of the type you registered when creating the vector 
 * \param copy Should we do a byte for byte copy?  
 * \return returns an index into the array in the vector for the created element
 */
static uint32_t YoyoPushBack_(YoyoVector* vector, void* element, bool copy = true)
{
    //TIMED_BLOCK();
    Assert(vector && element);
    Assert(vector->pushable);
	Assert(vector->start_at == -1);//You must have forget to reset the vector or are trying to resize during iteration.

    //TODO(Ray):Test this.
    //check if we have space if not resize to create it.
    if(vector->max_size < vector->unit_size * (vector->count + 1))
    {
		uint32_t new_size = vector->total_size + (vector->total_size * vector->resize_ratio);
		uint8_t* temp_ptr = (uint8_t*)vector->mem_arena->base;
		vector->base = vector->mem_arena->base = YoyoPlatformAllocateMemory(new_size);
		vector->mem_arena->size = new_size;
		memcpy(vector->base, (void*)temp_ptr, vector->total_size);
		vector->max_size = new_size;
    }
    //TODO(ray):have some protection here to make sure we are added in the right type.
    uint8_t *ptr = (uint8_t*)YoyoPushSize(vector->mem_arena, vector->unit_size);
    if (copy)
    {
        uint32_t byte_count = vector->unit_size;
        uint32_t index = 0;
        while (index < byte_count)
        {
            *ptr++ = *((uint8_t*)element + index);
            index++;
        }
    }
    else
    {
        ptr = (uint8_t*)element;
    }
    
    vector->total_size += vector->unit_size;
    uint32_t result_index = vector->count++;
    return result_index;
}

#define YoyoGetVectorElement(type,vector,index) (type*)YoyoGetVectorElement_(vector,index)
#define YoyoGetVectorFirst(type,vector) (type*)YoyoGetVectorElement_(vector,0)
#define YoyoGetVectorLast(type,vector) (type*)YoyoGetVectorElement_(vector,vector.count)
#define YoyoPeekVectorElement(type,vector) (type*)YoyoGetVectorElement_(vector,*vector.count-1)
static void* YoyoGetVectorElement_(YoyoVector* vector, uint32_t index)
{
	Assert(vector);
	//TODO(Ray):May want to think about this. Need to give a hint to the client code.
	void* Location = (uint8_t*)vector->base + (index * vector->unit_size);
	return Location;
}

static void* YoyoSetVectorElement(YoyoVector* vector, uint32_t element_index, void* element, bool copy = true)
{
	//TIMED_BLOCK();
	Assert(vector && element);
	vector->pushable = false;
	//TODO(ray):have some protection here to make sure we are added in the right type.
	void* location = (uint8_t*)vector->base + (element_index * vector->unit_size);
	uint8_t* ptr = (uint8_t*)location;
	if (copy)
	{
		uint32_t byte_count = vector->unit_size;
		uint32_t index = 0;
		while (index < byte_count)
		{
			*ptr++ = *((uint8_t*)element + index);
			index++;
		}
	}
	else
	{
		ptr = (uint8_t*)element;
	}

	vector->total_size += vector->unit_size;
	vector->count++;
	return location;
}

#define YoyoPushEmptyVectorElement(vector) YoyoPushEmptyVectorElement_(vector)
#define YoyoPushAndCastEmptyVectorElement(type,vector) (type*)YoyoPushEmptyVectorElement_(vector)
static void* YoyoPushEmptyVectorElement_(YoyoVector* vector)
{
	uint8_t *ptr = (uint8_t*)YoyoPushSize(vector->mem_arena, vector->unit_size);

	vector->total_size += vector->unit_size;
	vector->count++;
	return (void*)ptr;
}

#define YoyoPopAndPeekVectorElement(type,vector) (Type*)PopAndPeekVectorElement_(vector)
static void* YoyoPopAndPeekVectorElement_(YoyoVector* vector)
{
	Assert(vector);
	void* Result = YoyoGetVectorElement_(vector, vector->count);
	vector->mem_arena->used -= vector->unit_size;
	vector->total_size -= vector->unit_size;
	vector->count--;
	return Result;
}

static void YoyoPopVectorElement(YoyoVector* vector)
{
	Assert(vector);
	vector->mem_arena->used -= vector->unit_size;
	vector->total_size -= vector->unit_size;
	vector->count--;
}
//END VECTOR GENERAL USAGE FUNCTIONS

//BEGIN VECTOR ITERATION FUNCS
#define YoyoIterateVector(vector,type) (type*)YoyoIterateVectorElement_(vector)
#define YoyoIterateVectorFromIndex(vector,type,index) (type*)YoyoIterateVectorElement_(vector,index)
#define YoyoIterateVectorFromToIndex(vector,type,index,to_index) (type*)YoyoIterateVectorElement_(vector,index,to_index)
static void* YoyoIterateVectorElement_(YoyoVector *vector, int start_at = -1, int end_at = -1)
{
	//TIMED_BLOCK();
	Assert(vector);
	if (vector->at_index >= vector->count)return 0;
	if (start_at >= 0 && vector->start_at < 0)
	{
		vector->at_index = start_at;
		vector->start_at = start_at;
	}
	else if (end_at >= 0 && vector->at_index == (uint32_t)end_at)
	{
		return 0;
	}
	return YoyoGetVectorElement_(vector, vector->at_index++);
}

static void YoyoResetVectorIterator(YoyoVector *vector)
{
	//TIMED_BLOCK();
	Assert(vector);
	vector->at_index = 0;
	vector->start_at = -1;
}

//END VECTOR ITERATION FUNCS

