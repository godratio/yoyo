#pragma once

#include <stdarg.h>
#include <stdio.h>

static void OutputToConsole(b32 UseToggle,const char* FormatString,va_list list)
{
    if (UseToggle)
    {
        char TextBuffer[100];
#if WINDOWS
		vsprintf(TextBuffer,FormatString, list);
		printf("%s",TextBuffer);
#elif OSX
        vsprintf(TextBuffer, FormatString, list);
        printf("%s",TextBuffer);
#endif
    }
}

APIDEF void YoyoyoPlatformOutput(bool use_toggle,const char* FormatString,...)
{
    va_list List;
	va_start(List, FormatString);
	char TextBuffer[100];
    PlatformOutputToConsole(use_toggle,FormatString, List);
	va_end(List);
}

APIDEF void YoyoyoWaitForInput()
{
	char buffer[2048];
	fgets(buffer, 2048, stdin);
}

APIDEF void YoyoyoPlatformOutputInputPrompt(char* Buffer,size_t input_buffer_size,b32 UseToggle,const char* FormatString)
{
//check input buffer size
    YoyoyoPlatformOutput(true,FormatString);
    fgets(Buffer,input_buffer_size,stdin);
}

static YoyoAString* YoyoyoPlatformFormatString(MemoryArena *arena,char* format_string,...)
{
    va_list list;
    va_start(list,format_string);
    char TextBuffer[1000];
#if OSX
    vsprintf(TextBuffer,format_string,list);
#elif WINDOWS
    vsprintf(TextBuffer,format_string,list);
#elif IOS
    vsprintf(TextBuffer,format_string,list);
#endif

    YoyoAString* result = YoyoAsciiStringFromChar(TextBuffer,arena);
    va_end(list);
    return result;
}


