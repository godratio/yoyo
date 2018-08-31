#pragma once
#include "yoyoyo_ascii_string.h"
#include "yoyoyo_ascii_vector.h"
#include "api/api_memory.h"


struct YoyoReadFileResult
{
	s32 content_size;
	void* content;
};

struct YoyoDirFilesResult
{
	YoyoVector files;//files infos
	~YoyoDirFilesResult()
	{
		YoyoFreeVectorMem(&files);
	}
};

struct YoyoFileInfo
{
	void* file;
	YoyoAString* name;
	memory_index file_size;
	u32 file_count;
};


#if WINDOWS
#include "yoyo_file_win.h"
#elif OSX
#elif IOS
#endif