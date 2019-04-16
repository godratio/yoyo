#if !defined(YOYO_THREADS_H)

#if OSX
#include <x86intrin.h>
#elif WINDOWS
#include <intrin.h>
#elif IOS
#include <arm_neon.h>
#endif

struct TicketMutex
{
    uint64_t volatile ticket;
    uint64_t volatile serving;
};

//NOTE(Ray):There is a declspec on the impl path so that in MSVC all functiosn are properly exported
#if WINDOWS //MSVC
#if YOYOIMPL
#define YOYO_ATOMICS_INLINE //__declspec(dllexport) __forceinline
#else

#define YOYO_ATOMICS_INLINE //__forceinline
#endif

#else //XCODE CLANG
#define YOYO_ATOMICS_INLINE // __attribute__((always_inline))
#endif

uint64_t YoyoCreateThread(void*(*func)(void*),void* func_data);
YOYO_ATOMICS_INLINE uint64_t AtomicCompareExchange64(uint64_t volatile *value,uint64_t new_value,uint64_t expected_value);
YOYO_ATOMICS_INLINE uint64_t AtomicIncrement64(uint64_t volatile *value);
YOYO_ATOMICS_INLINE uint64_t AtomicDecrement64(uint64_t volatile *value);
YOYO_ATOMICS_INLINE uint64_t AtomicAdd64(uint64_t volatile *value,uint64_t amount);
YOYO_ATOMICS_INLINE void BeginTicketMutex(TicketMutex* mutex);
YOYO_ATOMICS_INLINE void EndTicketMutex(TicketMutex* mutex);

#ifdef YOYOIMPL
#if IOS || OSX

#include <pthread.h>
#include <libkern/OSAtomic.h>

#elif WINDOWS
#endif

memory_index YoyoGetThreadID()
{
#if IOS || OSX
    mach_port_t threadport = pthread_mach_thread_np(pthread_self());
     return (memory_index)threadport;
#elif WINDOWS
     //NOTE(Ray):Use our two instruction trick.
#endif
}

//void *(*start_routine) (void *),
uint64_t YoyoCreateThread(void*(*func)(void*),void* func_data)
{
    uint64_t result;//thread id
#if IOS || OSX
    pthread_t tID;
    int tErr;
    tErr = pthread_create( &tID , NULL, func, func_data);
    result = (uint64_t)tID;
#elif WINDOWS
//CreateThread
#endif
    return result;
}

//TODO(Ray):WriteBarrier

//TODO(Ray):ReadBarrier

YOYO_ATOMICS_INLINE uint64_t AtomicCompareExchange64(uint64_t volatile *value,uint64_t new_value,uint64_t expected_value)
{
    uint64_t result;
#if IOS || OSX
//    result = *value;
    //TODO(Ray):Find a way to guarentee to return the result of the old value or we just return bool.
    //We need to pick to be consistent with either the windows or osx/ios apil
    result = __sync_val_compare_and_swap((int64_t* volatile)value,(int64_t)expected_value,(int64_t)new_value);
    //OSAtomicCompareAndSwap64(expected_value, new_value, value);
    
#elif WINDOWS
//    __InterlockedCompareExchange
#endif
    return result;
}

YOYO_ATOMICS_INLINE uint64_t AtomicIncrement64(uint64_t volatile *value)
{
    uint64_t result;
#if IOS || OSX
//__sync
//    result = OSAtomicIncrement64((int64_t* volatile)value);
    result = __sync_fetch_and_add((int64_t* volatile)value,(int64_t)1);
#elif WINDOWS
#endif
    return result;
}

YOYO_ATOMICS_INLINE uint64_t AtomicDecrement64(uint64_t volatile *value)
{
    uint64_t result;
#if IOS || OSX
    result = __sync_fetch_and_sub((int64_t* volatile)value,(int64_t)1);    
//    result = OSAtomicDecrement64((int64_t* volatile)value);
#elif WINDOWS
#endif
    return result;
}

YOYO_ATOMICS_INLINE uint64_t AtomicAdd64(uint64_t volatile *value,uint64_t amount)
{
    uint64_t result;
#if IOS || OSX
    result = __sync_fetch_and_add((int64_t* volatile)value,(int64_t)amount);
//    result = OSAtomicAdd64(amount,(int64_t* volatile)value);
#elif WINDOWS
#endif
    return result;
}

YOYO_ATOMICS_INLINE void BeginTicketMutex(TicketMutex* mutex)
{
    int64_t ticket = AtomicAdd64(&mutex->ticket,1);
    #if IOS
//    while(ticket != mutex->serving){__yield();};
    while(ticket != mutex->serving){__asm__ __volatile__("yield");};
    #else
    while(ticket != mutex->serving){_mm_pause();};
    #endif
}

YOYO_ATOMICS_INLINE void EndTicketMutex(TicketMutex* mutex)
{
    AtomicAdd64(&mutex->serving,1);
}

#endif
#define YOYO_THREADS_H
#endif
