#include "yoyoyo_ascii_string.h"
#include <stdio.h>
int main()
{
    YoyoAString* s = YoyoAsciiStringAllocate("This is a test string");
    *s = YoyoAsciiNullTerminate(*s);
    printf("Testing YoyoAscii Allocate null terminate :  %s \n",s->string);    

    size_t l = YoyoAsciiGetLengthChar(s->string);
    printf("Testing YoyoAscii Char length :  %d \n",(int)l);
    
    size_t nl = YoyoAsciiStringLength(s);
    printf("Testing YoyoAscii string length :  %d \n",(int)nl);

    
    YoyoAString* csl = YoyoAsciiStringAllocate("This is a test create string from literal");
    *csl = YoyoAsciiNullTerminate(*csl);
    printf("Testing YoyoAscii Create String from litereal null terminate :  %s \n",csl->string);

    l = YoyoAsciiGetLengthChar(csl->string);
    printf("Testing YoyoAscii Char length :  %d \n",(int)l);
    
    nl = YoyoAsciiStringLength(csl);
    printf("Testing YoyoAscii string length :  %d \n",(int)nl);
    

    return 0;
}
