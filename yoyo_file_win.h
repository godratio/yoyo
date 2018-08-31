#pragma once
#include "yoyoyo_ascii_vector.h"
#include "yoyoyo_ascii_string.h"
#include <Windows.h>
#include "yoyo_file.h"
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

static YoyoAString* YoyoBuildPathToAssets(MemoryArena* arena, u32 type)
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
	u32 size = GetCurrentDirectory(0, NULL);
	PushSize(arena, size);
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
