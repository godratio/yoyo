#if 0

#pragma once
#include <Windows.h>

static void*
Win32AllocateMemory(size_t size)
{
	// TODO(ray): Verify the type we are passing in make sure
	//we are getting the proper page size back.
	void* mem = VirtualAlloc(
		0,
		size,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE
	);
	if (!mem)
	{
		// TODO(ray): Some error handling because we couldnt get the memory we need.
	}
	return mem;
}

static void
Win32DeAllocateMemory(void* mem, size_t size)
{
	VirtualFree(
		mem,
		size,
		MEM_RELEASE
	);
}

#endif