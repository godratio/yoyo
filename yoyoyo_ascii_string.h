#pragma once
#include <stdint.h>
#include <stdlib.h>

#define MAX_FILENAME_LENGTH 50
#define MAX_FILE_EXTENSION_LENGTH 10

struct YoyoAString
{
    uint32_t null_teminated;
    uint32_t length;
    char* string;  
};

//NOTE(ray):Assumes string is already null terminated.
static inline uint32_t YoyoAsciiStrLength(YoyoAString* s)
{
    uint32_t length = 0;
    char* at = s->string;
    while(*at)
    {
        length++;
        at++;
    }
    s->length = length;
    return length;
}

//NOTE(ray):Assumes string is already null terminated.
static inline uint32_t GetLengthChar(char* s)
{
    uint32_t length = 0;
    char* at = s;
    while(*at)
    {
        length++;
        at++;
    }
    return length;
}

static inline YoyoAString YoyoAsciiNullTerminate(YoyoAString s)
{
    char* null_point = s.string + s.length;
    *null_point = '\0';
    s.null_teminated = true;
    return s;
}

//TODO(Ray):Make a way to reclaim the memory from literals created here.
//TODO(Ray):Allow to have the option to do the length check safely.
//NOTE(Ray):This function requires you free your own memory once your done.
//Put here or convience not reccomended atm.
static YoyoAString* YoyoAsciiStringAllocate(char* s)
{
    uint32_t length = GetLengthChar(s);
    void* mem = malloc(sizeof(YoyoAString) + length);//PlatformAllocateMemory(sizeof(string) + Length);
    YoyoAString* result = (YoyoAString*)mem;
    result->length = 0;
    char* at = s;
    char* start_ptr  = (char*)mem + sizeof(YoyoAString);
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


