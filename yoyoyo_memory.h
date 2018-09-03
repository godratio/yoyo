#pragma once
#include <cstdint>
#include <stddef.h>
#if WINDOWS
#include "yoyoyo_mem_win.h"
#elif OSX
#include "yoyo_mem_osx.h"
#elif IOS
#endif

enum YoyoPartitionPushFlag
{
	YoyoPartitionFlag_ClearToZero = 0x1,
};

struct ParitionPushParams
{
	uint32_t flags;
	uint32_t alignment;
};

inline ParitionPushParams
DefaultPartitionParams()
{
	ParitionPushParams params;
	params.flags = YoyoPartitionFlag_ClearToZero;
	params.alignment = 4;
	return(params);
}

struct MemoryArena
{
	void* base;
	size_t size;
	uint32_t used;
	uint32_t temp_count;
};

struct TempMemory
{
	MemoryArena *partition;
	uint32_t used;
};

static void*
YoyoGetArenaPointer(MemoryArena arena)
{
	void * result = (uint8_t*)arena.base + arena.used;
	return result;
}

inline void* YoyoPlatformAllocateMemory(size_t size)
{
	void* Result;
#if OSX
	Result = OSXAllocateMemory(size);
#elif WINDOWS
	Result = Win32AllocateMemory(size);
#elif IOS
	Result = IOSAllocateMemory(size);
#endif
	return Result;
}

inline void
YoyoPlatformDeallocateMemory(void* mem, size_t size)
{
#if OSX
	OSXDeAllocateMemory(mem, size);
#elif WINDOWS
	//TODO(Ray):Verify windows is really freeing this memory.
	Win32DeAllocateMemory(mem, size);
#elif IOS
	IOSDeAllocateMemory(mem, size);
#endif
}

inline size_t
YoyoGetAlignmentOffset(MemoryArena *arena, size_t alignment)
{
	size_t AlignmentOffset = 0;

	size_t ResultPointer = (size_t)arena->base + arena->used;
	size_t AlignmentMask = alignment - 1;
	if (ResultPointer & AlignmentMask)
	{
		AlignmentOffset = alignment - (ResultPointer & AlignmentMask);
	}

	return(AlignmentOffset);
}

struct duel_memory_partition
{
	MemoryArena FixedSized;
	MemoryArena VariableSized;
};

static void FreeDuelMemoryPartion(duel_memory_partition* Partition)
{
	YoyoPlatformDeallocateMemory(Partition->FixedSized.base, Partition->FixedSized.size);
	YoyoPlatformDeallocateMemory(Partition->VariableSized.base, Partition->VariableSized.size);
}

static void YoyoFreeMemoryArena(MemoryArena* arena)
{
	YoyoPlatformDeallocateMemory(arena->base, arena->size);
}

static MemoryArena AllocateMemoryArena(uint32_t size)
{
	MemoryArena result;
	void* base = YoyoPlatformAllocateMemory(size);
	result.base = base;
	result.size = size;
	result.used = 0;
	result.temp_count = 0;
	return result;
}

static duel_memory_partition AllocateDuelMemoryPartition(uint32_t Size)
{
	duel_memory_partition Result;
	Result.FixedSized = AllocateMemoryArena(Size);
	Result.VariableSized = AllocateMemoryArena(Size);
	return Result;
}

static void YoyoAllocateArena(MemoryArena *partition, size_t size, void* base)
{
	//Assert
	partition->base = base;
	partition->size = size;
	partition->used = 0;
	partition->temp_count = 0;
}

static MemoryArena YoyoAllocateArena(size_t size, void* base)
{
	MemoryArena result;
	//Assert
	result.base = base;
	result.size = size;
	result.used = 0;
	result.temp_count = 0;
	return result;
}

inline MemoryArena* YoyoPlatformAllocateArena(size_t size)
{
	size_t size_of_mp = sizeof(MemoryArena);
	size += size_of_mp;
	uint8_t* mem = (uint8_t*)YoyoPlatformAllocateMemory(size);
	MemoryArena* header = (MemoryArena *)mem;
	YoyoAllocateArena(header, size - size_of_mp, mem + size_of_mp);
	return header;
}

//TODO(ray):Fix this to clear more effeciently. or give option for clearing  method
static void YoyoClearToZero(MemoryArena *arena)
{
	for (uint32_t index = 0; index < arena->size; ++index)
	{
		uint8_t* byte = (uint8_t*)arena->base + index;
		*byte = 0;
	}
}

static void YoyoDeallocateArena(MemoryArena *arena, bool clean_mem_to_zero = true)
{
	//Assert
	arena->used = 0;
	if (clean_mem_to_zero)
	{
		YoyoClearToZero(arena);
	}
}

static TempMemory YoyoBeginTempMemory(MemoryArena *arena)
{
	TempMemory result;
	result.partition = arena;
	arena->temp_count++;
	result.used = arena->used;
	return result;
}

static void YoyoEndTempMemory(TempMemory temp_mem)
{
	temp_mem.partition->used = temp_mem.used;
	temp_mem.partition->temp_count--;
}

static void YoyoValidateTempMemory(TempMemory temp_mem)
{
	Assert(temp_mem.partition->temp_count < 1);
}

static bool YoyoTestFlags(uint32_t flag, uint32_t test_against)
{
	if ((flag & test_against) == test_against)
	{
		return true;
	}
	return false;
}

static void YoyoClearSize(MemoryArena *arena, size_t size)
{
	Assert(size > 0)
		
	size--;
	if (arena->used < size)
	{
		size = arena->used;
	}

	if (arena->used == 0)
	{
		return;
	}
	else
	{
		uint8_t* byte = (uint8_t*)arena->base + arena->used;
		while (size--)
		{
			*byte++ = 0;
		}
	}
}

#define YoyoZeroStruct(Instance) YoyoClearToZero(&(Instance),sizeof(Instance))
#define YoyoZeroArray(Count, Pointer) YoyoClearToZero(Pointer,Count*sizeof((Pointer)[0]))
static void YoyoClearToZero(void* Mem, uint32_t Size)
{
	Assert(Size > 0)
		uint8_t* Byte = (uint8_t*)Mem;
	while (Size--)
	{
		*Byte++ = 0;
	}
}

//TODO(ray):Add memory alignment options here!!

#define YoyoPushArray(Partition,Type,Count,...) (Type*)YoyoPushSize_(Partition,sizeof(Type)*Count,## __VA_ARGS__)
#define YoyoPushStruct(Partition,Type,...) (Type*)YoyoPushSize_(Partition,sizeof(Type),## __VA_ARGS__)
#define YoyoPushSize(Partition,Size,...) YoyoPushSize_(Partition,Size,## __VA_ARGS__)
static void* YoyoPushSize_(MemoryArena*Partition, size_t Size, ParitionPushParams PushParams = DefaultPartitionParams())
{
	//Assert Used < Size
	Assert(Partition->used + Size <= Partition->size)

		void * Result = (uint8_t*)Partition->base + Partition->used;
	if (YoyoTestFlags(PushParams.flags, YoyoPartitionFlag_ClearToZero))
	{
		YoyoClearSize(Partition, Size);
	}

	Partition->used = Partition->used + Size;

	return Result;
}

inline ParitionPushParams
NoClear()
{
	ParitionPushParams Params = DefaultPartitionParams();
	Params.flags &= ~YoyoPartitionFlag_ClearToZero;
	return(Params);
}

inline char *
PushCharString(MemoryArena *Partition, char *CharString)
{
	uint32_t Size = 1;
	for (char *At = CharString;
		*At;
		++At)
	{
		++Size;
	}

	char *Dest = (char *)YoyoPushSize_(Partition, Size, NoClear());
	for (uint32_t CharIndex = 0;
		CharIndex < Size;
		++CharIndex)
	{
		Dest[CharIndex] = CharString[CharIndex];
	}

	return(Dest);
}


