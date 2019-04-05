#if !defined(YOYOYO_H)

#define YOYODIAG

#if WINDOWS
#define Assert(Expression) if(!(Expression)){*(int *)0 = 0;};
#else
#define Assert(Expression) if(!(Expression)){__builtin_trap();};
#endif

#include <stdint.h>

#ifdef OSX
typedef __darwin_size_t memory_index;
#else
typedef intptr_t memory_index;
#endif

#include "yoyoyo_math.h"

#include "yoyo_diagnostics.h"
//TODO(Ray):Move this dependence from here to yoyomemory
#include "../api/api_include.h"

#include "meow_hash_yoyo/meow_intrinsics.h"
#include "meow_hash_yoyo/meow_hash.h"

#define ArrayCount(Array) sizeof(Array) / sizeof((Array)[0])

#define KiloBytes(Value) (Value * 1024LL)
#define MegaBytes(Value) (KiloBytes(Value) * 1024LL)
#define GigaBytes(Value) (MegaBytes(Value) * 1024LL)
#define TeraBytes(Value) (GigaBytes(Value) * 1024LL)

#include "yoyoyo_ascii_vector.h"
#include "yoyoyo_hashtable.h"

#include "yoyoyo_math_ext.h"
#include "yoyoyo_scene.h"

//NOTE(Ray):A very simple memory diagnostics tool to track allocations and deallocations in yoyo api and metalizer.
#ifdef YOYOIMPL
namespace YoyoMemoryDiagnostics
{
    bool yoyo_diag_log_to_console = true;
    bool log_own_allocations = false;
    
    YoyoVector allocations_table;
    const char* this_file_name = __FILE__;
    memory_index frame_index = 0;

    YoyoMemStats cpu_mem_stats;
    YoyoMemStats gpu_mem_stats;

    YoyoSysMemInfo cpu_sys_mem_info;
    YoyoSysMemInfo gpu_sys_mem_info;
    
    void Init()
    {
        allocations_table = YoyoInitVector(10,YoyoMemDiagAllocEntry,false);
    }

    bool IsOwnAllocation(char* file_name)
    {
        if(strcmp(this_file_name,file_name))
        {
            return true;
        }
        return false;
    }

    void BeginFrame()
    {
    }
    
    void EndFrame()
    {
        ++frame_index;        
    }
    
    void AllocEntry_(memory_index size,char* file_name,uint32_t line_no)
    {

        if(IsOwnAllocation(file_name))
        {
            PlatformOutput(yoyo_diag_log_to_console,"file name: %s : line no : %d \n",file_name,line_no);            
        }
    }
};
#endif

#define YOYOYO_H
#endif
