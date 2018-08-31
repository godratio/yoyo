#pragma once
#include "yoyoyo_ascii_string.h"
#include "yoyoyo_ascii_vector.h"
#include "yoyoyo_memory.h"
struct YoyoReadFileResult
{
	int content_size;
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
	size_t file_size;
	uint32_t file_count;
};


#if WINDOWS
#include "yoyo_file_win.h"
#elif OSX
#elif IOS
#endif