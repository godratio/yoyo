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
};

#define YOYO_DIAGNOSTICS_H
#endif
