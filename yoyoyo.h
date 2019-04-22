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

#include "yoyo_threads.h"

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

    bool log_output = true;
    bool yoyo_diag_log_to_console = true;
    bool log_own_allocations = false;

    YoyoVector allocations_table;
    const char* this_file_name = __FILE__;
    memory_index frame_index = 0;

    YoyoMemStats cpu_mem_stats;
    YoyoMemStats gpu_mem_stats;

    YoyoSysMemInfo cpu_sys_mem_info;
    YoyoSysMemInfo gpu_sys_mem_info;

    memory_index est_memory_usage;
    memory_index est_gpu_memory_usage;
    
    void Init()
    {
        allocations_table = YoyoInitVector(10,YoyoMemDiagAllocEntry,false);
        cpu_mem_stats = {};
        gpu_mem_stats = {};
        cpu_sys_mem_info = {};
        gpu_sys_mem_info = {};
        est_memory_usage = 0;
        est_gpu_memory_usage = 0;
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

    //NOTE(Ray):All GPU allocated stats are just what we request from the OS but may not actually be used so may differ
    //than reported by the os.  We are just tracking what we request for now.
    void AllocEntry_(memory_index size,char* file_name,uint32_t line_no)
    {
        if(IsOwnAllocation(file_name))
        {
            cpu_mem_stats.total_allocs += 1;
            cpu_mem_stats.total_mem_alloced += size;
            est_memory_usage += size;            
            PlatformOutput(yoyo_diag_log_to_console,"mem alloc: file name: %s : line no : %d \n",file_name,line_no);            
        }
    }
    
    void DeallocEntry_(memory_index size,char* file_name,uint32_t line_no)
    {
        if(IsOwnAllocation(file_name))
        {
            cpu_mem_stats.total_deallocs += 1;
            cpu_mem_stats.total_mem_dealloced += size;
            est_memory_usage -= size;
            PlatformOutput(yoyo_diag_log_to_console,"mem dealloc file name: %s : line no : %d \n",file_name,line_no);
        }
    }

//Buffers and textures only at the moment howeer that is not very comprehensive
    void GPUAllocEntry_(memory_index size,char* file_name,uint32_t line_no)
    {
        if(IsOwnAllocation(file_name))
        {
            gpu_mem_stats.total_allocs += 1;
            gpu_mem_stats.total_mem_alloced += size;
            est_gpu_memory_usage += size;            
            PlatformOutput(yoyo_diag_log_to_console,"gpu mem alloc: file name: %s : line no : %d \n",file_name,line_no);
        }        
    }

    void GPUDeallocEntry_(memory_index size,char* file_name,uint32_t line_no)
    {
        if(IsOwnAllocation(file_name))
        {
            gpu_mem_stats.total_deallocs += 1;
            gpu_mem_stats.total_mem_dealloced += size;
            est_gpu_memory_usage -= size;
            PlatformOutput(yoyo_diag_log_to_console,"gpu mem dealloc: file name: %s : line no : %d \n",file_name,line_no);            
        }        
    }

    void OuputReport()
    {
        PlatformOutput(&log_output,"---YOYO_MEM_DIAG-------\n");

        PlatformOutput(&log_output,"---CPUStats------------\n");
        
        PlatformOutput(&log_output,"---total_allocs:%d------\n",cpu_mem_stats.total_allocs);
        PlatformOutput(&log_output,"---total_mem_alloced:%d-\n",cpu_mem_stats.total_mem_alloced / MegaBytes(1));
        PlatformOutput(&log_output,"---total_deallocs:%d----\n",cpu_mem_stats.total_deallocs);        
        PlatformOutput(&log_output,"---total_mem_dealloced:%d-\n",cpu_mem_stats.total_mem_dealloced);
        PlatformOutput(&log_output,"---est_memory_usage:%d-\n",est_memory_usage);
        PlatformOutput(&log_output,"---END_CPUStats--------\n");
        
        PlatformOutput(&log_output,"---GPUStats------------\n");
        
        PlatformOutput(&log_output,"---total_allocs:%d------\n",gpu_mem_stats.total_allocs);
        PlatformOutput(&log_output,"---total_mem_alloced:%d-\n",gpu_mem_stats.total_mem_alloced / MegaBytes(1));
        PlatformOutput(&log_output,"---total_deallocs:%d----\n",gpu_mem_stats.total_deallocs);        
        PlatformOutput(&log_output,"---total_mem_dealloced:%d-\n",gpu_mem_stats.total_mem_dealloced);
        PlatformOutput(&log_output,"---est_gpu_memory_usage:%d-\n",est_gpu_memory_usage);
        
        PlatformOutput(&log_output,"---ENDGPUStats---------\n");
        PlatformOutput(&log_output,"---END_YOYO_MEM_DIAG---\n");
    }
};
#endif

#define YOYOYO_H
#endif
