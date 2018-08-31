#pragma once
#include "yoyoyo_ascii_vector.h"
#include "yoyoyo_ascii_string.h"
#include <Windows.h>
#include "yoyo_file.h"
#include "yoyoyo_mem_win.h"
static char* DataDir = "/../../data/";

enum YoyoDirectoryType
{
	YoyoDirectory_None,
	YoyoDirectory_Models,
	YoyoDirectory_Materials,
	YoyoDirectory_Shaders,
	YoyoDirectory_Textures,
	YoyoDirectory_Sounds,
	YoyoDirectory_Fonts,
	YoyoDirectory_Lighting
};

static YoyoAString* YoyoBuildPathToAssets(MemoryArena* arena, uint32_t type)
{
	YoyoAString* data_path = YoyoAsciiStringFromChar(DataDir, arena);
	YoyoAString* final_path;
	if (type == 0)
	{
		final_path = YoyoAsciiAppendString(*data_path, *YoyoAsciiStringFromChar("/", arena), arena);
	}
	else if (type == 1)
	{
		final_path = YoyoAsciiAppendString(*data_path, *YoyoAsciiStringFromChar("material/", arena), arena);
	}
	else if (type == 2)
	{
		final_path = YoyoAsciiAppendString(*data_path, *YoyoAsciiStringFromChar("shaders/", arena), arena);
	}
	else if (type == 3)
	{
		final_path = YoyoAsciiAppendString(*data_path, *YoyoAsciiStringFromChar("textures/", arena), arena);
	}
	else if (type == 4)
	{
		final_path = YoyoAsciiAppendString(*data_path, *YoyoAsciiStringFromChar("models/", arena), arena);
	}
	else if (type == 5)
	{
		final_path = YoyoAsciiAppendString(*data_path, *YoyoAsciiStringFromChar("textures/", arena), arena);
	}
	else if (type == 6)
	{
		final_path = YoyoAsciiAppendString(*data_path, *YoyoAsciiStringFromChar("lighting/", arena), arena);
	}

	YoyoAString* current_dir = YoyoAsciiAllocatEmptyString(arena);
	uint32_t size = GetCurrentDirectory(0, NULL);
	YoyoPushSize(arena, size);
	GetCurrentDirectory(size, current_dir->string);
	current_dir->length = size;
	{
		//TODO(ray):Some error handling.
	}
	return YoyoAsciiAppendString(*current_dir, *final_path, arena);;
}

static YoyoDirFilesResult
YoyoWin32GetAllFilesInDir(YoyoAString path, MemoryArena* string_mem)
{
	YoyoDirFilesResult result;

	char* wild_card = "\\*";
	YoyoAString* wild_card_path = YoyoAsciiAppendString(path, *YoyoAsciiStringFromChar(wild_card, string_mem), string_mem);

	result.files = YoyoInitVector(1000, sizeof(YoyoFileInfo),false);
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	LARGE_INTEGER filesize;

	hFind = FindFirstFile(wild_card_path->string, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		return result;
	}

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;

			YoyoFileInfo info;
			info.name = YoyoAsciiStringFromChar(ffd.cFileName, string_mem);// ffd.cFileName;
			YoyoPushBack(&result.files, info);
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	return result;
}

static YoyoReadFileResult
YoyoWin32ReadEntireFile(char* path)
{
	//Assert(Path);
	YoyoReadFileResult result = {};

	HANDLE file = CreateFileA(
		path,
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
	);

	if (file != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER file_size;
		if (GetFileSizeEx(
			file,
			&file_size
		))
		{
			DWORD size_result;
			result.content = VirtualAlloc(
				0,
				file_size.QuadPart,
				MEM_COMMIT | MEM_RESERVE,
				PAGE_READWRITE
			);
			if (result.content)
			{
				if (ReadFile(
					file,
					result.content,
					file_size.QuadPart,
					&size_result,
					0
				))
				{
					result.content_size = file_size.QuadPart;
				}
				else
				{
					//TODO(ray):logging
				}
			}
			else
			{
				//TODO(ray):logging
				Win32DeAllocateMemory(result.content, result.content_size);
			}
		}
		else
		{
			//TODO(ray):logging
		}
	}
	else
	{
		//TODO(ray):logging
		int ErrorCode = GetLastError();
		OutputDebugString("INVALIDHANDLE");
	}
	CloseHandle(file);
	return result;
}

static  bool YoyoWin32WriteToFile(FILE* file, void* mem, size_t size, bool is_done = false)
{
	bool result = false;
	fwrite(mem, size, 1, file);
	if (ferror(file))
	{
		result = false;
	}
	else
	{
		result = true;
	}

	if (is_done)
	{
		fclose(file);
	}
	return result;
}

struct YoyoPlatformFilePointer
{
#if WINDOWS 
	//HANDLE file;
	FILE* file;
#elif IOS || OSX
	FILE file;
#endif
};

static bool YoyoPlatformWriteMemoryToFile(YoyoPlatformFilePointer* file, char* file_name, void* mem, size_t size, bool is_done = false)
{
	if (file->file == nullptr)
	{
		file->file = fopen(file_name, "wb");
	}
	return YoyoWin32WriteToFile(file->file, mem, size, is_done);
}

static YoyoReadFileResult YoyoPlatformReadEntireFile(char* FileName)
{
	YoyoReadFileResult result;
	result = YoyoWin32ReadEntireFile(FileName);
	return result;
}

static YoyoReadFileResult YoyoPlatformReadEntireFile(YoyoAString* FileName)
{
	YoyoReadFileResult result;
	result = YoyoWin32ReadEntireFile(YoyoAsciiNullTerminate(*FileName).string);
	return result;
}

static YoyoReadFileResult YoyoPlatformReadEntireFileFromAssets(char* FileName, uint32_t Type, MemoryArena* Memory)
{
	YoyoReadFileResult result;
	YoyoAString* asset_path = YoyoBuildPathToAssets(Memory, Type);
	YoyoAString* final_path_to_asset = YoyoAsciiAppendString(*asset_path, *YoyoAsciiStringFromChar(FileName, Memory), Memory);
	*final_path_to_asset = YoyoAsciiNullTerminate(*final_path_to_asset);
	result = YoyoWin32ReadEntireFile(final_path_to_asset->string);
	return result;
}

static YoyoReadFileResult YoyoPlatformReadEntireFileFromAssets(YoyoAString* FileName, uint32_t Type, MemoryArena* Memory)
{
	YoyoReadFileResult result;
	YoyoAString* asset_path = YoyoBuildPathToAssets(Memory, Type);
	YoyoAString* final_path_to_asset = YoyoAsciiAppendString(*asset_path, *YoyoAsciiStringFromChar(FileName->string, Memory), Memory);
	*final_path_to_asset = YoyoAsciiNullTerminate(*final_path_to_asset);
	result = YoyoWin32ReadEntireFile(final_path_to_asset->string);
	return result;
}

static YoyoDirFilesResult YoyoPlatformGetAllFilesInDir(YoyoAString Path, MemoryArena* StringMem)
{
	YoyoDirFilesResult result;
	result = YoyoWin32GetAllFilesInDir(Path, StringMem);
	return result;
}

static YoyoDirFilesResult YoyoPlatformGetAllAssetFilesInDir(uint32_t Type, MemoryArena* StringMem)
{
	YoyoDirFilesResult result;
	YoyoAString* path = YoyoBuildPathToAssets(StringMem, Type);
	result = YoyoWin32GetAllFilesInDir(*path, StringMem);
	return result;
}

