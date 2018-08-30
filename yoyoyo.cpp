#include "yoyoyo_ascii_string.h"
#include <stdio.h>
int main()
{
    YoyoAString* s = YoyoAsciiStringAllocate("This is a test string");
    *s = YoyoAsciiNullTerminate(*s);
    printf("Testing YoyoAscii Allocate null terminate :  %s \n",s->string);    

    uint32_t l = GetLengthChar(s->string);
    printf("Testing YoyoAscii Char length :  %d \n",l);
    
    uint32_t nl = YoyoAsciiStrLength(s);
    printf("Testing YoyoAscii string length :  %d \n",nl);

    
    YoyoAString* csl = YoyoAsciiCreateStringFromLiteral("This is a test create string from literal");
    *csl = YoyoAsciiNullTerminate(*csl);
    printf("Testing YoyoAscii Create String from litereal null terminate :  %s \n",csl->string);

    l = GetLengthChar(csl->string);
    printf("Testing YoyoAscii Char length :  %d \n",l);
    
    nl = YoyoAsciiStrLength(csl);
    printf("Testing YoyoAscii string length :  %d \n",nl);
    

    return 0;
}
