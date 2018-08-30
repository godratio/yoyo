#pragma once

#define MAX_FILENAME_LENGTH 50
#define MAX_FILE_EXTENSION_LENGTH 10

#include <stdint.h>
#include <stdio.h>
#include "./api/api_include.h"

#ifdef __cplusplus
#define APIDEF extern "C"
#else
#define APIDEF extern
#endif

struct YoyoAString
{
    bool null_terminated;
    size_t length;
    char* string;
};

//NOTE(ray):Assumes string is already null terminated.
APIDEF size_t YoyoAsciiStringLength(YoyoAString* s)
{
    size_t length = 0;
    char* at = s->string;
    while(*at)
    {
        length++;
        at++;
    }
    s->length = length;
    return length;
}

APIDEF size_t YoyoAsciiStringLengthSafe(YoyoAString* s,size_t safe_length)
{
    size_t length = 0;
    char* at = s->string;
    while(*at)
    {
        length++;
        at++;
        if(length > safe_length)
        {
            break;
        }
    }
    s->length = length;
    return length;
}

//NOTE(ray):Assumes string is already null terminated.
APIDEF size_t YoyoAsciiGetLengthChar(char* s)
{
    size_t length = 0;
    char* at = s;
    while(*at)
    {
        length++;
        at++;
    }
    return length;
}

//TODO(Ray):Make a way to reclaim the memory from literals created here.
//TODO(Ray):Allow to have the option to do the length check safely.
//NOTE(Ray):This function requires you free your own memory once your done.
//Put here or convience not reccomended atm.
static YoyoAString* YoyoAsciiStringAllocate(char* s)
{
	size_t length = YoyoAsciiGetLengthChar(s);
	void* mem = malloc(sizeof(YoyoAString) + length);//PlatformAllocateMemory(sizeof(string) + Length);
	YoyoAString* result = (YoyoAString*)mem;
	result->length = 0;
	char* at = s;
	char* start_ptr = (char*)mem + sizeof(YoyoAString);
	char* string_ptr = (char*)mem + sizeof(YoyoAString);
	while (*at)
	{
		*string_ptr = *at;
		string_ptr++;
		at++;
	}
	result->string = start_ptr;
	result->length = length;
	return result;
}

APIDEF size_t YoyoAsciiGetLengthCharSafely(char* s,size_t safe_length)
{
    size_t length = 0;
    char* at = s;
    while(*at)
    {
        length++;
        at++;
        if(length > safe_length)
        {
            break;
        }
    }
    return length;
}

APIDEF YoyoAString YoyoAsciiNullTerminate(YoyoAString s)
{
    char* null_terminate_point = s.string + s.length;
    *null_terminate_point = '\0';
    s.null_terminated = true;
    return s;
}

//TODO(ray):Make a way to reclaim the memory
APIDEF YoyoAString* YoyoAsciiStringFromChar(char* s, MemoryArena* mem)
{
	YoyoAString* result = (YoyoAString*)PushSize(mem, sizeof(YoyoAString));
	result->length = 0;
	char* at = s;
	void* start_ptr = GetPartitionPointer(*mem);
	while (*at)
	{
		char * string_ptr = (char*)PushSize(mem, 1);
		*string_ptr = *at;
		result->length++;
		at++;
	}
	result->string = (char*)start_ptr;
	return result;
}

static YoyoAString* YoyoAsciiCreateStringRangedChar(char* s, char* e, MemoryArena* mem)
{
	YoyoAString* result = (YoyoAString*)PushSize(mem, sizeof(YoyoAString));
	char* at = s;
	void* start_ptr = GetPartitionPointer(*mem);
	char* string_ptr = 0;//(char*)Memory;
	while (*at != *e)
	{
		string_ptr = (char*)PushSize(mem, 1);
		*string_ptr = *at;
		result->length++;
		at++;
	}
	result->string = (char*)start_ptr;
	return result;
}

APIDEF YoyoAString* YoyoAsciiStringFromCharLength(char* s, uint32_t end_length, MemoryArena* mem)
{
	YoyoAString* result = (YoyoAString*)PushSize(mem, sizeof(YoyoAString));

	char* at = s;
	void* start_ptr = GetPartitionPointer(*mem);
	char* string_ptr = 0;//(char*)Memory;
	uint32_t iterator = 0;
	while (iterator < end_length)
	{
		string_ptr = (char*)PushSize(mem, 1);
		*string_ptr = *at;
		result->length++;
		at++;
		iterator++;
	}
	result->string = (char*)start_ptr;
	return result;
}

APIDEF YoyoAString* YoyoCreateStringRangedPointer(char* s, char* e, MemoryArena* mem)
{
	YoyoAString* result = (YoyoAString*)PushSize(mem, sizeof(YoyoAString));

	char* at = s;
	void* start_ptr = GetPartitionPointer(*mem);
	char* string_ptr = 0;//(char*)Memory;
	while (at != e)
	{
		string_ptr = (char*)PushSize(mem, 1);
		*string_ptr = *at;
		result->length++;
		at++;
	}
	result->string = (char*)start_ptr;
	return result;
}

static YoyoAString* YoyoAsciiAllocatEmptyString(MemoryArena* mem)
{
	Assert(mem);
	return YoyoAsciiStringFromChar("", mem);
}


APIDEF int YoyoAsciiStringCompare(YoyoAString a, YoyoAString b)
{
	if (a.null_terminated && b.null_terminated)
	{
		char* a_ptr = a.string;
		char* b_ptr = b.string;
		while (*a_ptr && *b_ptr)
		{
			if (*a_ptr != *b_ptr)
				return false;

			a_ptr++; b_ptr++;
		}
	}
	else
	{
		char* a_ptr = a.string;
		char* b_ptr = b.string;

		size_t max_iterations = (a.length > b.length) ? a.length : b.length;
		for (uint32_t index = 0; index < max_iterations; ++index)
		{
			if (*a_ptr != *b_ptr)
				return false;

			a_ptr++; b_ptr++;
		}
	}
	return true;
}

APIDEF int YoyoAsciiStringCompareToChar(YoyoAString a, char* b)
{
	char* a_ptr = a.string;
	char* b_ptr = b;

	size_t max_iterations = a.length;
	for (uint32_t index = 0; index < max_iterations; ++index)
	{
		//TODO(Ray):Need to check logic here for string where B is longer than A
		if (*a_ptr != *b_ptr)
			return false;
		a_ptr++; b_ptr++;
	}
	return true;
}

APIDEF int YoyoAsciiCharCompareToChar(char* a, char* b, uint32_t max_iterations)
{
	char* a_ptr = a;
	char* b_ptr = b;

	for (uint32_t index = 0; index < max_iterations; ++index)
	{
		if (*a_ptr == '\0' || *b_ptr == '\0')break;
		if (*a_ptr != *b_ptr)
			return false;
		a_ptr++; b_ptr++;
	}
	return true;
}


APIDEF YoyoAString* YoyoAsciiGetFileExtension(YoyoAString* file_name_or_path, MemoryArena *string_mem, b32 keep_extension_delimeter = false)
{
	Assert(file_name_or_path->length > 1)
		//walk back from end of string till we hit a '.'
		char* end = file_name_or_path->string + file_name_or_path->length - 1;
	uint32_t look_back = 1;
	if (keep_extension_delimeter)
	{
		look_back = 0;
	}
	uint32_t steps_taken = 1;
	while (*(end - look_back) != '.')
	{
		--end;
		++steps_taken;
		if (steps_taken > MAX_FILE_EXTENSION_LENGTH)
		{
			//TODO(ray):Log this as an error?
			break;
		}
	}
	YoyoAString* extension_name = YoyoAsciiStringFromCharLength(end, steps_taken, string_mem);
	return extension_name;
}

APIDEF YoyoAString* YoyoAsciiStripExtension(YoyoAString* file_name_or_path, MemoryArena *mem)
{
	Assert(file_name_or_path->length > 1);
	//walk back from end of string till we hit a '.'
	char* end = file_name_or_path->string + file_name_or_path->length - 1;
	uint32_t step_count = 1;
	while (*end != '.')
	{
		--end;
		if (step_count > MAX_FILENAME_LENGTH)
		{
			//TODO(ray):Log this as an error?
			break;
		}
	}
	return YoyoAsciiCreateStringRangedChar(&file_name_or_path->string[0], &end[0], mem);
}


APIDEF YoyoAString* YoyoAsciiStripAndOutputExtension(YoyoAString* file_name_or_path, YoyoAString* ext, MemoryArena *mem, b32 keep_delimeter = false)
{
	Assert(file_name_or_path->length > 1)

		YoyoAString* result = YoyoAsciiStripExtension(file_name_or_path, mem);
	YoyoAString* extension_name = YoyoAsciiGetFileExtension(file_name_or_path, mem, keep_delimeter);
	//string TerminatedExtensionName = NullTerminate(*ExtensionName);
	*ext = *extension_name;
	return result;
}

APIDEF YoyoAString* YoyoAsciiPadRight(YoyoAString* s, char pad_char, size_t pad_amount, MemoryArena* mem)
{
	//TODO(RAY):LENGTH IS WRONG
	YoyoAString* result = PushStruct(mem, YoyoAString);
	result->string = (char*)PushSize(mem, (size_t)(s->length + pad_amount));
	result->length = pad_amount + s->length;
	//    char* At = Result->String;
	char* source_string = s->string;
	//    while(*At++)
	for (uint32_t string_index = 0; string_index < result->length; ++string_index)
	{
		char* at = result->string + string_index;
		if (string_index > s->length - 1)
		{
			*at = pad_char;
			if (string_index <= (s->length + pad_amount))
			{
				at++;
				*at = '\0';
			}
		}
		else
		{
			*at = *source_string++;
		}
	}
	return result;
}

APIDEF YoyoAString* YoyoAsciiEnforceMinSize(YoyoAString* s, size_t min_size, MemoryArena* mem)
{
	if (s->length < min_size)
	{
		size_t Diff = min_size - s->length;
		s = YoyoAsciiPadRight(s, ' ', Diff, mem);
	}
	else if (s->length > min_size)
	{
		uint32_t count = 0;
		char* at = s->string;

		while (*at++)
		{
			if (count < min_size + 2)
			{
				s->length = min_size;
				*at = '\0';
			}
			count++;
		}
	}
	return s;
}

#define YoyoAsciiAppendStringToChar(front,back,mem) YoyoAsciiAppendString(*YoyoAsciiStringFromChar(front,mem),back,mem)
#define YoyoAsciiAppendCharToString(front,back,mem) YoyoAsciiAppendString(front,*YoyoAsciiStringFromChar(back,mem),mem)

APIDEF YoyoAString* YoyoAsciiAppendString(YoyoAString front, YoyoAString back, MemoryArena* mem)
{
	YoyoAString *result = PushStruct(mem, YoyoAString);
	void* start_ptr = GetPartitionPointer(*mem);
	char* string_ptr;
	char* at = front.string;
	uint32_t iterations = 0;
	while (*at && iterations < front.length)
	{
		string_ptr = (char*)PushSize(mem, 1);
		*string_ptr = *at;
		result->length++;
		at++;
		iterations++;
	}
	at = back.string;
	iterations = 0;
	while (*at && iterations < back.length)
	{
		string_ptr = (char*)PushSize(mem, 1);
		*string_ptr = *at;
		result->length++;
		at++;
		iterations++;
	}
	result->string = (char*)start_ptr;
	*result = YoyoAsciiNullTerminate(*result);
	return result;
}

#define YoyoAppendCharToStringAndAdvace(front,back,mem) YoyoAppendStringAndAdvance(front,*YoyoAsciiStringFromChar(back,mem),mem)
APIDEF void YoyoAppendStringAndAdvance(YoyoAString* front, YoyoAString back, MemoryArena* mem)
{
	uint32_t length = 0;
	void* start_pointer = GetPartitionPointer(*mem);
	char* str_ptr;
	char* at = front->string;
	uint32_t iterations = 0;

	while (*at && iterations < front->length)
	{
		str_ptr = (char*)PushSize(mem, 1);
		*str_ptr = *at;
		length++;
		at++;
		iterations++;
	}
	at = back.string;
	iterations = 0;

	while (*at && iterations < back.length)
	{
		str_ptr = (char*)PushSize(mem, 1);
		*str_ptr = *at;
		length++;
		at++;
		iterations++;
	}

	front->string = (char*)start_pointer;
	front->length = length;
	*front = YoyoAsciiNullTerminate(*front);
}


