#pragma once
#include "yoyoyo_math.h"

namespace yoyo_math
{

//#if YOYO_MATH_SIMD
//NOTE(RAY):https://www.officedaytime.com/simd512e/simdimg/si.php?f=haddps
    VM_INLINE __m128 HorizontalAdd(__128 a,__128 b)
    {
        return = _mm_hadd_ps(a, b);
    }
//NOTE(Ray):Allowing this outside of scalar area this could be used by anyone.
    //might create a local only version accessing sclar arrays if the compiler does not
    //play nice.
//NOTE(Ray):Check that the compiler is properly inlining our component access calls
//otherwise this will get expensive possiblly.

    VM_INLINE float4 HorizontalAdd(float4 a,float4 b)
    {
        //TODO(Ray):Compress these once we verified all is good.
        float x = a.x() + a.y();
        float y = a.z() + a.w();
        float z = b.x() + b.y();
        float w = b.z() + b.w();
        return float4(x,y,z,w);
    }

//#else

//#endif
}

