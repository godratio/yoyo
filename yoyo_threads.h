#if !defined(YOYO_THREADS_H)
#ifdef YOYOIMPL

#if IOS || OSX
#include <pthread.h>
#elif WINDOWS
#endif

memory_index YoyoGetThreadID()
{
#if IOS || OSX
    mach_port_t threadport = pthread_mach_thread_np(pthread_self());
     return (memory_index)threadport;
#elif WINDOWS
#endif
}
#endif
#define YOYO_THREADS_H
#endif
