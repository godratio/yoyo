#include "yoyoyo.h"

#include "yoyoyo_memory.h"
#include "yoyoyo_ascii_string.h"
#include "yoyoyo_ascii_vector.h"
#include "yoyo_io.h"
#include "yoyo_file.h"
#include "yoyoyo_hashtable.h"

#include <vector>
#include <stdio.h>

//TODO(Ray):Finish writing tests 
int main(void)
{
//YOYO STRINGs Examples

    MemoryArena* s_arena = YoyoPlatformAllocateArena(MegaBytes(100));
    
    YoyoAString* s = YoyoAsciiStringAllocate("This is a test string");
    *s = YoyoAsciiNullTerminate(*s);


    YoyoyoPlatformOutput(true,"PlatformOUTPUT TEST :  %s \n",s->string);

    uint32_t buf_size = 2048;
    char in_buff[2048];
    YoyoyoPlatformOutputInputPrompt(in_buff,buf_size,true,"Input:");

    YoyoAString* formatted_string = YoyoyoPlatformFormatString(s_arena,"This is a test formatted string %f \n", 0.001f);
    YoyoyoPlatformOutput(true,formatted_string->string);
    
    size_t l = YoyoAsciiGetLengthChar(s->string);
    printf("Testing YoyoAscii Char length :  %d \n",(int)l);
    
    size_t nl = YoyoAsciiStringLength(s);
    printf("Testing YoyoAscii string length :  %d \n",(int)nl);

    size_t sls = YoyoAsciiStringLengthSafe(s,21);    
    printf("Testing YoyoAscii string length Safe :  %d \n",(int)sls);

    size_t slsc = YoyoAsciiGetLengthCharSafely(s->string,21);
    printf("Testing YoyoAscii string length Safe char :  %d \n",(int)slsc);

    YoyoAString* sc = YoyoAsciiStringFromChar("Test char literal 12345!@#$%(){}[]", s_arena);
    *sc = YoyoAsciiNullTerminate(*sc);
    printf("Testing YoyoAscii string from char :  %s \n",sc->string);

    YoyoAString* rsc =  YoyoAsciiCreateStringRangedChar(sc->string, "!", s_arena);
    *rsc = YoyoAsciiNullTerminate(*rsc);
    printf("Testing YoyoAscii string from char to char :  %s \n",rsc->string);

    YoyoAString*rsp = YoyoCreateStringRangedPointer(rsc->string, rsc->string + 3, s_arena);
    *rsp = YoyoAsciiNullTerminate(*rsp);
    printf("Testing YoyoAscii string from char to ptr :  %s \n",rsp->string);

    YoyoAString* sfl = YoyoAsciiStringFromCharLength(rsc->string, 5, s_arena);
    *sfl = YoyoAsciiNullTerminate(*sfl);
    printf("Testing YoyoAscii string from char to length :  %s \n",sfl->string);

    YoyoAString* cmp_equal1 = YoyoAsciiStringFromChar("Comparison Test1234&*(:LKJ)",s_arena);
    char* char_equal1 = "Comparison Test1234&*(:LKJ)";
    YoyoAString* cmp_equal2 = YoyoAsciiStringFromChar("Comparison Test1234&*(:LKJ)",s_arena);
    YoyoAString* cmp_not_equal = YoyoAsciiStringFromChar("!!failComparison Test1234&*(:LKJ)",s_arena);

    if(YoyoAsciiStringCompare(*cmp_equal1,*cmp_equal2))
    {
        printf("Comparison Test Pass.\n");
    }
    else
    {
        printf("Comparison Test Fail!!\n");
    }

    if(!YoyoAsciiStringCompare(*cmp_equal1,*cmp_not_equal))
    {
        printf("Comparison false Test Pass.\n");
    }
    else
    {
        printf("Comparison false Test Fail!!\n");
    }

    if(YoyoAsciiStringCompareToChar(*cmp_equal1,char_equal1))
    {
        printf("string char Comparison Test Pass.\n");
    }
    else
    {
        printf("string char Comparison Test Fail!!\n");
    }

    if(!YoyoAsciiStringCompareToChar(*cmp_not_equal,char_equal1))
    {
        printf("string char false Comparison Test Pass.\n");
    }
    else
    {
        printf("string char false Comparison Test Fail!!\n");
    }

    if(YoyoAsciiCharCompareToChar(char_equal1,char_equal1,10))
    {
        printf("string char Comparison Test Pass.\n");
    }
    else
    {
        printf("string char Comparison Test Fail!!\n");
    }

    if(!YoyoAsciiCharCompareToChar(cmp_not_equal->string,char_equal1,10))
    {
        printf("string char false Comparison Test Pass.\n");
    }
    else
    {
        printf("string char false Comparison Test Fail!!\n");
    }

    YoyoAString* fake_path_file = YoyoAsciiStringFromChar("~/blah/blah/test.png",s_arena);    

    YoyoAString* ext = YoyoAsciiGetFileExtension(fake_path_file, s_arena);
    
    printf("Get Extension result :  %s \n",ext->string);

    YoyoAString* stripped_ext = YoyoAsciiStripExtension(fake_path_file, s_arena);
    printf("Stripped Extension result :  %s \n",stripped_ext->string);

//END YOYO STRING EXAMPLES
   
//BEGIN YOYO VECTOR EXAMPLES
    YoyoVector y_vec = YoyoInitVector(5,int,false);
    int temp_mem = 8;
    for(int i = 0; i < 10;++i)
    {
		YoyoPushBack(&y_vec, i);
        printf(" %d \n",i);
    }

	int* element;
	while(element = YoyoIterateVector(&y_vec,int))
	{
		printf(" %d \n", *element);
	}
	YoyoResetVectorIterator(&y_vec);
	while (element = YoyoIterateVector(&y_vec, int))
	{
		printf(" %d \n", *element);
	}
	YoyoResetVectorIterator(&y_vec);

	for(int i = 0;i < 10;++i)
	{
		int* e = YoyoGetVectorElement(int, &y_vec, i);
		printf(" %d \n", *e);
	}
	int* ep = YoyoPeekVectorElement(int, &y_vec);
	printf(" %d \n", *ep);

	int n = 100;
	YoyoSetVectorElement(&y_vec, 0, (void*)&n);

	while (element = YoyoIterateVector(&y_vec, int))
	{
		printf(" %d \n", *element);
	}
	YoyoResetVectorIterator(&y_vec);

	//TODO(Ray):Finish vector testing :P

    YoyoyoPlatformOutputInputPrompt(in_buff,buf_size,true,"Waiting:");
//END YOYO VECTOR EXAMPLES    

//Begin Hash examples
    int value = 8;
    char* key = "eight";
    
    YoyoHashTable h_t = YoyoInitHashTable(100);
    YoyoAddElementToHashTable(&h_t,key,&value);

    int* extracted_value = YoyoGetElementByHash(int,&h_t,key);
    
	printf("hashtable extracted value is : %d \n", *element);    
//End hash examples


    return 0;
}
