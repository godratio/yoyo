#pragma once
#include <stdint.h>
#include <stdlib.h>

#define MAX_FILENAME_LENGTH 50
#define MAX_FILE_EXTENSION_LENGTH 10


struct YoyoAString
{
    bool null_teminated;
    size_t length;
    char* string;  
};

//NOTE(ray):Assumes string is already null terminated.
static inline size_t YoyoAsciiStrLength(YoyoAString* s)
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

static inline size_t YoyoAsciiStrGetLengthSafely(YoyoAString* s,size_t safety_lenth)
{
    size_t length = 0;
    char* at = s->string;
    while(*at)
    {
        length++;
        at++;
        if(length > safety_lenth)
        {
            break;
        }
    }
    s->length = length;
    return length;
}

//NOTE(ray):Assumes string is already null terminated.
static inline size_t GetLengthChar(char* s)
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

static inline size_t YoyoAsciiStrGetLengthSafelyChar(char* s,size_t safety_lenth)
{
    size_t length = 0;
    char* at = s;
    while(*at)
    {
        length++;
        at++;
        if(length > safety_lenth)
        {
            break;
        }
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

static YoyoAString* YoyoAsciiCreateStringFromLiteral(char* s,MemoryArena* mem)
{
    YoyoAString* result = (YoyoAString*)PushSize(mem,sizeof(YoyoAString));
    result->length = 0;
    char* at = String;
    void* start_ptr = GetPartitionPointer(*mem);
	while (*at)
    {
        char * string_ptr = (char*)PushSize(mem,1);
        *string_ptr = *at;
        result->length++;
        at++;
    }
    result->string = (char*)start_ptr;
    return Result;
}

static YoyoAString* YoyoAsciiCreateStringRangedChar(char* String,char* End, MemoryArena* Memory)
{
    string* Result = (string*)PushSize(Memory, sizeof(string));
    
    char* At = String;
    void* StartPointer = GetPartitionPointer(*Memory);
    char* StringPtr = 0;//(char*)Memory;
    while (*At != *End)
    {
        StringPtr = (char*)PushSize(Memory, 1);
        *StringPtr = *At;
        Result->Length++;
        At++;
    }
    Result->String = (char*)StartPointer;
    return Result;
}

static YoyoAString* YoyoAsciiAllocatEmptyString(MemoryArena* mem)
{
    Assert(mem);
    return CreateStringFromLiteral("",mem);
}



