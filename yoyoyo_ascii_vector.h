#pragma once

//TODO(Ray):Move memory from api to yoyoyoyo
#include <stdint.h>
#include <string.h>

struct YoyoVector
{
    void* base;
	uint32_t total_size;
	uint32_t unit_size;
	uint32_t count;
	uint32_t total_count;
	uint32_t at_index;
	int32_t start_at;
	uint32_t** free_list;
    MemoryArena mem_arena;
    bool pushable;
	float resize_ratio;//0.1 10% 1 100% default is 50% or 1/2 resizing
	bool allow_resize;
};

//BEGIN VECTOR LIFECYCLE
#define YoyoInitVector(start_size,type,pre_empt) YoyoInitVector_(start_size,sizeof(type),pre_empt)
#define YoyoInitVectorSize(start_size,size,pre_empt) YoyoInitVector_(start_size,size,pre_empt)
static YoyoVector YoyoInitVector_(uint32_t start_size, uint32_t unit_size, bool pre_empt = false)
{
    //TIMED_BLOCK();
    Assert(start_size > 0);
    Assert(unit_size > 0);
    
    YoyoVector result;
	uint32_t start_alloc_size = start_size * unit_size;
    result.total_size = 0;
    result.unit_size = unit_size;
    result.total_count = start_size;
    result.resize_ratio = 0.5f;
    result.allow_resize = true;
    
    result.at_index = 0;
    result.start_at = -1;
    result.pushable = true;
    //TODO(ray): change this to get memory froma a pre allocated partition.
    //void* starting_memory = PlatformAllocateMemory(start_alloc_size);
    //MemoryArena* partition = (MemoryArena*)starting_memory;
    //AllocatePartition(partition, start_alloc_size,partition+sizeof(MemoryArena*));
    void* base = PlatformAllocateMemory(start_alloc_size);
    AllocatePartition(&result.mem_arena, start_alloc_size,base);
    if(pre_empt)
    {
        result.count = start_size;
        PushSize(&result.mem_arena,start_alloc_size);
    }
    else
    {
        result.count = 0;
    }
    result.base = result.mem_arena.base;
    return result;
}

static void YoyoClearVector(YoyoVector *vector)
{
	Assert(vector);
    vector->pushable = true;
    vector->mem_arena.used = 0;
    vector->total_size = 0;
	vector->count = 0;
	vector->at_index = 0;
	vector->start_at = -1;
}

static void YoyoFreeVectorMem(YoyoVector *vector)
{
	if (vector->total_size > 0)
	{
		YoyoClearVector(vector);
        vector->pushable = true;
		vector->total_size = 0;
		vector->total_count = 0;
        PlatformDeAllocateMemory(vector->mem_arena.base, vector->mem_arena.size);
		vector->base = nullptr;
	}
}
//END VECTOR LIFECYCLE

//BEGIN VECTOR GENERAL USAGE FUNCTIONS

#define YoyoPushBack(vector, element) YoyoPushBack_(vector,(void*)&element,true);
#define YoyoPushBackPtr(vector, element) YoyoPushBack_(vector,(void*)element,true);
#define YoyoPushBackVoidPtr(vector, element) YoyoPushBack_(vector,(void*)&(*(u8*)element),true);
#define YoyoPushBackCopy(vector, element, copy) YoyoPushBack_(vector,(void*)&element,copy);
#define YoyoPushBackPtrCopy(vector, element, copy) YoyoPushBack_(vector,(void*)&element,copy);

#define YoyoStretchPushBack(vector, element) YoyoStretchPushBack_(vector,(void*)&element,true);
#define YoyoStretchPushBackPtr(vector, element) YoyoStretchPushBack_(vector,(void*)element,true);
#define YoyoStretchPushBackVoidPtr(vector, element) YoyoStretchPushBack_(vector,(void*)&(*(u8*)element),true);
#define YoyoStretchPushBackCopy(vector, element, copy) YoyoStretchPushBack_(vector,(void*)&element,copy);
#define YoyoStretchPushBackPtrCopy(vector, element, copy) YoyoStretchPushBack_(vector,(void*)&element,copy);

//NOTE(ray):If you use SetVectorElement Pushes will no longer work properly.
/**
 * \brief Push an element of a vector.  
 * \param vector Point to the vector to push to 
 * \param element One element of the type you registered when creating the vector 
 * \param copy Should we do a byte for byte copy?  
 * \return returns an index into the array in the vector for the created element
 */
static uint32_t YoyoPushBack_(YoyoVector* vector, void* element, bool copy = true,bool clear = false)
{
    //TIMED_BLOCK();
    Assert(vector && element);
    Assert(vector->pushable);
	Assert(vector->start_at == -1);//You must have forget to reset the vector or are trying to resize during iteration.
    
    partition_push_params mem_params = DefaultPartitionParams();
    mem_params.Flags = PartitionFlag_None;
    //TODO(ray):have some protection here to make sure we are added in the right type.
    //TODO(Ray):Might be better to allow for compile time switch to memcpy.
    uint8_t *ptr = (uint8_t*)PushSize(&vector->mem_arena, vector->unit_size,mem_params);
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

static uint32_t YoyoStretchPushBack_(YoyoVector* vector, void* element, bool copy = true,bool clear = false)
{
    Assert(vector && element);
    Assert(vector->pushable);
	Assert(vector->start_at == -1);//You must have forget to reset the vector or are trying to resize during iteration.

    //Execute the resize of the  buffer 
    if(vector->mem_arena.size <= vector->unit_size * (vector->count) && vector->count >= 1)
    {
        uint32_t old_size = vector->mem_arena.size;
		float resize_ratio = vector->resize_ratio;
        //TODO(Ray):Check about min and max implementation in yoyomath
        //TODO(Ray):Make sure we are on the propery byte boundary.
        float new_count = fmax(round(vector->count * vector->resize_ratio),1);
		uint32_t new_size = vector->mem_arena.size + (vector->unit_size * new_count);
        uint8_t* old_base_ptr = (uint8_t*)vector->mem_arena.base;
        
		vector->base = PlatformAllocateMemory(new_size);

        vector->mem_arena.base = vector->base;
        vector->mem_arena.size = new_size;
        
		memcpy(vector->base, (void*)old_base_ptr, vector->total_size);

		PlatformDeAllocateMemory(old_base_ptr, old_size);
    }
    
    //Than we do the push back as normal
    return YoyoPushBack_(vector,element,copy,clear);
}
//NOTE(Ray):Why are type and vector reversed between the push and access api methods!?!?!?! fix that.
//NOTE(Ray):We purposely have no bounds checking here its your responsibility.
//TODO(Ray):Implement a bounds check version of this function for when it might be good to have one.
//for now dont want need it.
#define YoyoGetVectorElement(type,vector,index) (type*)YoyoGetVectorElement_(vector,index)
#define YoyoGetVectorFirst(type,vector) (type*)YoyoGetVectorElement_(vector,0)
//NOTE(Ray):This is no good? Why get vector last instead of peek and why -1 and one noe sounds like a bug!!
//#define YoyoGetVectorLast(type,vector)  (type*)YoyoGetVectorElement_(vector,vector.count - 1)
//NOTE(Ray):Peek is prefferred at the moment.
#define YoyoPeekVectorElement(type,vector) (type*)YoyoGetVectorElement_(vector,*vector.count-1)
//#define YoyoIteraterPeekVector(type,vector) (type*)YoyoIterateVectorElement_(vector,*vector.at_index)
//#define YoyoIteraterPeekNextVector(type,vector) (type*)YoyoIterateVectorElement_(vector,*vector.at_index + 1)
//#define YoyoIteraterPeekOffsetVector(type,vector,offset) (type*)YoyoIterateVectorElement_(vector,*vector.at_index + offset)
static void* YoyoGetVectorElement_(YoyoVector* vector, uint32_t index)
{
	Assert(vector);
	if (index > vector->count - 1 || vector->count == 0)return 0;
	//TODO(Ray):May want to think about this. Need to give a hint to the client code.
	return  (uint8_t*)vector->base + (index * vector->unit_size);;
}

#define YoyoGetVectorElementAnyIndex(type,vector,index) (type*)YoyoGetVectorElementAnyIndex_(vector,index)
static void* YoyoGetVectorElementAnyIndex_(YoyoVector* vector, uint32_t index)
{
    Assert(vector);
    //if (index < 0 || index >= vector->total_count - 1)return 0;
    //TODO(Ray):May want to think about this. Need to give a hint to the client code.
    return  (uint8_t*)vector->base + (index * vector->unit_size);;
}

#define YoyoGetReferenceToElement(type,vector) (type**)YoyoGetVectorElement_(vector,*vector.count-1)
//#define YoyoIteraterPeekVector(type,vector) (type*)YoyoIterateVectorElement_(vector,*vector.at_index)
//#define YoyoIteraterPeekNextVector(type,vector) (type*)YoyoIterateVectorElement_(vector,*vector.at_index + 1)
//#define YoyoIteraterPeekOffsetVector(type,vector,offset) (type*)YoyoIterateVectorElement_(vector,*vector.at_index + offset)
static void** YoyoGetReferenceToElement_(YoyoVector* vector, uint32_t index)
{
	Assert(vector);
	if (index < 0 || index > vector->count - 1)return 0;
	//TODO(Ray):May want to think about this. Need to give a hint to the client code.
	uint8_t** Location = ((uint8_t**)&vector->base + (index * vector->unit_size));
	return (void**)Location;
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
    uint8_t *ptr = (uint8_t*)PushSize(&vector->mem_arena, vector->unit_size);
	vector->total_size += vector->unit_size;
	vector->count++;
	return (void*)ptr;
}

#define YoyoPopAndPeekVectorElement(type,vector) (Type*)PopAndPeekVectorElement_(vector)
static void* YoyoPopAndPeekVectorElement_(YoyoVector* vector)
{
	Assert(vector);
	void* Result = YoyoGetVectorElement_(vector, vector->count);
    vector->mem_arena.used -= vector->unit_size;
	vector->total_size -= vector->unit_size;
	vector->count--;
	return Result;
}

static void YoyoPopVectorElement(YoyoVector* vector)
{
	Assert(vector);
    vector->mem_arena.used -= vector->unit_size;
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

//NOTE(ray):Incorporates bounds checking and satisfies the iterator promise of a return by zero
//if you if you try to access our of bounds of the iterator index.
#define YoyoGetVectorIteratorOffset(type,vector,offset) (type*)YoyoGetVectorIterator_(vector,*vector.at_index + offset)
static void* YoyoGetVectorIterator_(YoyoVector* vector, uint32_t index)
{
	Assert(vector);
	if (index < 0 || index >= vector->count)return 0;
	//TODO(Ray):May want to think about this. Need to give a hint to the client code.
	void* Location = (uint8_t*)vector->base + (index * vector->unit_size);
	return Location;
}
static void YoyoResetVectorIterator(YoyoVector *vector)
{
	//TIMED_BLOCK();
	if (!vector)return;
	//Assert(vector);
	vector->at_index = 0;
	vector->start_at = -1;
}
//END VECTOR ITERATION FUNCS
