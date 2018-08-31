#pragma once

#include <mach/mach_init.h>
#include <mach/mach_vm.h>

static void*
OSXAllocateMemory(size_t Size)
{
    mach_vm_address_t address;
    kern_return_t kr;
    mach_vm_size_t size = (mach_vm_size_t)Size;
    kr = mach_vm_allocate(mach_task_self(), &address, size, VM_FLAGS_ANYWHERE);
    //TODO(ray):Make sure this actually casts properly.
    return (void*)address;
}

static void OSXDeAllocateMemory(void* Memory,size_t Size)
{
    mach_vm_address_t Address = (mach_vm_address_t)Memory;
    //mach_vm_size_t MachSize = (mach_vm_size_t)Size;
    mach_vm_deallocate(mach_task_self(), Address, Size);
}
 
