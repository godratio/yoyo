#include "yoyoyo.h"

#include "yoyoyo_memory.h"
#include "yoyoyo_ascii_string.h"
#include "yoyoyo_ascii_vector.h"
#include "yoyo_io.h"
#include "yoyo_file.h"
#include "yoyoyo_hashtable.h"
#include "yoyoyo_math.h"

#include "yoyoyo_math_ext.h"

#define CATCH_CONFIG_RUNNER
#include "./catch2/single_include/catch2/catch.hpp"

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
    //YoyoyoPlatformOutputInputPrompt(in_buff,buf_size,true,"Input:");

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

    //YoyoyoPlatformOutputInputPrompt(in_buff,buf_size,true,"Waiting:");
//END YOYO VECTOR EXAMPLES    

//Begin Hash examples
    int value = 8;
    char* key = "eight";
    
    YoyoHashTable h_t = YoyoInitHashTable(100);
    YoyoAddElementToHashTable(&h_t,key,&value);

    int* extracted_value = YoyoGetElementByHash(int,&h_t,key);
    
	printf("hashtable extracted value is : %d \n", *extracted_value);    
//End hash examples

//Begin Math Examples

    float3 a = float3(0,0,0);
    float3 b = float3(1,1,1);
    float3 c = a + b;
    struct floatn nt = floatn(100);
    float xn = _x(nt);//(nt)x;
    struct floatn xy = _yx(nt);
    YoyoyoPlatformOutput(true, "math : x %f y %f z %f \n",c.x(),c.y(),c.z());
//End Math Examples
//Some math tests
    char* args_c = "";
    int result = Catch::Session().run();
    
//END MATH TESTS
    YoyoyoPlatformOutputInputPrompt(in_buff,buf_size,true,"test finished enter to exit:..");
    return 0;
}

// C++ program to find adjoint and inverse of a matrix
//#include<bits/stdc++.h>
using namespace std;
#define N 4

// Function to get cofactor of A[p][q] in temp[][]. n is current
// dimension of A[][]
void getCofactor(float A[N][N], float temp[N][N], int p, int q, int n)
{
    int i = 0, j = 0;
    
    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those element
            //  which are not in given row and column
            if (row != p && col != q)
            {
                temp[i][j++] = A[row][col];
                
                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

/* Recursive function for finding determinant of matrix.
 n is current dimension of A[][]. */
int determinant(float A[N][N], int n)
{
    int D = 0; // Initialize result
    
    //  Base case : if matrix contains single element
    if (n == 1)
        return A[0][0];
    
    float temp[N][N]; // To store cofactors
    
    int sign = 1;  // To store sign multiplier
    
    // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of A[0][f]
        getCofactor(A, temp, 0, f, n);
        D += sign * A[0][f] * determinant(temp, n - 1);
        
        // terms are to be added with alternate sign
        sign = -sign;
    }
    
    return D;
}
// Function to get adjoint of A[N][N] in adj[N][N].
void adjoint(float A[N][N],float adj[N][N])
{
    if (N == 1)
    {
        adj[0][0] = 1;
        return;
    }
    
    // temp is used to store cofactors of A[][]
    int sign = 1;
    float temp[N][N];
    
    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++)
        {
            // Get cofactor of A[i][j]
            getCofactor(A, temp, i, j, N);
            
            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((i+j)%2==0)? 1: -1;
            
            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj[j][i] = (sign)*(determinant(temp, N-1));
        }
    }
}

// Function to calculate and store inverse, returns false if
// matrix is singular
bool inverse_(float A[N][N], float inverse[N][N])
{
    // Find determinant of A[][]
    int det = determinant(A, N);
    if (det == 0)
    {
        cout << "Singular matrix, can't find its inverse";
        return false;
    }
    
    // Find adjoint
    float adj[N][N];
    adjoint(A, adj);
    
    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++)
            inverse[i][j] = adj[i][j]/float(det);
    
    return true;
}

float4x4 inverse_test(float4x4 a)
{
    float input[4][4];
    for(int i = 0;i < 4;++i)
    {
        float t[4];
        if(i == 0)
            a.c0.store(t);
        if(i == 1)
            a.c1.store(t);
        if(i == 2)
            a.c2.store(t);
        if( i == 3)
            a.c3.store(t);
        
        input[i][0] = t[0];
        input[i][1] = t[1];
        input[i][2] = t[2];
        input[i][3] = t[3];
    }
    float output[4][4];
    inverse_(input, output);
    for(int i = 0;i < 4;++i)
    {
        float t[4];
        if(i == 0)
            a.c0 = float4(input[i][]);// .store(&t);
        if(i == 1)
            a.c1 = float4(input[i][0]);//.store(&t);
        if(i == 2)
            a.c2 = float4(input[i][0]);//.store(&t);
        if( i == 3)
            a.c3 = float4(input[i][0]);//.store(&t);
    }
    return a;
}

//Note(Ray):Const tested values calculated in mathematica
//NOTE(ray):Each section is a loop and the values are reinited for each section.
//confusing if you didnt know that but hand to write it this way.
TEST_CASE( "Operators", "[MATHLIB]" ) {
    //add
    float2 a = float2(0,0);
    float2 b = float2(1,1);
    float2 c = float2(1,1);
    float2 f = float2(0,0);
    SECTION( "+" )
    {
        c = a + b;
        REQUIRE( a.x() == 0);
        REQUIRE( a.y() == 0);
        REQUIRE( b.x() == 1);
        REQUIRE( b.y() == 1);
        REQUIRE( c.x() == 1);
        REQUIRE( c.y() == 1);
    }    

    SECTION( "scalar *" )
    {
        float2 d = float2(0.1f,0.1f);
        float2 e = d * 10;
        REQUIRE(e.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(e.y() == Approx(1.0f).margin(0.000001f));
    }    

    SECTION( "*" )
    {
        f =  b * c;
        REQUIRE(f.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(f.y() == Approx(1.0f).margin(0.000001f));
    }    

    SECTION( "/" )
    {
        a = float2(1,1);
        b = float2(5,5);
        f = a / b;
        REQUIRE(f.x() == Approx(0.2f).margin(0.000001f));
        REQUIRE(f.y() == Approx(0.2f).margin(0.000001f));        
    }

    SECTION( "-" )
    {
        a = float2(10,10);
        b = float2(1,1);
        f = a - b;
        REQUIRE(f.x() == Approx(9.0f).margin(0.000001f));
        REQUIRE(f.y() == Approx(9.0f).margin(0.000001f));        
    }

    SECTION( "*=" )
    {
        a = float2(4,1);
        b = float2(1,4);
        a *= b;
        REQUIRE(a.x() == Approx(4.0f).margin(0.000001f));
        REQUIRE(a.y() == Approx(4.0f).margin(0.000001f));        
    }

    SECTION( "/=" )
    {
        a = float2(1,1);
        b = float2(3,1);
        a /= b;
        REQUIRE(a.x() == Approx(0.33333f).margin(0.000001f));
        REQUIRE(a.y() == Approx(1.0f).margin(0.000001f));
    }
}

TEST_CASE( "HLSL Functions", "[MATHLIB]" )
{
    float2 a = float2(1,1);
    float2 b = float2(100,1);
    
    SECTION( "length" )
    {
        float2 aa = float2(0,0);
        float2 bb = float2(1,1);
        float2 c = aa + bb;
        float l = length(c);
        REQUIRE(l == Approx(1.41421f).margin(0.000001f));
    }
    
    SECTION( "min" )
    {
        a = minimum(a,b);
        REQUIRE(a.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.y() == Approx(1.0f).margin(0.000001f));
    }

    SECTION( "max" )
    {
        a = float2(1,1);
        b = float2(100,1);
        a = maximum(a,b);
        REQUIRE(a.x() == Approx(100.0f).margin(0.000001f));
        REQUIRE(a.y() == Approx(1.0f).margin(0.000001f));
    }

    SECTION( "clamp" )
    {
        a = float2(1,1);
        b = float2(100,1);
        float2 g = float2(1,1);
        a = clamp(g,a,b);
        REQUIRE(a.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.y() == Approx(1.0f).margin(0.000001f));
    }

    SECTION( "sum" )
    {
        a = float2(1,1);
        float h = sum(a);
        REQUIRE(h == Approx(2.0f).margin(0.000001f));
    }

    SECTION( "dot" )
    {
        a = float2(1,1);
        b = float2(100,1);
        float i = dot(a,b);
        REQUIRE(i == Approx(101.0f).margin(0.000001f));
    }

    SECTION( "lengthSq" )
    {
        a = float2(1,1);
        float j = lengthSq(a);
        REQUIRE(j == Approx(2.0f).margin(0.000001f));
    }

    SECTION( "normalize" )
    {
        a = float2(1,1);
        a = normalize(a);
        REQUIRE(a.x() == Approx(0.707107f).margin(0.000001f));
        REQUIRE(a.y() == Approx(0.707107f).margin(0.000001f));
    }

    SECTION( "lerp" )
    {
        a = float2(1,1);
        b = float2(100,1);
        float k = 0.5f;
        a = lerp(a,b,k);
        REQUIRE(a.x() == Approx(50.5f).margin(0.000001f));
        REQUIRE(a.y() == Approx(1.0f).margin(0.000001f));        
    }

    SECTION( "movelh")
    {
        float4 a = float4(1,1,0,0);
        float4 b = float4(2,2,3,3);
        a = movelh(a,b);
        REQUIRE(a.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.z() == Approx(2.0f).margin(0.000001f));
        REQUIRE(a.w() == Approx(2.0f).margin(0.000001f));
    }

    SECTION( "movehl")
    {
        float4 a = float4(1,1,0,0);
        float4 b = float4(2,2,3,3);
        a = movehl(a,b);
        REQUIRE(a.x() == Approx(0.0f).margin(0.000001f));
        REQUIRE(a.y() == Approx(0.0f).margin(0.000001f));
        REQUIRE(a.z() == Approx(3.0f).margin(0.000001f));
        REQUIRE(a.w() == Approx(3.0f).margin(0.000001f));
    }

}

TEST_CASE( "Matrices", "[MATHLIB]" )
{
    float4 c0 = float4(1,0,0,0);
    float4 c1 = float4(0,1,0,0);
    float4 c2 = float4(0,0,1,0);
    float4 c3 = float4(0,0,0,1);
    float4x4 a = float4x4(c0,c1,c2,c3);

    SECTION( "identity 4x4" )
    {
        REQUIRE(a.c0.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c1.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c2.z() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c3.w() == Approx(1.0f).margin(0.000001f));
        //TODO(Ray):check all other cells are zero
    }

    SECTION( "transpose 4x4" )
    {
        a = transpose(a);
        REQUIRE(a.c0.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c1.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c2.z() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c3.w() == Approx(1.0f).margin(0.000001f));
        //TODO(Ray):check all other cells are zero
    }

    SECTION( "translate 4x4")
    {
        a = translate(float3(0.1f,0.5f,1000.0f));
        REQUIRE(a.c3.x() == Approx(0.1f).margin(0.000001f));
        REQUIRE(a.c3.y() == Approx(0.5f).margin(0.000001f));
        REQUIRE(a.c3.z() == Approx(1000.0f).margin(0.000001f));
        REQUIRE(a.c3.w() == Approx(1.0f).margin(0.000001f));
    }

    SECTION( "scale scalar 4x4")
    {
        a = scale(1.0f);
        REQUIRE(a.c0.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c1.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c2.z() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c3.w() == Approx(1.0f).margin(0.000001f));
    }

    SECTION( "scale float3 direct 4x4")
    {
        a = scale(1.0f,1.0f,1.0f);
        REQUIRE(a.c0.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c1.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c2.z() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c3.w() == Approx(1.0f).margin(0.000001f));
    }

    SECTION( "scale float3 4x4")
    {
        a = scale(float3(1.0f,1.0f,1.0f));
        REQUIRE(a.c0.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c1.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c2.z() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c3.w() == Approx(1.0f).margin(0.000001f));
    }

    SECTION( "look_rotation 3x3")
    {
        float3 f = float3(0.0f,0.0f,1.0f);
        float3 u = float3(0.0f,1.0f,0.0f);
                          
        float3x3 a = look_rotation(f,u);
        REQUIRE(a.c0.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c1.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c2.z() == Approx(1.0f).margin(0.000001f));
        
    }

    SECTION( "look_at 4x4")
    {
        float3 e = float3(0.0f,0.0f,1.0f);
        float3 t = float3(0.0f,1.0f,0.0f);
        float3 u = float3(0.0f,1.0f,0.0f);
        
        a = look_at(e,t,u);
        REQUIRE(a.c0.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c1.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c2.z() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c3.w() == Approx(1.0f).margin(0.000001f));
    }

    SECTION( "rotate 4x4")
    {
        float4x4 b = float4x4(1,1,1,1);
        float3 a = float3(0,0,1);
        a = rotate(b,a);
        REQUIRE(a.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.z() == Approx(1.0f).margin(0.000001f));
    }

    SECTION( "transform 4x4")
    {
        float4x4 b = float4x4(1,1,1,1);
        float3 a = float3(0,0,1);
        a = transform(b,a);
        REQUIRE(a.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.z() == Approx(1.0f).margin(0.000001f));
    }

    SECTION( "inverse 4x4 ")
    {
        float4x4 a = float4x4::identity();//(1,1,1,1);
        a = inverse(a);
        REQUIRE(a.c0.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c1.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c2.z() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c3.w() == Approx(1.0f).margin(0.000001f));
    }
    SECTION( "inverse 4x4 ")
    {
        float4x4 a = float4x4::identity();//(1,1,1,1);
        a = inverse_test(a);
        REQUIRE(a.c0.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c1.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c2.z() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c3.w() == Approx(1.0f).margin(0.000001f));
    }

    
/*
    SECTION( "rotate")
    {
        REQUIRE(a.c0.x() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c1.y() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c2.z() == Approx(1.0f).margin(0.000001f));
        REQUIRE(a.c3.w() == Approx(1.0f).margin(0.000001f));
        //TODO(Ray):check all other cells are zero
    }
*/
    
}

//inverse
    
    
    /*
    SECTION( "Pushing int increases count total count is the same" ) {
        int test_int = 100;
        YoyoPushBack(&v, test_int);
        
        REQUIRE( v.total_count == 5 );
        REQUIRE( v.count >= 1 );
    }
    
    SECTION( "resizing bigger changes size and capacity" ) {
        v.resize( 10 );
        
        REQUIRE( v.size() == 10 );
        REQUIRE( v.capacity() >= 10 );
    }
     */


//TODO(Ray):Vector tests.
TEST_CASE( "Vector tests always resize unless you explicity set them not too.", "[vector]" ) {
    
    YoyoVector v = YoyoInitVector(5, int, false);
    
    REQUIRE( v.total_count == 5 );
    REQUIRE( v.count == 0);
    SECTION( "Pushing int increases count total count is the same" ) {
        int test_int = 100;
        YoyoPushBack(&v, test_int);
        
        REQUIRE( v.total_count == 5 );
        REQUIRE( v.count >= 1 );
    }
}
