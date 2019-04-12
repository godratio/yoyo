#if !defined(YOYO_DIAGNOSTICS_H)

struct YoyoMemDiagLoc
{
    char* file_name;
    uint32_t line_no;
};

struct YoyoMemDiagAllocEntry
{
    YoyoMemDiagLoc loc;
    memory_index size;
    memory_index frame_index;
    uint32_t thread_index;
};

struct YoyoSysMemInfo
{
    memory_index est_total_sys_mem;
    memory_index est_sys_avail;
};

struct YoyoMemStats
{
    memory_index total_allocs;
    memory_index total_deallocs;
    memory_index total_mem_alloced;
    memory_index total_mem_dealloced;
    uint32_t memory_pressure_warnings_issued;//mobile
};

namespace YoyoMemoryDiagnostics
{
    void Init();

#define AllocEntry(size) YoyoMemoryDiagnostics::AllocEntry_(size,__FILE__,__LINE__);
    void AllocEntry_(memory_index size,char* file_name,uint32_t line_no);
    
#define DeallocEntry(size) YoyoMemoryDiagnostics::AllocEntry_(size,__FILE__,__LINE__);
    void DeallocEntry_(memory_index size,char* file_name,uint32_t line_no);

    //Buffers and textures only at the moment howeer that is not very comprehensive just quick and dirty for now
//Represents the memory we actually we have requested from the gpu
#define GPUAllocTextureEntry(size) YoyoMemoryDiagnostics::AllocEntry_(size,__FILE__,__LINE__);
    void GPUAllocTextureEntry_(memory_index size,char* file_name,uint32_t line_no);
#define GPUDeallocTextureEntry(size) YoyoMemoryDiagnostics::AllocEntry_(size,__FILE__,__LINE__);
    void GPUDeallocTexutreEntry_(memory_index size,char* file_name,uint32_t line_no);
#define GPUAllocBufferEntry(size) YoyoMemoryDiagnostics::AllocEntry_(size,__FILE__,__LINE__);
    void GPUAllocBufferEntry_(memory_index size,char* file_name,uint32_t line_no);
#define GPUDeallocBufferEntry(size) YoyoMemoryDiagnostics::AllocEntry_(size,__FILE__,__LINE__);
    void GPUDeallocBufferEntry_(memory_index size,char* file_name,uint32_t line_no);
    
//When using GPU heap resources
#define GPUAllocEntry(size) YoyoMemoryDiagnostics::AllocEntry_(size,__FILE__,__LINE__);
    void GPUAllocEntry_(memory_index size,char* file_name,uint32_t line_no);
        
#define GPUDeallocEntry(size) YoyoMemoryDiagnostics::AllocEntry_(size,__FILE__,__LINE__);
    void GPUDeallocEntry_(memory_index size,char* file_name,uint32_t line_no);

//TODO(Ray):Track vectors and their allocations
//TODO(Ray):Send collected frame data via tcp to remote machine for logging.
//TODO(Ray):Alternatively allow for some output to the console and / or via an overlay if no
//remote machine is available.
    
    void OuputReport();
};

#define YOYO_DIAGNOSTICS_H
#endif
