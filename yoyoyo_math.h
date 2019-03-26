//TODOs
//1. ToArray calls
//TODO(Ray):NOTE(Ray):They are currently returning off the stack return to those and rethink later.
//2. 
//TODO(Ray):look up proper define for compilers
//3.
//NOTE(RAY)://TODO(Ray):I dont think we are getting any benefit form __vectorcall in scalar mode so disabling of now

#if !defined(YOYO_MATH_H)

#include <stdint.h>

#if OSX || IOS
#include <math.h>
#elif WINDOWS
#include <cmath>
#endif

#if OSX
#include <x86intrin.h>
#elif WINDOWS
#include <intrin.h>
#endif

//NOTE(Ray):There is a declspec on the impl path so that in MSVC all functiosn are properly exported
#if WINDOWS //MSVC
#if YOYOIMPL
#define VM_INLINE __declspec(dllexport) __forceinline
#else
#define VM_INLINE __forceinline
#endif
#define V_CALL //__vectorcall
#else //XCODE CLANG
#define VM_INLINE __attribute__((always_inline))
#define V_CALL //__vectorcall
#endif

#ifndef M_PI 
#define M_PI        3.14159265358979323846f
#endif

#define M_TAO       6.28318530717f

#define DEG2RAD   M_PI/(float)180
#define RAD2DEG   (float)180 / M_PI

#define DEGREE_TO_RADIAN(a) a * DEG2RAD
#define RADIAN_TO_ANGLE(a)  a * RAD2DEG

//#define DEG2RAD(_a) ((_a)*M_PI/180.0f)
//#define RAD2DEG(_a) ((_a)*180.0f/M_PI)
#define U16MAX 65535

#ifndef INT_MIN
#define INT_MIN     (-2147483647 - 1)
#endif

#ifndef INT_MAX
#define INT_MAX     2147483647
#endif

#ifndef FLX_MAX
#define FLT_MAX     3.402823466e+38F
#endif

#define U32MAX      ((u32)-1)

//#define FLT_MIN     -FLT_MAX
#define POSITIVE_INFINITY  INFINITY;
#define POSITIVE_INFINITY2  float2(INFINITY,INFINITY);
#define POSITIVE_INFINITY3  float3(INFINITY,INFINITY,INFINITY);
#define POSITIVE_INFINITY4  float4(INFINITY,INFINITY,INFINITY,INFINITY);

// Shuffle helpers.
// Examples: SHUFFLE3(v, 0,1,2) leaves the vector unchanged.
//           SHUFFLE3(v, 0,0,0) splats the X coord out.
#define SHUFFLEN(V, X,Y) floatn(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(Y,Y,Y,X)))

//#define YOYO_MATH_SIMD 1
//#define YOYO_USE_SIMD 1

#define SHUFFLE2(V, X,Y) float2(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(Y,Y,Y,X)))
#define SHUFFLE3(V, X,Y,Z) float3(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(Z,Z,Y,X)))
#define SHUFFLE4(V, X,Y,Z,W) float4(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(W,Z,Y,X)))

//Shuffle without dereferencing the this pointer
#define SHUFFLE2m(m, X,Y) float2(_mm_shuffle_ps(m, m, _MM_SHUFFLE(Y,Y,Y,X)))
#define SHUFFLE3m(m, X,Y,Z) float3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(Z,Z,Y,X)))
#define SHUFFLE4m(m, X,Y,Z,W) float4(_mm_shuffle_ps(m, (m, _MM_SHUFFLE(W,Z,Y,X)))

//#include "yoyoyo_simd.h"
//using namespace yoyo_math;
//NOTE(Ray):used only for making pointers to memory that correlates to one of our math types.
union float2data
{
	struct
	{
		float x, y;
	};
	float i[2] = {};
};

union float3data
{
    
	struct
	{
		float x, y, z;
	};
	float i[3] = {};
};

union float4data
{
    struct
    {
        float x, y, z, w;
    };
    float i[4] = {};
};

typedef float4data quaterniondata;
struct float4;
struct float2
{
#if YOYO_MATH_SIMD
    __m128 m;
#else
    float m[2];
#endif

// Constructors.
    VM_INLINE float2();
    VM_INLINE explicit V_CALL float2(const float *p);
    
    VM_INLINE explicit V_CALL float2(float x);
    
    VM_INLINE explicit V_CALL float2(float x, float y);
    
//NOTE(Ray):Cant do this compiler complains about ambigious functions signatures.
    //VM_INLINE explicit V_CALL float2(uint32_t x, uint32_t y) { m = _mm_set_ps((float)y, (float)y, (float)y, (float)x); }
#if YOYO_MATH_SIMD
    VM_INLINE explicit V_CALL float2(__m128 v);
#endif
    
    VM_INLINE float V_CALL x() const;

    VM_INLINE float V_CALL y() const;
    
    VM_INLINE float V_CALL u() const;
 
    VM_INLINE float V_CALL v() const;

	VM_INLINE float2 V_CALL xy() const;

    VM_INLINE float2 V_CALL yx() const;
 
	VM_INLINE float2 V_CALL uv() const;

    VM_INLINE float2 V_CALL vu() const;
 
	VM_INLINE float2data V_CALL tofloat2data();

    VM_INLINE void store(float *p) const;

    VM_INLINE float* to_array();

    void setX(float x);

    void V_CALL setY(float y);
	static uint32_t V_CALL size();
	VM_INLINE float4 V_CALL xyxy() const;
};

struct float3
{

#if YOYO_MATH_SIMD
	__m128 m;
#else
	float m[3];
#endif

	// Constructors.
	VM_INLINE float3();

	VM_INLINE explicit V_CALL float3(const float *p);
	
	VM_INLINE explicit V_CALL float3(float x);

	VM_INLINE explicit V_CALL float3(int x);

	VM_INLINE explicit V_CALL float3(uint32_t x);

	VM_INLINE explicit V_CALL float3(float x, float y, float z);

	VM_INLINE explicit V_CALL float3(float2 xy, float z);
	
	VM_INLINE explicit V_CALL float3(float3data a);

#if YOYO_MATH_SIMD
	VM_INLINE explicit V_CALL float3(__m128 v);
#endif

	VM_INLINE float V_CALL x() const;

	VM_INLINE float V_CALL y() const;

	VM_INLINE float V_CALL z() const;

	VM_INLINE float2 V_CALL xy() const;
    
#ifdef YOYO_USE_PHYSX_EXT
    VM_INLINE explicit V_CALL float3(physx::PxVec3 a);
    VM_INLINE physx::PxVec3 toPhysx();
    //PHsyx extensions
    //physx::PxVec3 toPhysx();
#endif

    VM_INLINE float3data V_CALL tofloat3data();
    
    VM_INLINE void V_CALL store(float *p) const;
	VM_INLINE float4 V_CALL xyxy() const;

    void V_CALL setX(float x);

    void V_CALL setY(float y);

    void V_CALL setZ(float z);

	VM_INLINE static uint32_t size();
	
	VM_INLINE float3 V_CALL xyz() const;

	VM_INLINE float3 V_CALL zyx() const;

	VM_INLINE float3 V_CALL yxz() const;

	VM_INLINE float3 V_CALL yzx() const;
	
	VM_INLINE float3 V_CALL zxy() const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef YOYOIMPL
// Constructors.
VM_INLINE float2::float2()
{
#if YOYO_MATH_SIMD
    m = _mm_set1_ps(0);
#else
    m[0] = 0;
    m[1] = 0;
#endif
}
VM_INLINE V_CALL float2::float2(const float *p)
{
#if YOYO_MATH_SIMD
    m = _mm_set_ps(p[1], p[1], p[1], p[0]);
#else
    m[0] = p[0];
    m[1] = p[1];
#endif
}
    
VM_INLINE V_CALL float2::float2(float x)
{
#if YOYO_MATH_SIMD
    m = _mm_set_ps(x, x, x, x);
#else
    m[0] = x;
    m[1] = x;
#endif
}
    
VM_INLINE V_CALL float2::float2(float x, float y)
{
#if YOYO_MATH_SIMD
    m = _mm_set_ps(y, y, y, x);
#else
    m[0] = x;
    m[1] = y;
#endif
}
    
//NOTE(Ray):Cant do this compiler complains about ambigious functions signatures.
//VM_INLINE explicit V_CALL float2(uint32_t x, uint32_t y) { m = _mm_set_ps((float)y, (float)y, (float)y, (float)x); }
#if YOYO_MATH_SIMD
VM_INLINE V_CALL float2::float2(__m128 v)
{
    m = v;
}
#endif
    
VM_INLINE float V_CALL float2::x() const
{
#if YOYO_MATH_SIMD
    return _mm_cvtss_f32(m);
#else
    return m[0];
#endif
}

VM_INLINE float V_CALL float2::y() const
{
#if YOYO_MATH_SIMD
    return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1)));
#else
    return m[1];
#endif
}
    
VM_INLINE float V_CALL float2::u() const
{
#if YOYO_MATH_SIMD
    return _mm_cvtss_f32(m);
#else
    return m[0];
#endif
}

VM_INLINE float V_CALL float2::v() const
{
#if YOYO_MATH_SIMD
    return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1)));
#else
    return m[1];
#endif
}

VM_INLINE float2 V_CALL float2::xy() const
{
#if YOYO_MATH_SIMD
    return SHUFFLE2(*this, 1, 0);
#else
    return float2(m[0],m[1]);
#endif
}

VM_INLINE float2 V_CALL float2::yx() const
{
#if YOYO_MATH_SIMD
    return SHUFFLE2(*this, 0, 1);
#else
    return float2(m[1],m[0]);
#endif
}

VM_INLINE float2 V_CALL float2::uv() const
{
#if YOYO_MATH_SIMD
    return SHUFFLE2(*this, 1, 0);
#else
    return float2(m[0],m[1]);
#endif
}

VM_INLINE float2 V_CALL float2::vu() const
{
#if YOYO_MATH_SIMD
    return SHUFFLE2(*this, 0, 1);
#else
    return float2(m[1],m[0]);
#endif
}
 
VM_INLINE float2data V_CALL float2::tofloat2data()
{
    float2data a;
#if YOYO_MATH_SIMD
    a.x = _mm_cvtss_f32(m);
    a.y = _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1)));
#else
    a.i[0] = m[0];
    a.i[1] = m[1];
#endif
    return a;
}

VM_INLINE void float2::store(float *p) const
{
#if YOYO_MATH_SIMD
    p[0] = x();
    p[1] = y();
#else
    p[0] = m[0];
    p[1] = m[1];
#endif
}


VM_INLINE float* float2::to_array()
{
#if YOYO_MATH_SIMD
    float result[2] = { x(),y()};
    return result;
#else
    float result[2] = { m[0],m[1]};
    return result;
#endif
}

void float2::setX(float x)
{
#if YOYO_MATH_SIMD
    m = _mm_move_ss(m, _mm_set_ss(x));
#else
    m[0] = x;
#endif
}

void V_CALL float2::setY(float y)
{
#if YOYO_MATH_SIMD
    __m128 t = _mm_move_ss(m, _mm_set_ss(y));
    t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
    m = _mm_move_ss(t, m);
#else
    m[1] = y;
#endif
}

uint32_t V_CALL float2::size() { return sizeof(float) * 2; }



VM_INLINE float3::float3()
    {
#if YOYO_MATH_SIMD
        m = _mm_set1_ps(0.0f);
#else
        m[0] = 0;
        m[1] = 0;
        m[2] = 0;
#endif
    }
	VM_INLINE  V_CALL float3::float3(const float *p)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(p[2], p[2], p[1], p[0]);
#else
		m[0] = p[0];
		m[1] = p[1];
		m[2] = p[2];
#endif
	}
	
	VM_INLINE  V_CALL float3::float3(float x)
	{
		float f_x = x;
#if YOYO_MATH_SIMD
		m = _mm_set_ps(x, x, x, x);
#else
		m[0] = f_x;
		m[1] = f_x;
		m[2] = f_x;
#endif
	}

	VM_INLINE  V_CALL float3::float3(int x)
	{
		float f_x = (float)x;
#if YOYO_MATH_SIMD
		m = _mm_set_ps(f_x, f_x, f_x, f_x);
#else
		m[0] = f_x;
		m[1] = f_x;
		m[2] = f_x;
#endif
	}

	VM_INLINE  V_CALL float3::float3(uint32_t x)
	{
		float f_x = (float)x;
#if YOYO_MATH_SIMD
		m = _mm_set_ps(f_x, f_x, f_x, f_x);
#else
		m[0] = f_x;
		m[1] = f_x;
		m[2] = f_x;
#endif
	}

	VM_INLINE  V_CALL float3::float3(float x, float y, float z)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(y, z, y, x);
#else
		m[0] = x;
		m[1] = y;
		m[2] = z;
#endif
	}
	VM_INLINE  V_CALL float3::float3(float2 xy, float z)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(z, z, xy.y(), xy.x());
#else
		m[0] = xy.m[0];
		m[1] = xy.m[1];
		m[2] = z;
#endif
	}
	
	VM_INLINE  V_CALL float3::float3(float3data a)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(a.z, a.z, a.y, a.x);
#else
		m[0] = a.x;
		m[1] = a.y;
		m[2] = a.z;
#endif
	}

#if YOYO_MATH_SIMD
VM_INLINE  V_CALL float3::float3(__m128 v)
	{
		m = v;
	}
#endif

VM_INLINE float V_CALL float3::x() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(m);
#else
		return m[0];
#endif
	}
	VM_INLINE float V_CALL float3::y() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1)));
#else
		return m[1];
#endif
	}

	VM_INLINE float V_CALL float3::z() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2)));
#else
		return m[2];
#endif
	}

	// Constructors.
    //VM_INLINE float3() {}
    //VM_INLINE explicit V_CALL float3(const float *p) { m = _mm_set_ps(p[2], p[2], p[1], p[0]); }
    //VM_INLINE explicit V_CALL float3(float x) { m = _mm_set_ps(x, x, x, x); }
	//VM_INLINE explicit V_CALL float3(int x) { m = _mm_set_ps((float)x, (float)x, (float)x, (float)x); }
	//VM_INLINE explicit V_CALL float3(float2 xy, float z) { m = _mm_set_ps(z, z, xy.y(), xy.x()); }
	//VM_INLINE explicit V_CALL float3(float3data a) { m = _mm_set_ps(a.z, a.z, a.y, a.x); }
    //VM_INLINE explicit V_CALL float3(float x, float y, float z) { m = _mm_set_ps(z, z, y, x); }
    //VM_INLINE explicit V_CALL float3(__m128 v) { m = v; }

    //VM_INLINE float V_CALL x() const { return _mm_cvtss_f32(m); }
    //VM_INLINE float V_CALL y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    //VM_INLINE float V_CALL z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
	VM_INLINE float2 V_CALL float3::xy() const
    {
#if YOYO_MATH_SIMD
	    return SHUFFLE2(*this,0, 1);
#else
        return float2(m[0], m[1]);
#endif
    }

    
#ifdef YOYO_USE_PHYSX_EXT
    VM_INLINE  V_CALL float3::float3(physx::PxVec3 a)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(a.z, a.z, a.y, a.x);
#else
		m[0] = a.x;
		m[1] = a.y;
		m[2] = a.z;
#endif
	}

#endif

    VM_INLINE float3data V_CALL float3::tofloat3data()
    {
        float3data a;
#if YOYO_MATH_SIMD
        a.x = _mm_cvtss_f32(m);
        a.y = _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1)));
        a.z = _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2)));
#else
		a.i[0] = m[0];
		a.i[1] = m[1];
		a.i[2] = m[2];
#endif
    	return a;
    }
    
    VM_INLINE void V_CALL float3::store(float *p) const { p[0] = x(); p[1] = y(); p[2] = z(); }

    void V_CALL float3::setX(float x)
    {
#if YOYO_MATH_SIMD
        m = _mm_move_ss(m, _mm_set_ss(x));
#else
		m[0] = x;
#endif
    }

    void V_CALL float3::setY(float y)
    {
#if YOYO_MATH_SIMD
        __m128 t = _mm_move_ss(m, _mm_set_ss(y));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
        m = _mm_move_ss(t, m);
#else
		m[1] = y;
#endif
    }

    void V_CALL float3::setZ(float z)
    {
#if YOYO_MATH_SIMD
    	__m128 t = _mm_move_ss(m, _mm_set_ss(z));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
        m = _mm_move_ss(t, m);
#else
		m[2] = z;
#endif
    }

VM_INLINE uint32_t float3::size() { return sizeof(float) * 3; }
	
	VM_INLINE float3 V_CALL float3::xyz() const
    {
#if YOYO_MATH_SIMD
	    return SHUFFLE3(*this, 0, 1, 2);
#else
		return float3(m[0], m[1], m[2]);
#endif
    }
	VM_INLINE float3 V_CALL float3::zyx() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE3(*this, 0, 1, 2);
#else
		return float3(m[2], m[1], m[0]);
#endif
	}
	VM_INLINE float3 V_CALL float3::yxz() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE3(*this, 0, 1, 2);
#else
		return float3(m[1], m[0], m[2]);
#endif
	}

	VM_INLINE float3 V_CALL float3::yzx() const
    {
#if YOYO_MATH_SIMD
	    return SHUFFLE3(*this, 1, 2, 0);
#else
		return float3(m[1], m[2], m[0]);
#endif
    }
	
	VM_INLINE float3 V_CALL float3::zxy() const
    {
#if YOYO_MATH_SIMD
	    return SHUFFLE3(*this, 2, 0, 1);
#else
		return float3(m[2], m[0], m[1]);
#endif
    }









/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

struct quaternion;

struct float4
{
#if YOYO_MATH_SIMD
    __m128 m;
#else
	float m[4];
#endif

	// Constructors.
    VM_INLINE V_CALL float4();

	// Constructors.
	VM_INLINE explicit V_CALL float4(const float *p);

	VM_INLINE explicit V_CALL float4(float x);

	VM_INLINE explicit V_CALL float4(float x, float y, float z,float w);
 
	VM_INLINE explicit V_CALL float4(float4data a);

	VM_INLINE explicit V_CALL float4(float3 a, float b);

	VM_INLINE explicit V_CALL float4(float2 a, float2 b);

#if YOYO_MATH_SIMD
	VM_INLINE explicit V_CALL float4(__m128 v);
#endif

    VM_INLINE float4data V_CALL tofloat4data();

	VM_INLINE float V_CALL x() const;

	VM_INLINE float V_CALL y() const;

	VM_INLINE float V_CALL z() const;

	VM_INLINE float V_CALL w() const;

	VM_INLINE float V_CALL r() const;

	VM_INLINE float V_CALL g() const;

	VM_INLINE float V_CALL b() const;

	VM_INLINE float V_CALL a() const;

	VM_INLINE float V_CALL left() const;

	VM_INLINE float V_CALL top() const;

	VM_INLINE float V_CALL right() const;

	VM_INLINE float V_CALL bottom() const;

	VM_INLINE float3 V_CALL xyz() const;

	VM_INLINE float3 V_CALL zyx() const;

	VM_INLINE float3 V_CALL yxz() const;

	VM_INLINE float3 V_CALL yzx() const;

	VM_INLINE float3 V_CALL zxy() const;

	VM_INLINE float4 V_CALL xzyw() const;

	VM_INLINE float4 V_CALL zwxy() const;

	VM_INLINE float4 V_CALL yyzz() const;

	VM_INLINE float4 V_CALL wwxx() const;

	VM_INLINE float4 V_CALL wxwx() const;

	VM_INLINE float4 V_CALL zyzy() const;

	VM_INLINE float4 V_CALL yxwz() const;

	VM_INLINE float4 V_CALL yxyx() const;

	VM_INLINE float4 V_CALL xwxw() const;

	VM_INLINE float4 V_CALL xyxy() const;

	VM_INLINE float2 V_CALL xy() const;
	
	VM_INLINE float2 V_CALL zw() const;
	
	VM_INLINE float2 V_CALL xx() const;
	
	VM_INLINE float2 V_CALL yz() const;

	VM_INLINE float2 V_CALL wx() const;

	VM_INLINE float2 V_CALL xz() const;

	VM_INLINE float2 V_CALL yx() const;

	VM_INLINE float2 V_CALL yw() const;

	VM_INLINE float2 V_CALL zx() const;

	VM_INLINE float2 V_CALL zz() const;

	VM_INLINE float2 V_CALL wz() const;
	
	VM_INLINE float2 V_CALL wy() const;

#ifdef YOYO_USE_PHYSX_EXT
	VM_INLINE explicit V_CALL float4(physx::PxVec3 a);
	VM_INLINE physx::PxVec4 toPhysx();
#endif
	//NOTE(Ray):Should avoid using these whenever possible
    VM_INLINE void V_CALL store(float *p) const;

	void setX(float x);
	void V_CALL setY(float y);
	void setZ(float x);
	void V_CALL setW(float y);
	static uint32_t size();
};

typedef float2 bool2;
VM_INLINE float2 V_CALL operator+ (float2 a, float2 b);
VM_INLINE float2 V_CALL operator+ (float  a, float2 b);
VM_INLINE float2 V_CALL operator- (float2 a, float2 b); 
VM_INLINE float2 V_CALL operator- (float2 a, float b);
VM_INLINE float2 V_CALL operator- (float  a, float2 b); 
VM_INLINE float2 V_CALL operator* (float2 a, float2 b); 
VM_INLINE float2 V_CALL operator* (float2 a, float b);
VM_INLINE float2 V_CALL operator* (float a, float2 b);
VM_INLINE float2 V_CALL operator/ (float2 a, float2 b);
VM_INLINE float2 V_CALL operator/ (float2 a, float b);
VM_INLINE float2 V_CALL operator/ (float a, float2 b);
VM_INLINE bool2 V_CALL operator==(float2 a, float2 b);
VM_INLINE bool2 V_CALL operator!=(float2 a, float2 b); 
VM_INLINE bool2 V_CALL operator< (float2 a, float2 b);
VM_INLINE bool2 V_CALL operator> (float2 a, float2 b);
VM_INLINE bool2 V_CALL operator<=(float2 a, float2 b);
VM_INLINE bool2 V_CALL operator>=(float2 a, float2 b);
VM_INLINE float2 V_CALL operator- (float2 a);

VM_INLINE float2& V_CALL operator+= (float2 &a, float2 b);
VM_INLINE float2& V_CALL operator-= (float2 &a, float2 b);
VM_INLINE float2& V_CALL operator*= (float2 &a, float2 b);
VM_INLINE float2& V_CALL operator/= (float2 &a, float2 b);
VM_INLINE float2& V_CALL operator*= (float2 &a, float b);
VM_INLINE float2& V_CALL operator/= (float2 &a, float b);

typedef float3 bool3;

VM_INLINE float3 V_CALL operator+ (float3 a, float3 b);
VM_INLINE float3 V_CALL operator+ (float  a, float3 b);
VM_INLINE float3 V_CALL operator+ (float3 a, float b);
VM_INLINE float3 V_CALL operator- (float3 a, float3 b);
VM_INLINE float3 V_CALL operator- (float3 a, float b);
VM_INLINE float3 V_CALL operator- (float  a, float3 b);
VM_INLINE float3 V_CALL operator* (float3 a, float3 b);
VM_INLINE float3 V_CALL operator* (float3 a, float b);
VM_INLINE float3 V_CALL operator* (float a, float3 b);
VM_INLINE float3 V_CALL operator/ (float3 a, float3 b);
VM_INLINE float3 V_CALL operator/ (float3 a, float b);
VM_INLINE float3 V_CALL operator/ (float a, float3 b);
VM_INLINE bool3 V_CALL operator==(float3 a, float3 b);
VM_INLINE bool3 V_CALL operator!=(float3 a, float3 b);
VM_INLINE bool3 V_CALL operator< (float3 a, float3 b);
VM_INLINE bool3 V_CALL operator> (float3 a, float3 b);
VM_INLINE bool3 V_CALL operator<=(float3 a, float3 b);
VM_INLINE bool3 V_CALL operator>=(float3 a, float3 b);
VM_INLINE bool3 V_CALL operator- (float3 a);

//VM_INLINE float3 V_CALL operator+ (float3 a, float3 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
//VM_INLINE float3 V_CALL operator+ (float3 a, float b) { a.m = _mm_add_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float3 V_CALL operator+ (float  a, float3 b) { b.m = _mm_add_ps( _mm_set1_ps(a),b.m); return b; }

//VM_INLINE float3 V_CALL operator- (float3 a, float3 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
//VM_INLINE float3 V_CALL operator- (float3 a, float b) { a.m = _mm_sub_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float3 V_CALL operator- (float  a, float3 b) { b.m = _mm_sub_ps( _mm_set1_ps(a),b.m); return b; }

//VM_INLINE float3 V_CALL operator* (float3 a, float3 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
//VM_INLINE float3 V_CALL operator* (float3 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float3 V_CALL operator* (float a, float3 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }

//VM_INLINE float3 V_CALL operator/ (float3 a, float3 b) { a.m = _mm_div_ps(a.m, b.m); return a; }
//VM_INLINE float3 V_CALL operator/ (float3 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float3 V_CALL operator/ (float a, float3 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }

VM_INLINE float3& V_CALL operator+= (float3 &a, float3 b);
VM_INLINE float3& V_CALL operator-= (float3 &a, float3 b);
VM_INLINE float3& V_CALL operator*= (float3 &a, float3 b);
VM_INLINE float3& V_CALL operator/= (float3 &a, float3 b);
VM_INLINE float3& V_CALL operator*= (float3 &a, float b);
VM_INLINE float3& V_CALL operator/= (float3 &a, float b);

//VM_INLINE bool3 V_CALL operator==(float3 a, float3 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
//VM_INLINE bool3 V_CALL operator!=(float3 a, float3 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
////VM_INLINE bool3 V_CALL operator< (float3 a, float3 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
//VM_INLINE bool3 V_CALL operator> (float3 a, float3 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
//VM_INLINE bool3 V_CALL operator<=(float3 a, float3 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
//VM_INLINE bool3 V_CALL operator>=(float3 a, float3 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
//VM_INLINE float3 V_CALL operator- (float3 a) { return float3(_mm_setzero_ps()) - a; }
//VM_INLINE float3 abs(float3 v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }
// Returns a 3-bit code where bit0..bit2 is X..Z
#if 0
VM_INLINE unsigned V_CALL mask(float3 v)
{
	return _mm_movemask_ps(v.m) & 7;
}
// Once we have a comparison, we can branch based on its results:
VM_INLINE bool V_CALL any(bool3 v) { return mask(v) != 0; }
VM_INLINE bool V_CALL all(bool3 v) { return mask(v) == 7; }
#endif

typedef float4 bool4;

VM_INLINE float4 V_CALL operator+ (float4 a, float4 b);
VM_INLINE float4 V_CALL operator+ (float  a, float4 b);
VM_INLINE float4 V_CALL operator+ (float4 a, float b);
VM_INLINE float4 V_CALL operator- (float4 a, float4 b);
VM_INLINE float4 V_CALL operator- (float4 a, float b);
VM_INLINE float4 V_CALL operator- (float  a, float4 b);
VM_INLINE float4 V_CALL operator* (float4 a, float4 b);
VM_INLINE float4 V_CALL operator* (float4 a, float b);
VM_INLINE float4 V_CALL operator* (float a, float4 b);
VM_INLINE float4 V_CALL operator/ (float4 a, float4 b);
VM_INLINE float4 V_CALL operator/ (float4 a, float b);
VM_INLINE float4 V_CALL operator/ (float a, float4 b);
VM_INLINE bool4 V_CALL operator==(float4 a, float4 b);
VM_INLINE bool4 V_CALL operator!=(float4 a, float4 b);
VM_INLINE bool4 V_CALL operator< (float4 a, float4 b);
VM_INLINE bool4 V_CALL operator> (float4 a, float4 b);
VM_INLINE bool4 V_CALL operator<=(float4 a, float4 b);
VM_INLINE bool4 V_CALL operator>=(float4 a, float4 b);
VM_INLINE bool4 V_CALL operator- (float4 a);

//VM_INLINE float4 V_CALL operator+ (float4 a, float4 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
//VM_INLINE float4 V_CALL operator+ (float4 a, float b) { a.m = _mm_add_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float4 V_CALL operator+ (float  a, float4 b) { b.m = _mm_add_ps( _mm_set1_ps(a),b.m); return b; }

//VM_INLINE float4 V_CALL operator- (float4 a, float4 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
//VM_INLINE float4 V_CALL operator- (float4 a, float b) { a.m = _mm_sub_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float4 V_CALL operator- (float  a, float4 b) { b.m = _mm_sub_ps( _mm_set1_ps(a),b.m); return b; }

//VM_INLINE float4 V_CALL operator* (float4 a, float4 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
//VM_INLINE float4 V_CALL operator* (float4 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float4 V_CALL operator* (float a, float4 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }

//VM_INLINE float4 V_CALL operator/ (float4 a, float4 b) { a.m = _mm_div_ps(a.m, b.m); return a; }
//VM_INLINE float4 V_CALL operator/ (float4 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float4 V_CALL operator/ (float a, float4 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }

VM_INLINE float4& V_CALL operator+= (float4 &a, float4 b);
VM_INLINE float4& V_CALL operator-= (float4 &a, float4 b);

VM_INLINE float4& V_CALL operator*= (float4 &a, float4 b);
VM_INLINE float4& V_CALL operator*= (float4 &a, float b);

VM_INLINE float4& V_CALL operator/= (float4 &a, float4 b);
VM_INLINE float4& V_CALL operator/= (float4 &a, float b);

//VM_INLINE bool4 V_CALL operator==(float4 a, float4 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
//VM_INLINE bool4 V_CALL operator!=(float4 a, float4 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
//VM_INLINE bool4 V_CALL operator< (float4 a, float4 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
//VM_INLINE bool4 V_CALL operator> (float4 a, float4 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
//VM_INLINE bool4 V_CALL operator<=(float4 a, float4 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
//VM_INLINE bool4 V_CALL operator>=(float4 a, float4 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
//VM_INLINE float4 V_CALL operator- (float4 a) { return float4(_mm_setzero_ps()) - a; }

#ifdef YOYOIMPL

// Constructors.
VM_INLINE V_CALL float4::float4()
    {
#if YOYO_MATH_SIMD
        m = _mm_set1_ps(0.0f);
#else
        m[0] = 0;
        m[1] = 0;
        m[2] = 0;
        m[3] = 0;
#endif
    }

	// Constructors.
	VM_INLINE  V_CALL float4::float4(const float *p)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(p[3], p[2], p[1], p[0]);
#else
		m[0] = p[0];
		m[1] = p[1];
		m[2] = p[2];
		m[3] = p[3];
#endif
	}

	VM_INLINE  V_CALL float4::float4(float x)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(x, x, x, x);
#else
		m[0] = x;
		m[1] = x;
		m[2] = x;
		m[3] = x;
#endif
	}

	VM_INLINE  V_CALL float4::float4(float x, float y, float z,float w)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(w, z, y, x);
#else
		m[0] = x;
		m[1] = y;
		m[2] = z;
		m[3] = w;
#endif
	}

	VM_INLINE  V_CALL float4::float4(float4data a)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(a.w, a.z, a.y, a.x);
#else
		m[0] = a.x;
		m[1] = a.y;
		m[2] = a.z;
		m[3] = a.w;
#endif
	}

VM_INLINE  V_CALL float4::float4(float3 a, float b)
    {
#if YOYO_MATH_SIMD
	    m = _mm_set_ps(b, a.z(), a.y(), a.x());
#else
		m[0] = a.x();
		m[1] = a.y();
		m[2] = a.z();
		m[3] = b;
#endif
    }

VM_INLINE  V_CALL float4::float4(float2 a, float2 b)
    {
#if YOYO_MATH_SIMD
	    m = _mm_set_ps(b.y(), b.x(), a.y(), a.x());
#else
		m[0] = a.m[0];
		m[1] = a.m[1];
		m[2] = b.m[0];
		m[3] = b.m[1];
#endif
    }

	//VM_INLINE explicit V_CALL float4(const float *p) { m = _mm_set_ps(p[3], p[2], p[1], p[0]); }
    //VM_INLINE explicit V_CALL float4(float x, float y, float z,float w) { m = _mm_set_ps(w, z, y, x); }
    //VM_INLINE explicit V_CALL float4(float x) { m = _mm_set_ps(x, x, x, x); }
#if YOYO_MATH_SIMD
	VM_INLINE  V_CALL float4::float4(__m128 v) { m = v; }
#endif

	//VM_INLINE explicit V_CALL float4(float3 a,float b){m = _mm_set_ps(b,a.z(),a.y(),a.x());}
    //VM_INLINE explicit V_CALL float4(float2 a,float2 b){m = _mm_set_ps(b.y(),b.x(),a.y(),a.x());}

    VM_INLINE float4data V_CALL float4::tofloat4data()
    {
#if YOYO_MATH_SIMD
        float4data a;
        a.x = _mm_cvtss_f32(m);
        a.y = _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1)));
        a.z = _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2)));
        a.w = _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3)));
#else
		float4data a;
		a.x = m[0];
		a.y = m[1];
		a.z = m[2];
		a.w = m[3];
#endif
    	return a;
    }

	VM_INLINE float V_CALL float4::x() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(m);
#else
		return m[0];
#endif
	}

VM_INLINE float V_CALL float4::y() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1)));
#else
		return m[1];
#endif
	}

	VM_INLINE float V_CALL float4::z() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2)));
#else
		return m[2];
#endif
	}

VM_INLINE float V_CALL float4::w() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3,3, 3, 3)));
#else
		return m[3];
#endif
	}

	VM_INLINE float V_CALL float4::r() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(m);
#else
		return m[0];
#endif
	}

VM_INLINE float V_CALL float4::g() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1)));
#else
		return m[1];
#endif
	}

	VM_INLINE float V_CALL float4::b() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2)));
#else
		return m[2];
#endif
	}

VM_INLINE float V_CALL float4::a() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3)));
#else
		return m[3];
#endif
	}

	VM_INLINE float V_CALL float4::left() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(m);
#else
		return m[0];
#endif
	}

VM_INLINE float V_CALL float4::top() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1)));
#else
		return m[1];
#endif
	}

	VM_INLINE float V_CALL float4::right() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2)));
#else
		return m[2];
#endif
	}

VM_INLINE float V_CALL float4::bottom() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3)));
#else
		return m[3];
#endif
	}

VM_INLINE float3 V_CALL float4::xyz() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE3(*this, 0, 1, 2);
#else
		return float3(m[0], m[1], m[2]);
#endif
	}

VM_INLINE float3 V_CALL float4::zyx() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE3(*this, 0, 1, 2);
#else
		return float3(m[2], m[1], m[0]);
#endif
	}

	VM_INLINE float3 V_CALL float4::yxz() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE3(*this, 0, 1, 2);
#else
		return float3(m[1], m[0], m[2]);
#endif
	}

	VM_INLINE float3 V_CALL float4::yzx() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE3(*this, 1, 2, 0);
#else
		return float3(m[1], m[2], m[0]);
#endif
	}

	VM_INLINE float3 V_CALL float4::zxy() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE3(*this, 2, 0, 1);
#else
		return float3(m[2], m[0], m[1]);
#endif
	}

VM_INLINE float4 V_CALL float4::xzyw() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE4(*this, 0, 1, 2, 3);
#else
		return float4(m[0], m[1], m[2], m[3]);
#endif
	}

	VM_INLINE float4 V_CALL float4::zwxy() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE4(*this, 2, 3, 0, 1);
#else
		return float4(m[2], m[3], m[0], m[1]);
#endif
	}

	VM_INLINE float4 V_CALL float4::yyzz() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE4(*this, 1, 1, 2, 2);
#else
		return float4(m[1], m[1], m[2], m[2]);
#endif
	}

VM_INLINE float4 V_CALL float4::wwxx() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE4(*this, 3, 3, 0, 0);
#else
		return float4(m[3], m[3], m[0], m[0]);
#endif
	}

	VM_INLINE float4 V_CALL float4::wxwx() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE4(*this, 3, 0, 3, 0);
#else
		return float4(m[3], m[0], m[3], m[0]);
#endif
	}

	VM_INLINE float4 V_CALL float4::zyzy() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE4(*this, 2, 1, 2, 1);
#else
		return float4(m[2], m[1], m[2], m[1]);
#endif
	}

	VM_INLINE float4 V_CALL float4::yxwz() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE4(*this, 1, 0, 3, 2);
#else
		return float4(m[1], m[0], m[3], m[2]);
#endif
	}

	VM_INLINE float4 V_CALL float4::yxyx() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE4(*this, 1, 0, 1, 0);
#else
		return float4(m[1], m[0], m[1], m[0]);
#endif
	}

VM_INLINE float4 V_CALL float4::xwxw() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE4(*this, 0, 3, 0, 3);
#else
		return float4(m[0], m[3], m[0], m[3]);
#endif
	}
	

	VM_INLINE float4 V_CALL float4::xyxy() const
    {
#if YOYO_MATH_SIMD
	    return SHUFFLE4(*this, 0, 1, 0, 1);
#else
		return float4(m[0], m[1], m[0], m[1]);
#endif
    }

	VM_INLINE float2 V_CALL float4::xy() const
    {
#if YOYO_USE_SIMD
        return SHUFFLE2(*this,0, 1);
#else
        return float2(m[0], m[1]);
#endif
    }
	
	VM_INLINE float2 V_CALL float4::zw() const
	{
#if YOYO_USE_SIMD
		return SHUFFLE2(*this, 2, 3);
#else
        return float2(m[2], m[3]);
#endif
	}
	
	VM_INLINE float2 V_CALL float4::xx() const
	{
#if YOYO_USE_SIMD
		return SHUFFLE2(*this, 0, 0);
#else
        return float2(m[0], m[0]);
#endif
	}
	
	VM_INLINE float2 V_CALL float4::yz() const
	{
#if YOYO_USE_SIMD
		return SHUFFLE2(*this, 1, 2);
#else
        return float2(m[1], m[2]);
#endif
	}

	VM_INLINE float2 V_CALL float4::wx() const
	{
#if YOYO_USE_SIMD
		return SHUFFLE2(*this, 3, 0);
#else
        return float2(m[3], m[0]);
#endif
	}

	VM_INLINE float2 V_CALL float4::xz() const
	{
#if YOYO_USE_SIMD
		return SHUFFLE2(*this, 0, 2);
#else
        return float2(m[0], m[2]);
#endif
	}

	VM_INLINE float2 V_CALL float4::yx() const
	{
#if YOYO_USE_SIMD
		return SHUFFLE2(*this, 1, 0);
#else
        return float2(m[1], m[0]);
#endif
	}

	VM_INLINE float2 V_CALL float4::yw() const
	{
#if YOYO_USE_SIMD
		return SHUFFLE2(*this, 1, 3);
#else
        return float2(m[1], m[3]);
#endif
	}

	VM_INLINE float2 V_CALL float4::zx() const
	{
#if YOYO_USE_SIMD
		return SHUFFLE2(*this, 2, 0);
#else
        return float2(m[2], m[0]);
#endif
	}

	VM_INLINE float2 V_CALL float4::zz() const
	{
#if YOYO_USE_SIMD
		return SHUFFLE2(*this, 2, 2);
#else
        return float2(m[2], m[2]);
#endif
	}

	VM_INLINE float2 V_CALL float4::wz() const
	{
#if YOYO_USE_SIMD
		return SHUFFLE2(*this, 3, 2);
#else
        return float2(m[3], m[2]);
#endif
	}
	
	VM_INLINE float2 V_CALL float4::wy() const
	{
#if YOYO_USE_SIMD
		return SHUFFLE2(*this, 3, 1);
#else
        return float2(m[3], m[1]);
#endif
	}

#ifdef YOYO_USE_PHYSX_EXT
VM_INLINE V_CALL float4::float4(physx::PxVec3 a)
    {
#if YOYO_MATH_SIMD
	    m = _mm_set_ps(a.z, a.z, a.y, a.x);
#else
		m[0] = a.x;
		m[1] = a.y;
		m[2] = a.z;
		m[3] = 0;
#endif
    }

#endif
	//NOTE(Ray):Should avoid using these whenever possible

VM_INLINE void V_CALL float4::store(float *p) const { p[0] = x(); p[1] = y(); p[2] = z(); p[3] = w(); }

void float4::setX(float x)
	{
#if YOYO_MATH_SIMD
		m = _mm_move_ss(m, _mm_set_ss(x));
#else
		m[0] = x;
#endif
	}

void V_CALL float4::setY(float y)
	{
#if YOYO_MATH_SIMD
		__m128 t = _mm_move_ss(m, _mm_set_ss(y));
		t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
		m = _mm_move_ss(t, m);
#else
		m[1] = y;
#endif
	}

void float4::setZ(float x)
	{
#if YOYO_MATH_SIMD
        __m128 t = _mm_move_ss(m, _mm_set_ss(z()));
		t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
		m = _mm_move_ss(t, m);
#else
		m[2] = x;
#endif
	}

void V_CALL float4::setW(float y)
	{
#if YOYO_MATH_SIMD
        __m128 t = _mm_move_ss(m, _mm_set_ss(w()));
		t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(0, 2, 1, 0));
		m = _mm_move_ss(t, m);
#else
		m[3] = y;
#endif
	}

uint32_t float4::size() { return sizeof(float) * 4; }

VM_INLINE float4 V_CALL float2::xyxy() const
{
#if YOYO_MATH_SIMD
	return SHUFFLE4(*this, 0, 1, 0, 1);
#else
	return float4(m[0], m[1],m[0],m[1]);
#endif
}

VM_INLINE float4 V_CALL float3::xyxy() const
{
#if YOYO_MATH_SIMD
	return SHUFFLE4(*this, 0, 1, 0, 1);
#else
	return float4(m[0], m[1], m[0], m[1]);
#endif
}


//OPERATORS
VM_INLINE float2 V_CALL operator+ (float2 a, float2 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_add_ps(a.m, b.m); 
	return a;
#else
	a.m[0] = a.m[0] + b.m[0];
	a.m[1] = a.m[1] + b.m[1];
	return a;
#endif
}

VM_INLINE float2 V_CALL operator+ (float  a, float2 b)
{
#if YOYO_MATH_SIMD
	b.m = _mm_add_ps(_mm_set1_ps(a), b.m); 
#else
	b.m[0] = a + b.m[0];
	b.m[1] = a + b.m[1];
#endif
	return b;
}

VM_INLINE float2 V_CALL operator- (float2 a, float2 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_sub_ps(a.m, b.m); 
#else
	a.m[0] = a.m[0] - b.m[0];
	a.m[1] = a.m[1] - b.m[1];
#endif
	return a;
}

VM_INLINE float2 V_CALL operator- (float2 a, float b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_sub_ps(a.m, _mm_set1_ps(b)); 
#else
	a.m[0] = a.m[0] - b;
	a.m[1] = a.m[1] - b;
#endif
	return a;
}

VM_INLINE float2 V_CALL operator- (float  a, float2 b)
{
#if YOYO_MATH_SIMD
	b.m = _mm_sub_ps(_mm_set1_ps(a), b.m); 
#else
	b.m[0] = a - b.m[0];
	b.m[1] = a - b.m[1];
#endif
	return b;
}

VM_INLINE float2 V_CALL operator* (float2 a, float2 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_mul_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] * b.m[0];
	a.m[1] = a.m[1] * b.m[1];
#endif
	return a;
}

VM_INLINE float2 V_CALL operator* (float2 a, float b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); 
#else
	a.m[0] = a.m[0] * b;
	a.m[1] = a.m[1] * b;
#endif
	return a;
}

VM_INLINE float2 V_CALL operator* (float a, float2 b)
{
#if YOYO_MATH_SIMD
	b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); 
#else
	b.m[0] = a * b.m[0];
	b.m[1] = a * b.m[1];
#endif
	return b;
}

VM_INLINE float2 V_CALL operator/ (float2 a, float2 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_div_ps(a.m, b.m); 
#else
	a.m[0] = a.m[0] / b.m[0];
	a.m[1] = a.m[1] / b.m[1];
#endif
	return a;
}

VM_INLINE float2 V_CALL operator/ (float2 a, float b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); 
#else
	a.m[0] = a.m[0] * b;
	a.m[1] = a.m[1] * b;
#endif
	return a;
}

VM_INLINE float2 V_CALL operator/ (float a, float2 b)
{
#if YOYO_MATH_SIMD
	b.m = _mm_div_ps(_mm_set1_ps(a), b.m); 
#else
	b.m[0] = a / b.m[0];
	b.m[1] = a / b.m[1];
#endif
	return b;
}

VM_INLINE bool2 V_CALL operator==(float2 a, float2 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmpeq_ps(a.m, b.m); 
#else
	//TODO(Ray):What kind of equality should we check here bitwise?
	a.m[0] = a.m[0] == b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] == b.m[1] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool2 V_CALL operator!=(float2 a, float2 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmpneq_ps(a.m, b.m); 
#else
	a.m[0] = a.m[0] != b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] != b.m[1] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool2 V_CALL operator< (float2 a, float2 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmplt_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] < b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] < b.m[1] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool2 V_CALL operator> (float2 a, float2 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmpgt_ps(a.m, b.m); 
#else
	a.m[0] = a.m[0] > b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] > b.m[1] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool2 V_CALL operator<=(float2 a, float2 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmple_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] <= b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] <= b.m[1] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool2 V_CALL operator>=(float2 a, float2 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmpge_ps(a.m, b.m); 
#else
	a.m[0] = a.m[0] >= b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] >= b.m[1] ? 1 : 0;
#endif
	return a;
}

VM_INLINE float2 V_CALL operator- (float2 a)
{
#if YOYO_MATH_SIMD
	return float2(_mm_setzero_ps()) - a;
#else
	return float2(0.0f) - a;
#endif

}



VM_INLINE float2& V_CALL operator+= (float2 &a, float2 b) { a = a + b; return a; }
VM_INLINE float2& V_CALL operator-= (float2 &a, float2 b) { a = a - b; return a; }
VM_INLINE float2& V_CALL operator*= (float2 &a, float2 b) { a = a * b; return a; }
VM_INLINE float2& V_CALL operator/= (float2 &a, float2 b) { a = a / b; return a; }
VM_INLINE float2& V_CALL operator*= (float2 &a, float b) { a = a * b; return a; }
VM_INLINE float2& V_CALL operator/= (float2 &a, float b) { a = a / b; return a; }


typedef float3 bool3;

VM_INLINE float3 V_CALL operator+ (float3 a, float3 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_add_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] + b.m[0];
	a.m[1] = a.m[1] + b.m[1];
	a.m[2] = a.m[2] + b.m[2];
	
#endif
    return a;
}

VM_INLINE float3 V_CALL operator+ (float  a, float3 b)
{
#if YOYO_MATH_SIMD
	b.m = _mm_add_ps(_mm_set1_ps(a), b.m);
#else
	b.m[0] = a + b.m[0];
	b.m[1] = a + b.m[1];
	b.m[2] = a + b.m[2];
#endif
	return b;
}

VM_INLINE float3 V_CALL operator+ (float3 a, float b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_add_ps(a.m, _mm_set1_ps(b));
#else
	a.m[0] = a.m[0] + b;
	a.m[1] = a.m[1] + b;
	a.m[2] = a.m[2] + b;
#endif
	return a;

}

VM_INLINE float3 V_CALL operator- (float3 a, float3 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_sub_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] - b.m[0];
	a.m[1] = a.m[1] - b.m[1];
	a.m[2] = a.m[2] - b.m[2];
#endif
	return a;
}

VM_INLINE float3 V_CALL operator- (float3 a, float b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_sub_ps(a.m, _mm_set1_ps(b));
#else
	a.m[0] = a.m[0] - b;
	a.m[1] = a.m[1] - b;
	a.m[2] = a.m[2] - b;
#endif
	return a;
}

VM_INLINE float3 V_CALL operator- (float  a, float3 b)
{
#if YOYO_MATH_SIMD
	b.m = _mm_sub_ps(_mm_set1_ps(a), b.m);
#else
	b.m[0] = a - b.m[0];
	b.m[1] = a - b.m[1];
	b.m[2] = a - b.m[2];
#endif
	return b;
}

VM_INLINE float3 V_CALL operator* (float3 a, float3 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_mul_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] * b.m[0];
	a.m[1] = a.m[1] * b.m[1];
	a.m[2] = a.m[2] * b.m[2];
#endif
	return a;
}

VM_INLINE float3 V_CALL operator* (float3 a, float b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_mul_ps(a.m, _mm_set1_ps(b));
#else
	a.m[0] = a.m[0] * b;
	a.m[1] = a.m[1] * b;
	a.m[2] = a.m[2] * b;
#endif
	return a;
}

VM_INLINE float3 V_CALL operator* (float a, float3 b)
{
#if YOYO_MATH_SIMD
	b.m = _mm_mul_ps(_mm_set1_ps(a), b.m);
#else
	b.m[0] = a * b.m[0];
	b.m[1] = a * b.m[1];
	b.m[2] = a * b.m[2];
#endif
	return b;
}

VM_INLINE float3 V_CALL operator/ (float3 a, float3 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_div_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] / b.m[0];
	a.m[1] = a.m[1] / b.m[1];
	a.m[2] = a.m[2] / b.m[2];
#endif
	return a;
}

VM_INLINE float3 V_CALL operator/ (float3 a, float b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_div_ps(a.m, _mm_set1_ps(b));
#else
	a.m[0] = a.m[0] / b;
	a.m[1] = a.m[1] / b;
	a.m[2] = a.m[2] / b;
#endif
	return a;
}

VM_INLINE float3 V_CALL operator/ (float a, float3 b)
{
#if YOYO_MATH_SIMD
	b.m = _mm_div_ps(_mm_set1_ps(a), b.m);
#else
	b.m[0] = a / b.m[0];
	b.m[1] = a / b.m[1];
	b.m[2] = a / b.m[2];
#endif
	return b;
}


VM_INLINE bool3 V_CALL operator==(float3 a, float3 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmpeq_ps(a.m, b.m);
#else
	//TODO(Ray):What kind of equality should we check here bitwise?
	a.m[0] = a.m[0] == b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] == b.m[1] ? 1 : 0;
	a.m[2] = a.m[2] == b.m[2] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool3 V_CALL operator!=(float3 a, float3 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmpneq_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] != b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] != b.m[1] ? 1 : 0;
	a.m[2] = a.m[2] != b.m[2] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool3 V_CALL operator< (float3 a, float3 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmplt_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] < b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] < b.m[1] ? 1 : 0;
	a.m[2] = a.m[2] < b.m[2] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool3 V_CALL operator> (float3 a, float3 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmpgt_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] > b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] > b.m[1] ? 1 : 0;
	a.m[2] = a.m[2] > b.m[2] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool3 V_CALL operator<=(float3 a, float3 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmple_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] <= b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] <= b.m[1] ? 1 : 0;
	a.m[2] = a.m[2] <= b.m[2] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool3 V_CALL operator>=(float3 a, float3 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmpge_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] >= b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] >= b.m[1] ? 1 : 0;
	a.m[2] = a.m[2] >= b.m[2] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool3 V_CALL operator- (float3 a)
{
#if YOYO_MATH_SIMD
    return float3(_mm_setzero_ps()) - a;
#else
	return float3(0.0f) - a;
#endif

}

//VM_INLINE float3 V_CALL operator+ (float3 a, float3 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
//VM_INLINE float3 V_CALL operator+ (float3 a, float b) { a.m = _mm_add_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float3 V_CALL operator+ (float  a, float3 b) { b.m = _mm_add_ps( _mm_set1_ps(a),b.m); return b; }

//VM_INLINE float3 V_CALL operator- (float3 a, float3 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
//VM_INLINE float3 V_CALL operator- (float3 a, float b) { a.m = _mm_sub_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float3 V_CALL operator- (float  a, float3 b) { b.m = _mm_sub_ps( _mm_set1_ps(a),b.m); return b; }

//VM_INLINE float3 V_CALL operator* (float3 a, float3 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
//VM_INLINE float3 V_CALL operator* (float3 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float3 V_CALL operator* (float a, float3 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }

//VM_INLINE float3 V_CALL operator/ (float3 a, float3 b) { a.m = _mm_div_ps(a.m, b.m); return a; }
//VM_INLINE float3 V_CALL operator/ (float3 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float3 V_CALL operator/ (float a, float3 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }

VM_INLINE float3& V_CALL operator+= (float3 &a, float3 b) { a = a + b; return a; }
VM_INLINE float3& V_CALL operator-= (float3 &a, float3 b) { a = a - b; return a; }
VM_INLINE float3& V_CALL operator*= (float3 &a, float3 b) { a = a * b; return a; }
VM_INLINE float3& V_CALL operator/= (float3 &a, float3 b) { a = a / b; return a; }
VM_INLINE float3& V_CALL operator*= (float3 &a, float b) { a = a * b; return a; }
VM_INLINE float3& V_CALL operator/= (float3 &a, float b) { a = a / b; return a; }

//VM_INLINE bool3 V_CALL operator==(float3 a, float3 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
//VM_INLINE bool3 V_CALL operator!=(float3 a, float3 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
////VM_INLINE bool3 V_CALL operator< (float3 a, float3 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
//VM_INLINE bool3 V_CALL operator> (float3 a, float3 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
//VM_INLINE bool3 V_CALL operator<=(float3 a, float3 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
//VM_INLINE bool3 V_CALL operator>=(float3 a, float3 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
//VM_INLINE float3 V_CALL operator- (float3 a) { return float3(_mm_setzero_ps()) - a; }
//VM_INLINE float3 abs(float3 v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }
// Returns a 3-bit code where bit0..bit2 is X..Z
#if 0
VM_INLINE unsigned V_CALL mask(float3 v)
{
	return _mm_movemask_ps(v.m) & 7;
}
// Once we have a comparison, we can branch based on its results:
VM_INLINE bool V_CALL any(bool3 v) { return mask(v) != 0; }
VM_INLINE bool V_CALL all(bool3 v) { return mask(v) == 7; }
#endif


typedef float4 bool4;

VM_INLINE float4 V_CALL operator+ (float4 a, float4 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_add_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] + b.m[0];
	a.m[1] = a.m[1] + b.m[1];
	a.m[2] = a.m[2] + b.m[2];
	a.m[3] = a.m[3] + b.m[3];
#endif
    return a;
}

VM_INLINE float4 V_CALL operator+ (float  a, float4 b)
{
#if YOYO_MATH_SIMD
	b.m = _mm_add_ps(_mm_set1_ps(a), b.m);
#else
	b.m[0] = a + b.m[0];
	b.m[1] = a + b.m[1];
	b.m[2] = a + b.m[2];
	b.m[3] = a + b.m[3];
#endif
	return b;
}

VM_INLINE float4 V_CALL operator+ (float4 a, float b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_add_ps(a.m, _mm_set1_ps(b));
#else
	a.m[0] = a.m[0] + b;
	a.m[1] = a.m[1] + b;
	a.m[2] = a.m[2] + b;
	a.m[3] = a.m[3] + b;
#endif
	return a;

}

VM_INLINE float4 V_CALL operator- (float4 a, float4 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_sub_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] - b.m[0];
	a.m[1] = a.m[1] - b.m[1];
	a.m[2] = a.m[2] - b.m[2];
	a.m[3] = a.m[3] - b.m[3];
#endif
	return a;
}

VM_INLINE float4 V_CALL operator- (float4 a, float b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_sub_ps(a.m, _mm_set1_ps(b));
#else
	a.m[0] = a.m[0] - b;
	a.m[1] = a.m[1] - b;
	a.m[2] = a.m[2] - b;
	a.m[3] = a.m[3] - b;
#endif
	return a;
}

VM_INLINE float4 V_CALL operator- (float  a, float4 b)
{
#if YOYO_MATH_SIMD
	b.m = _mm_sub_ps(_mm_set1_ps(a), b.m);
#else
	b.m[0] = a - b.m[0];
	b.m[1] = a - b.m[1];
	b.m[2] = a - b.m[2];
	b.m[3] = a - b.m[3];
#endif
	return b;
}

VM_INLINE float4 V_CALL operator* (float4 a, float4 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_mul_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] * b.m[0];
	a.m[1] = a.m[1] * b.m[1];
	a.m[2] = a.m[2] * b.m[2];
	a.m[3] = a.m[3] * b.m[3];
#endif
	return a;
}

VM_INLINE float4 V_CALL operator* (float4 a, float b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_mul_ps(a.m, _mm_set1_ps(b));
#else
	a.m[0] = a.m[0] * b;
	a.m[1] = a.m[1] * b;
	a.m[2] = a.m[2] * b;
	a.m[3] = a.m[3] * b;
#endif
	return a;
}

VM_INLINE float4 V_CALL operator* (float a, float4 b)
{
#if YOYO_MATH_SIMD
	b.m = _mm_mul_ps(_mm_set1_ps(a), b.m);
#else
	b.m[0] = a * b.m[0];
	b.m[1] = a * b.m[1];
	b.m[2] = a * b.m[2];
	b.m[3] = a * b.m[3];
#endif
	return b;
}

VM_INLINE float4 V_CALL operator/ (float4 a, float4 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_div_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] / b.m[0];
	a.m[1] = a.m[1] / b.m[1];
	a.m[2] = a.m[2] / b.m[2];
	a.m[3] = a.m[3] / b.m[3];
#endif
	return a;
}

VM_INLINE float4 V_CALL operator/ (float4 a, float b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_div_ps(a.m, _mm_set1_ps(b));
#else
	a.m[0] = a.m[0] / b;
	a.m[1] = a.m[1] / b;
	a.m[2] = a.m[2] / b;
	a.m[3] = a.m[3] / b;
#endif
	return a;
}

VM_INLINE float4 V_CALL operator/ (float a, float4 b)
{
#if YOYO_MATH_SIMD
	b.m = _mm_div_ps(_mm_set1_ps(a), b.m);
#else
	b.m[0] = a / b.m[0];
	b.m[1] = a / b.m[1];
	b.m[2] = a / b.m[2];
	b.m[3] = a / b.m[3];
#endif
	return b;
}

VM_INLINE bool4 V_CALL operator==(float4 a, float4 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmpeq_ps(a.m, b.m);
#else
	//TODO(Ray):What kind of equality should we check here bitwise?
	a.m[0] = a.m[0] == b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] == b.m[1] ? 1 : 0;
	a.m[2] = a.m[2] == b.m[2] ? 1 : 0;
	a.m[3] = a.m[3] == b.m[3] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool4 V_CALL operator!=(float4 a, float4 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmpneq_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] != b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] != b.m[1] ? 1 : 0;
	a.m[2] = a.m[2] != b.m[2] ? 1 : 0;
	a.m[3] = a.m[3] != b.m[3] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool4 V_CALL operator< (float4 a, float4 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmplt_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] < b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] < b.m[1] ? 1 : 0;
	a.m[2] = a.m[2] < b.m[2] ? 1 : 0;
	a.m[3] = a.m[3] < b.m[3] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool4 V_CALL operator> (float4 a, float4 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmpgt_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] > b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] > b.m[1] ? 1 : 0;
	a.m[2] = a.m[2] > b.m[2] ? 1 : 0;
	a.m[3] = a.m[3] > b.m[3] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool4 V_CALL operator<=(float4 a, float4 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmple_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] <= b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] <= b.m[1] ? 1 : 0;
	a.m[2] = a.m[2] <= b.m[2] ? 1 : 0;
	a.m[3] = a.m[3] <= b.m[3] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool4 V_CALL operator>=(float4 a, float4 b)
{
#if YOYO_MATH_SIMD
	a.m = _mm_cmpge_ps(a.m, b.m);
#else
	a.m[0] = a.m[0] >= b.m[0] ? 1 : 0;
	a.m[1] = a.m[1] >= b.m[1] ? 1 : 0;
	a.m[2] = a.m[2] >= b.m[2] ? 1 : 0;
	a.m[3] = a.m[3] >= b.m[3] ? 1 : 0;
#endif
	return a;
}

VM_INLINE bool4 V_CALL operator- (float4 a)
{
#if YOYO_MATH_SIMD
	return float4(_mm_setzero_ps()) - a;
#else
	return float4(0.0f) - a;
#endif

}
//VM_INLINE float4 V_CALL operator+ (float4 a, float4 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
//VM_INLINE float4 V_CALL operator+ (float4 a, float b) { a.m = _mm_add_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float4 V_CALL operator+ (float  a, float4 b) { b.m = _mm_add_ps( _mm_set1_ps(a),b.m); return b; }

//VM_INLINE float4 V_CALL operator- (float4 a, float4 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
//VM_INLINE float4 V_CALL operator- (float4 a, float b) { a.m = _mm_sub_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float4 V_CALL operator- (float  a, float4 b) { b.m = _mm_sub_ps( _mm_set1_ps(a),b.m); return b; }

//VM_INLINE float4 V_CALL operator* (float4 a, float4 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
//VM_INLINE float4 V_CALL operator* (float4 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float4 V_CALL operator* (float a, float4 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }

//VM_INLINE float4 V_CALL operator/ (float4 a, float4 b) { a.m = _mm_div_ps(a.m, b.m); return a; }
//VM_INLINE float4 V_CALL operator/ (float4 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
//VM_INLINE float4 V_CALL operator/ (float a, float4 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }

VM_INLINE float4& V_CALL operator+= (float4 &a, float4 b) { a = a + b; return a; }
VM_INLINE float4& V_CALL operator-= (float4 &a, float4 b) { a = a - b; return a; }

VM_INLINE float4& V_CALL operator*= (float4 &a, float4 b) { a = a * b; return a; }
VM_INLINE float4& V_CALL operator*= (float4 &a, float b) { a = a * b; return a; }

VM_INLINE float4& V_CALL operator/= (float4 &a, float4 b) { a = a / b; return a; }
VM_INLINE float4& V_CALL operator/= (float4 &a, float b) { a = a / b; return a; }

//VM_INLINE bool4 V_CALL operator==(float4 a, float4 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
//VM_INLINE bool4 V_CALL operator!=(float4 a, float4 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
//VM_INLINE bool4 V_CALL operator< (float4 a, float4 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
//VM_INLINE bool4 V_CALL operator> (float4 a, float4 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
//VM_INLINE bool4 V_CALL operator<=(float4 a, float4 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
//VM_INLINE bool4 V_CALL operator>=(float4 a, float4 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
//VM_INLINE float4 V_CALL operator- (float4 a) { return float4(_mm_setzero_ps()) - a; }


#endif///////YOYOYIMPL
/*
//VM_INLINE bool2 V_CALL operator==(float2 a, float2 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
//VM_INLINE bool2 V_CALL operator!=(float2 a, float2 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
//VM_INLINE bool2 V_CALL operator< (float2 a, float2 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
//VM_INLINE bool2 V_CALL operator> (float2 a, float2 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
//VM_INLINE bool2 V_CALL operator<=(float2 a, float2 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
//VM_INLINE bool2 V_CALL operator>=(float2 a, float2 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
//VM_INLINE float2 V_CALL operator- (float2 a) { return float2(_mm_setzero_ps()) - a; }
//VM_INLINE float2 abs(float2 v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }
// Returns a 3-bit code where bit0..bit2 is X..Z

#if 0
VM_INLINE unsigned V_CALL mask(float2 v) { return _mm_movemask_ps(v.m) & 7; }

// Once we have a comparison, we can branch based on its results:
VM_INLINE bool V_CALL any(bool2 v) { return mask(v) != 0; }
VM_INLINE bool V_CALL all(bool2 v) { return mask(v) == 7; }
#endif
*/

#if 0
VM_INLINE unsigned V_CALL mask(float4 v) { return _mm_movemask_ps(v.m) & 7; }
VM_INLINE bool V_CALL any(bool4 v) { return mask(v) != 0; }
VM_INLINE bool V_CALL all(bool4 v) { return mask(v) == 7; }
#endif


//TODO(Ray):To slow later rework this using simd masking and comparisions perhaps will be faster maybe not
VM_INLINE float V_CALL safe_ratio_zero(float a, float b);
VM_INLINE float2 V_CALL safe_ratio2_zero(float2 a, float2 b);
VM_INLINE float3 V_CALL safe_ratio3_zero(float3 a, float3 b);
VM_INLINE float4 V_CALL safe_ratio4_zero(float4 a, float4 b);

//TODO(Ray):To slow later rework this using simd masking and comparisions perhaps will be faster maybe not
VM_INLINE float safe_ratio_one(float a, float b);
VM_INLINE float2 safe_ratio2_one(float2 a, float2 b);
VM_INLINE float3 safe_ratio3_one(float3 a, float3 b);
VM_INLINE float4 safe_ratio4_one(float4 a, float4 b);


//HLSL INT TYPES
typedef uint32_t uint;
struct uint2
{
    uint x;
    uint y;
	VM_INLINE explicit V_CALL uint2(uint a);
	VM_INLINE explicit V_CALL uint2(uint a, uint b, uint c);
    VM_INLINE explicit V_CALL uint2(float2 a);
    VM_INLINE explicit V_CALL uint2(float a,float b);
};

struct uint3
{
    uint x;
    uint y;
    uint z;
	VM_INLINE explicit V_CALL uint3(uint a);
	VM_INLINE explicit V_CALL uint3(uint a,uint b,uint c);
    VM_INLINE explicit V_CALL uint3(float3 a);
    VM_INLINE explicit V_CALL uint3(float a,float b,float c);
};

struct uint4
{
    uint x;
    uint y;
    uint z;
    uint w;
	VM_INLINE explicit V_CALL uint4(uint a);
	VM_INLINE explicit V_CALL uint4(uint a, uint b, uint c,uint d);
    VM_INLINE explicit V_CALL uint4(float4 a);
    VM_INLINE explicit V_CALL uint4(float a,float b,float c,float d);
};

#if 0
VM_INLINE uint2 V_CALL operator & (uint2 lhs, uint2 rhs);
VM_INLINE uint2 V_CALL operator & (uint2 lhs, uint rhs);
VM_INLINE uint2 V_CALL operator & (uint lhs, uint2 rhs);
#endif

VM_INLINE uint3 V_CALL operator & (uint3 lhs, uint3 rhs);
VM_INLINE uint3 V_CALL operator & (uint3 lhs, uint rhs);
VM_INLINE uint3 V_CALL operator & (uint lhs, uint3 rhs);

VM_INLINE uint3 V_CALL operator ^ (uint3 lhs, uint3 rhs);
VM_INLINE uint3 V_CALL operator ^ (uint3 lhs, uint rhs);
VM_INLINE uint3 V_CALL operator ^ (uint lhs, uint3 rhs);

VM_INLINE uint4 V_CALL operator & (uint4 lhs, uint4 rhs);
VM_INLINE uint4 V_CALL operator & (uint4 lhs, uint rhs);
VM_INLINE uint4 V_CALL operator & (uint lhs, uint4 rhs);
VM_INLINE bool2 V_CALL operator > (uint2 lhs, uint2 rhs);
VM_INLINE bool2 V_CALL operator > (uint2 lhs, uint rhs);
VM_INLINE bool2 V_CALL operator > (uint lhs, uint2 rhs);

VM_INLINE bool3 V_CALL operator > (uint3 lhs, uint3 rhs);
VM_INLINE bool3 V_CALL operator > (uint3 lhs, uint rhs);
VM_INLINE bool3 V_CALL operator > (uint lhs, uint3 rhs);

VM_INLINE bool4 V_CALL operator > (uint4 lhs, uint4 rhs);
VM_INLINE bool4 V_CALL operator > (uint4 lhs, uint rhs);
VM_INLINE bool4 V_CALL operator > (uint lhs, uint4 rhs);

#if 0
//HLSL Functions
VM_INLINE float2 V_CALL minimum(float2 a, float2 b);
VM_INLINE float2 V_CALL maximum(float2 a, float2 b);

VM_INLINE float3 V_CALL minimum(float3 a, float3 b);
VM_INLINE float3 V_CALL maximum(float3 a, float3 b);


VM_INLINE float V_CALL hmin(float3 v);
VM_INLINE float V_CALL hmax(float3 v);
VM_INLINE float4 V_CALL minimum(float4 a, float4 b);
VM_INLINE float4 V_CALL maximum(float4 a, float4 b);
#endif

#ifdef YOYOIMPL

//TODO(Ray):To slow later rework this using simd masking and comparisions perhaps will be faster maybe not
VM_INLINE float V_CALL safe_ratio_zero(float a, float b) { if (a == 0.0f || b == 0.0f) { return 0.0f; } else { return a / b; } }
VM_INLINE float2 V_CALL safe_ratio2_zero(float2 a, float2 b) { a.setX(safe_ratio_zero(a.x(), b.x())); a.setY(safe_ratio_zero(a.y(), b.y())); return a; }
VM_INLINE float3 V_CALL safe_ratio3_zero(float3 a, float3 b) { float2 thea = safe_ratio2_zero(a.xy(), b.xy()); float theab = safe_ratio_zero(a.z(), b.z()); return float3(thea, theab); }
VM_INLINE float4 V_CALL safe_ratio4_zero(float4 a, float4 b) { float3 thea = safe_ratio3_zero(a.xyz(), b.xyz()); float w = safe_ratio_zero(a.w(), b.w()); return float4(thea, w); }

//TODO(Ray):To slow later rework this using simd masking and comparisions perhaps will be faster maybe not
VM_INLINE float safe_ratio_one(float a, float b) { if (a == 0.0f || b == 0.0f) { return 1.0f; } else { return a / b; } }
VM_INLINE float2 safe_ratio2_one(float2 a, float2 b) { a.setX(safe_ratio_one(a.x(), b.x())); a.setY(safe_ratio_one(a.y(), b.y())); return a; }
VM_INLINE float3 safe_ratio3_one(float3 a, float3 b) { float2 thea = safe_ratio2_one(a.xy(), b.xy()); float theab = safe_ratio_one(a.z(), b.z()); return float3(thea, theab); }
VM_INLINE float4 safe_ratio4_one(float4 a, float4 b) { float3 thea = safe_ratio3_one(a.xyz(), b.xyz()); float w = safe_ratio_one(a.w(), b.w()); return float4(thea, w); }

VM_INLINE  V_CALL uint2::uint2(uint a) { this->x = a; this->y = a;}
VM_INLINE  V_CALL uint2::uint2(uint a, uint b, uint c) { this->x = a; this->y = b; }
VM_INLINE  V_CALL uint2::uint2(float2 a){this->x = (uint)a.x();this->y = (uint)a.y();}
VM_INLINE  V_CALL uint2::uint2(float a,float b){this->x = (uint)a;this->y = (uint)b;}

VM_INLINE  V_CALL uint3::uint3(uint a) { this->x = a; this->y = a; this->z = a; }
VM_INLINE  V_CALL uint3::uint3(uint a,uint b,uint c) { this->x = a; this->y = b; this->z = c; }
VM_INLINE  V_CALL uint3::uint3(float3 a){this->x = (uint)a.x();this->y = (uint)a.y();this->z = (uint)a.z();}
VM_INLINE  V_CALL uint3::uint3(float a,float b,float c){this->x = (uint)a;this->y = (uint)b;this->z = (uint)c;}

VM_INLINE  V_CALL uint4::uint4(uint a) { this->x = a; this->y = a; this->z = a; this->w = a; }
VM_INLINE  V_CALL uint4::uint4(uint a, uint b, uint c,uint d) { this->x = a; this->y = b; this->z = c; this->w = d; }
VM_INLINE  V_CALL uint4::uint4(float4 a){this->x = (uint)a.x();this->y = (uint)a.y();this->z = (uint)a.z();this->w = (uint)a.w();}
VM_INLINE  V_CALL uint4::uint4(float a,float b,float c,float d){this->x = (uint)a;this->y = (uint)b;this->z = (uint)c;this->w = (uint)d;}

#if 0
VM_INLINE uint2 V_CALL operator & (uint2 lhs, uint2 rhs) { return uint2 (lhs.x & rhs.x, lhs.y & rhs.y); }
VM_INLINE uint2 V_CALL operator & (uint2 lhs, uint rhs)  { return uint2 (lhs.x & rhs, lhs.y & rhs); }
VM_INLINE uint2 V_CALL operator & (uint lhs, uint2 rhs)  { return uint2 (lhs & rhs.x, lhs & rhs.y); }
#endif

VM_INLINE uint3 V_CALL operator & (uint3 lhs, uint3 rhs) { return uint3 (lhs.x & rhs.x, lhs.y & rhs.y,lhs.z & rhs.z); }
VM_INLINE uint3 V_CALL operator & (uint3 lhs, uint rhs)  { return uint3 (lhs.x & rhs, lhs.y & rhs,lhs.z & rhs); }
VM_INLINE uint3 V_CALL operator & (uint lhs, uint3 rhs)  { return uint3 (lhs & rhs.x, lhs & rhs.y,lhs & rhs.z); }

VM_INLINE uint3 V_CALL operator ^ (uint3 lhs, uint3 rhs) { return uint3(lhs.x ^ rhs.x, lhs.y ^ rhs.y, lhs.z ^ rhs.z); }
VM_INLINE uint3 V_CALL operator ^ (uint3 lhs, uint rhs) { return uint3(lhs.x ^ rhs, lhs.y ^ rhs, lhs.z ^ rhs); }
VM_INLINE uint3 V_CALL operator ^ (uint lhs, uint3 rhs) { return uint3(lhs ^ rhs.x, lhs ^ rhs.y, lhs ^ rhs.z); }

VM_INLINE uint4 V_CALL operator & (uint4 lhs, uint4 rhs) { return uint4 (lhs.x & rhs.x, lhs.y & rhs.y,lhs.z & rhs.z,lhs.w & rhs.w); }
VM_INLINE uint4 V_CALL operator & (uint4 lhs, uint rhs)  { return uint4 (lhs.x & rhs, lhs.y & rhs,lhs.z & rhs,lhs.w & rhs); }
VM_INLINE uint4 V_CALL operator & (uint lhs, uint4 rhs)  { return uint4 (lhs & rhs.x, lhs & rhs.y,lhs & rhs.z,lhs & rhs.w); }
VM_INLINE bool2 V_CALL operator > (uint2 lhs, uint2 rhs) { return bool2 (lhs.x > rhs.x, lhs.y > rhs.y); }
VM_INLINE bool2 V_CALL operator > (uint2 lhs, uint rhs)  { return bool2 (lhs.x > rhs, lhs.y > rhs); }
VM_INLINE bool2 V_CALL operator > (uint lhs, uint2 rhs)  { return bool2 (lhs > rhs.x, lhs > rhs.y); }

VM_INLINE bool3 V_CALL operator > (uint3 lhs, uint3 rhs) { return bool3 (lhs.x > rhs.x, lhs.y > rhs.y,lhs.z > rhs.z); }
VM_INLINE bool3 V_CALL operator > (uint3 lhs, uint rhs)  { return bool3 (lhs.x > rhs, lhs.y > rhs,lhs.z > rhs); }
VM_INLINE bool3 V_CALL operator > (uint lhs, uint3 rhs)  { return bool3 (lhs > rhs.x, lhs > rhs.y,lhs > rhs.z); }

VM_INLINE bool4 V_CALL operator > (uint4 lhs, uint4 rhs) { return bool4 (lhs.x > rhs.x, lhs.y > rhs.y,lhs.z > rhs.z,lhs.w > rhs.w); }
VM_INLINE bool4 V_CALL operator > (uint4 lhs, uint rhs)  { return bool4 (lhs.x > rhs, lhs.y > rhs,lhs.z > rhs, lhs.w > rhs); }
VM_INLINE bool4 V_CALL operator > (uint lhs, uint4 rhs)  { return bool4 (lhs > rhs.x, lhs > rhs.y,lhs > rhs.z,lhs > rhs.w); }


#if 0
//HLSL Functions
VM_INLINE float2 V_CALL minimum(float2 a, float2 b) { a.m = _mm_min_ps(a.m, b.m); return a; }
VM_INLINE float2 V_CALL maximum(float2 a, float2 b) { a.m = _mm_max_ps(a.m, b.m); return a; }

VM_INLINE float3 V_CALL minimum(float3 a, float3 b) { a.m = _mm_min_ps(a.m, b.m); return a; }
VM_INLINE float3 V_CALL maximum(float3 a, float3 b) { a.m = _mm_max_ps(a.m, b.m); return a; }


VM_INLINE float V_CALL hmin(float3 v)
{
    v = minimum(v, SHUFFLE3(v, 1, 0, 2));
    return minimum(v, SHUFFLE3(v, 2, 0, 1)).x();
}
VM_INLINE float V_CALL hmax(float3 v)
{
    v = maximum(v, SHUFFLE3(v, 1, 0, 2));
    return maximum(v, SHUFFLE3(v, 2, 0, 1)).x();
}

VM_INLINE float4 V_CALL minimum(float4 a, float4 b) { a.m = _mm_min_ps(a.m, b.m); return a; }
VM_INLINE float4 V_CALL maximum(float4 a, float4 b) { a.m = _mm_max_ps(a.m, b.m); return a; }
#endif

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VM_INLINE float3 V_CALL cross(float3 a, float3 b);

//NOTE(Ray):using fmin here reevaluate ...
VM_INLINE float V_CALL sum(float2 v);
VM_INLINE float V_CALL sum(float3 v);
VM_INLINE float V_CALL sum(float4 v);

VM_INLINE float V_CALL dot(float2 a, float2 b);
VM_INLINE float V_CALL dot(float3 a, float3 b);
VM_INLINE float V_CALL dot(float4 a, float4 b);

VM_INLINE float V_CALL length(float2 v);
VM_INLINE float V_CALL length(float3 v);
VM_INLINE float V_CALL length(float4 v);

VM_INLINE float2 V_CALL normalize(float2 v);
VM_INLINE float3 V_CALL normalize(float3 v);
VM_INLINE float4 V_CALL normalize(float4 v);

VM_INLINE float  V_CALL clamp(float  x, float  a, float  b);
#if 0
VM_INLINE float2 V_CALL clamp(float2 t, float2 a, float2 b);
VM_INLINE float3 V_CALL clamp(float3 t, float3 a, float3 b);
VM_INLINE float4 V_CALL clamp(float4 t, float4 a, float4 b);
#endif

VM_INLINE float V_CALL lengthSq(float2 v);
VM_INLINE float V_CALL lengthSq(float3 v);
VM_INLINE float V_CALL lengthSq(float4 v);

VM_INLINE float V_CALL  lerp(float  a, float  b, float t);
VM_INLINE float2 V_CALL lerp(float2 a, float2 b, float t);
VM_INLINE float3 V_CALL lerp(float3 a, float3 b, float t);
VM_INLINE float4 V_CALL lerp(float4 a, float4 b, float t);

VM_INLINE float V_CALL distance(float3 a, float3 b);
VM_INLINE float V_CALL power(float a, float p);

VM_INLINE float V_CALL  sine(float  x);
VM_INLINE float2 V_CALL sine(float2 x);
VM_INLINE float3 V_CALL sine(float3 x);
VM_INLINE float4 V_CALL sine(float4 x);

VM_INLINE float V_CALL  cosine(float  x);
VM_INLINE float2 V_CALL cosine(float2 x);
VM_INLINE float3 V_CALL cosine(float3 x);
VM_INLINE float4 V_CALL cosine(float4 x);

VM_INLINE void V_CALL sincos(float  x,float*  s,float*  c);
VM_INLINE void V_CALL sincos(float2 x,float2* s,float2* c);
VM_INLINE void V_CALL sincos(float3 x,float3* s,float3* c);
VM_INLINE void V_CALL sincos(float4 x,float4* s,float4* c);

VM_INLINE float  V_CALL sqroot(float a);;
VM_INLINE float2 V_CALL sqroot(float2 a);
VM_INLINE float3 V_CALL sqroot(float3 a);
VM_INLINE float4 V_CALL sqroot(float4 a);

VM_INLINE float V_CALL  rsqrt(float a);
VM_INLINE float2 V_CALL rsqrt(float2 a);
VM_INLINE float3 V_CALL rsqrt(float3 a);
VM_INLINE float4 V_CALL rsqrt(float4 a);

VM_INLINE float V_CALL  abso(float x);
VM_INLINE float2 V_CALL abso(float2 x);
VM_INLINE float3 V_CALL abso(float3 x);
VM_INLINE float4 V_CALL abso(float4 x);

//NOTE(Ray);:Are these really useful.  Shoul return bool answer for each component.
VM_INLINE bool V_CALL  isfinite(float x);
VM_INLINE bool2 V_CALL isfinite(float2 x);
VM_INLINE bool3 V_CALL isfinite(float3 x);
VM_INLINE bool4 V_CALL isfinite(float4 x);

VM_INLINE bool  V_CALL isinf(float x);
VM_INLINE bool2 V_CALL isinf(float2 x);
VM_INLINE bool3 V_CALL isinf(float3 x);
VM_INLINE bool4 V_CALL isinf(float4 x);

VM_INLINE uint  V_CALL asuint(float x);
VM_INLINE uint2 V_CALL asuint(float2 x);
VM_INLINE uint3 V_CALL asuint(float3 x);
VM_INLINE uint4 V_CALL asuint(float4 x);

VM_INLINE float  V_CALL asfloat(uint x);
VM_INLINE float2 V_CALL asfloat(uint2 x);
VM_INLINE float3 V_CALL asfloat(uint3 x);
VM_INLINE float4 V_CALL asfloat(uint4 x);

VM_INLINE bool  V_CALL isnan(float x);

#if 0
VM_INLINE bool2 V_CALL isnan(float2 x);
#endif

VM_INLINE bool3 V_CALL isnan(float3 x);
VM_INLINE bool4 V_CALL isnan(float4 x);

VM_INLINE float2 V_CALL lerp(float2 x, float2 y, float2 s);
VM_INLINE float3 V_CALL lerp(float3 x, float3 y, float3 s);
VM_INLINE float4 V_CALL lerp(float4 x, float4 y, float4 s);

VM_INLINE float  V_CALL unlerp(float  a, float  b, float  x);
VM_INLINE float2 V_CALL unlerp(float2 a, float2 b, float2 x);
VM_INLINE float3 V_CALL unlerp(float3 a, float3 b, float3 x);
VM_INLINE float4 V_CALL unlerp(float4 a, float4 b, float4 x);

VM_INLINE float  V_CALL remap(float  a, float  b, float  c, float  d, float  x);
VM_INLINE float2 V_CALL remap(float2 a, float2 b, float2 c, float2 d, float2 x);
VM_INLINE float3 V_CALL remap(float3 a, float3 b, float3 c, float3 d, float3 x);
VM_INLINE float4 V_CALL remap(float4 a, float4 b, float4 c, float4 d, float4 x);

VM_INLINE float  V_CALL mad(float  a, float  b, float  c);
VM_INLINE float2 V_CALL mad(float2 a, float2 b, float2 c);
VM_INLINE float3 V_CALL mad(float3 a, float3 b, float3 c);
VM_INLINE float4 V_CALL mad(float4 a, float4 b, float4 c);

VM_INLINE float  V_CALL saturate(float  x);

#if 0
VM_INLINE float2 V_CALL saturate(float2 x);
VM_INLINE float3 V_CALL saturate(float3 x);
VM_INLINE float4 V_CALL saturate(float4 x);
#endif

VM_INLINE float  V_CALL tangent(float  x);
VM_INLINE float2 V_CALL tangent(float2 x);
VM_INLINE float3 V_CALL tangent(float3 x);
VM_INLINE float4 V_CALL tangent(float4 x);

VM_INLINE float  V_CALL tanhy(float  x);
VM_INLINE float2 V_CALL tanhy(float2 x);
VM_INLINE float3 V_CALL tanhy(float3 x);
VM_INLINE float4 V_CALL tanhy(float4 x);

//VM_INLINE float  atan(float  x); noexcept { return (float);atan(x);; }
VM_INLINE float2 V_CALL atan(float2 x);
VM_INLINE float3 V_CALL atan(float3 x);
VM_INLINE float4 V_CALL atan(float4 x);

//VM_INLINE float  atan2(float y,  float x); noexcept  { return (float);atan2(y, x);; }
VM_INLINE float2 V_CALL atan2(float2 y, float2 x);
VM_INLINE float3 V_CALL atan2(float3 y, float3 x);
VM_INLINE float4 V_CALL atan2(float4 y, float4 x);

//VM_INLINE float  cos(float  x); noexcept { return cos(x);; }
VM_INLINE float2 V_CALL cos(float2 x);
VM_INLINE float3 V_CALL cos(float3 x);
VM_INLINE float4 V_CALL cos(float4 x);

//VM_INLINE float  cosh(float  x); noexcept { return (float);cosh(x);; }
VM_INLINE float2 V_CALL cosh(float2 x);
VM_INLINE float3 V_CALL cosh(float3 x);
VM_INLINE float4 V_CALL cosh(float4 x);

//VM_INLINE float  acos(float  x); noexcept { return (float);acos((float);x);; }
VM_INLINE float2 V_CALL acos(float2 x);
VM_INLINE float3 V_CALL acos(float3 x);
VM_INLINE float4 V_CALL acos(float4 x);

//VM_INLINE float  sin(float  x); noexcept { return (float);sin((float);x);; }
VM_INLINE float2 V_CALL sin(float2 x);
VM_INLINE float3 V_CALL sin(float3 x);
VM_INLINE float4 V_CALL sin(float4 x);

//VM_INLINE float  sinh(float  x); { return (float);sinh((float);x);; }
VM_INLINE float2 V_CALL sinh(float2 x);
VM_INLINE float3 V_CALL sinh(float3 x);
VM_INLINE float4 V_CALL sinh(float4 x);

//VM_INLINE float  asin(float x);  { return (float);asin((float);x);; }
VM_INLINE float2 V_CALL asin(float2 x);
VM_INLINE float3 V_CALL asin(float3 x);
VM_INLINE float4 V_CALL asin(float4 x);

//VM_INLINE float floor(float x); { return (float);floor((float);x);; }
VM_INLINE float2 V_CALL floor(float2 x);
VM_INLINE float3 V_CALL floor(float3 x);
VM_INLINE float4 V_CALL floor(float4 x);

//VM_INLINE float ceil(float x); { return (float);ceil((float);x);; }
VM_INLINE float2 V_CALL ceil(float2 x);
VM_INLINE float3 V_CALL ceil(float3 x);
VM_INLINE float4 V_CALL ceil(float4 x);

//VM_INLINE float  round(float x); { return (float);round((float);x);; }
VM_INLINE float2 V_CALL round(float2 x);
VM_INLINE float3 V_CALL round(float3 x);
VM_INLINE float4 V_CALL round(float4 x);

//VM_INLINE float trunc(float x); { return (float);trunc((float);x);; }
VM_INLINE float2 V_CALL trunc(float2 x);
VM_INLINE float3 V_CALL trunc(float3 x);
VM_INLINE float4 V_CALL trunc(float4 x);

VM_INLINE float V_CALL frac(float x);
VM_INLINE float2 V_CALL frac(float2 x);
VM_INLINE float3 V_CALL frac(float3 x);
VM_INLINE float4 V_CALL frac(float4 x);

VM_INLINE float V_CALL rcp(float x);
VM_INLINE float2 V_CALL rcp(float2 x);
VM_INLINE float3 V_CALL rcp(float3 x);
VM_INLINE float4 V_CALL rcp(float4 x);

VM_INLINE float V_CALL sign(float x);
VM_INLINE float2 V_CALL sign(float2 x);
VM_INLINE float3 V_CALL sign(float3 x);
VM_INLINE float4 V_CALL sign(float4 x);

//VM_INLINE float pow(float x, float y); { return (float);pow((float);x, (float);y);; }
VM_INLINE float2 V_CALL pow(float2 x, float2 y);
VM_INLINE float3 V_CALL pow(float3 x, float3 y);
VM_INLINE float4 V_CALL pow(float4 x, float4 y);

//VM_INLINE float exp(float x); { return (float);exp((float);x);; }
VM_INLINE float2 V_CALL exp(float2 x);
VM_INLINE float3 V_CALL exp(float3 x);
VM_INLINE float4 V_CALL exp(float4 x);

//VM_INLINE float exp2(float x); { return (float);pow(2.0f, (float);x);; }
VM_INLINE float2 V_CALL exp2(float2 x);
VM_INLINE float3 V_CALL exp2(float3 x);
VM_INLINE float4 V_CALL exp2(float4 x);


//VM_INLINE float fmod(float x, float y); { return fmod(x,y);; }
VM_INLINE float2 V_CALL fmod(float2 x, float2 y);
VM_INLINE float3 V_CALL fmod(float3 x, float3 y);
VM_INLINE float4 V_CALL fmod(float4 x, float4 y);

VM_INLINE float V_CALL distance(float x, float y);
VM_INLINE float V_CALL distance(float2 x, float2 y);

VM_INLINE float V_CALL distance(float4 x, float4 y);

VM_INLINE float V_CALL smoothstep(float a, float b, float x);
#if 0
VM_INLINE float2 V_CALL smoothstep(float2 a, float2 b, float2 x);
VM_INLINE float3 V_CALL smoothstep(float3 a, float3 b, float3 x);
VM_INLINE float4 V_CALL smoothstep(float4 a, float4 b, float4 x);
#endif


VM_INLINE float  V_CALL select(float  a, float  b, bool c);
VM_INLINE float2 V_CALL select(float2 a, float2 b, bool c);
VM_INLINE float3 V_CALL select(float3 a, float3 b, bool c);
VM_INLINE float4 V_CALL select(float4 a, float4 b, bool c);

VM_INLINE float2 V_CALL reflect(float2 i, float2 n);
VM_INLINE float3 V_CALL reflect(float3 i, float3 n);
VM_INLINE float4 V_CALL reflect(float4 i, float4 n);

VM_INLINE float2 V_CALL refract(float2 i, float2 n, float eta);
VM_INLINE float3 V_CALL refract(float3 i, float3 n, float eta);
VM_INLINE float4 V_CALL refract(float4 i, float4 n, float eta);

VM_INLINE float V_CALL radians(float x);
VM_INLINE float2 V_CALL radians(float2 x);
VM_INLINE float3 V_CALL radians(float3 x);
VM_INLINE float4 V_CALL radians(float4 x);

VM_INLINE float V_CALL degrees(float x);
VM_INLINE float2 V_CALL degrees(float2 x);
VM_INLINE float3 V_CALL degrees(float3 x);
VM_INLINE float4 V_CALL degrees(float4 x);

#ifdef YOYOIMPL

VM_INLINE float3 V_CALL cross(float3 a, float3 b)
{
	return float3(a.y() * b.z() - a.z() * b.y(),
		          a.z() * b.x() - a.x() * b.z(),
		          a.x() * b.y() - a.y() * b.x());
}

//NOTE(Ray):using fmin here reevaluate ...
VM_INLINE float V_CALL sum(float2 v) { return v.x() + v.y();}
VM_INLINE float V_CALL sum(float3 v) { return v.x() + v.y() + v.z(); }
VM_INLINE float V_CALL sum(float4 v) { return v.x() + v.y() + v.z(); }

VM_INLINE float V_CALL dot(float2 a, float2 b) { return sum(a*b); }
VM_INLINE float V_CALL dot(float3 a, float3 b) { return sum(a*b); }
VM_INLINE float V_CALL dot(float4 a, float4 b) { return sum(a*b); }

VM_INLINE float V_CALL length(float2 v) { return sqrtf(dot(v, v)); }
VM_INLINE float V_CALL length(float3 v) { return sqrtf(dot(v, v)); }
VM_INLINE float V_CALL length(float4 v) { return sqrtf(dot(v, v)); }

VM_INLINE float2 V_CALL normalize(float2 v) { return v * (safe_ratio_zero(1.0f , length(v))); }
VM_INLINE float3 V_CALL normalize(float3 v) { return v * (safe_ratio_zero(1.0f , length(v))); }
VM_INLINE float4 V_CALL normalize(float4 v) { return v * (safe_ratio_zero(1.0f , length(v))); }

VM_INLINE float  V_CALL clamp(float  x, float  a, float  b) { return fmax(a, fmin(b, x)); }
#if 0
VM_INLINE float2 V_CALL clamp(float2 t, float2 a, float2 b) { return minimum(maximum(t, a), b); }
VM_INLINE float3 V_CALL clamp(float3 t, float3 a, float3 b) { return minimum(maximum(t, a), b); }
VM_INLINE float4 V_CALL clamp(float4 t, float4 a, float4 b) { return minimum(maximum(t, a), b); }
#endif

VM_INLINE float V_CALL lengthSq(float2 v) { return dot(v, v); }
VM_INLINE float V_CALL lengthSq(float3 v) { return dot(v, v); }
VM_INLINE float V_CALL lengthSq(float4 v) { return dot(v, v); }

VM_INLINE float V_CALL  lerp(float  a, float  b, float t) { return a + (b-a)*t; }
VM_INLINE float2 V_CALL lerp(float2 a, float2 b, float t) { return a + (b-a)*t; }
VM_INLINE float3 V_CALL lerp(float3 a, float3 b, float t) { return a + (b-a)*t; }
VM_INLINE float4 V_CALL lerp(float4 a, float4 b, float t) { return a + (b-a)*t; }

VM_INLINE float V_CALL distance(float3 a, float3 b) { return length(b - a); }
VM_INLINE float V_CALL power(float a, float p) { return pow(a, p); }

VM_INLINE float V_CALL  sine(float  x) { return sinf(x); }
VM_INLINE float2 V_CALL sine(float2 x) { return float2(sinf(x.x()), sinf(x.y())); }
VM_INLINE float3 V_CALL sine(float3 x) { return float3(sinf(x.x()), sinf(x.y()), sinf(x.z())); }
VM_INLINE float4 V_CALL sine(float4 x) { return float4(sinf(x.x()), sinf(x.y()), sinf(x.z()), sinf(x.w())); }

VM_INLINE float V_CALL  cosine(float  x) { return cosf(x); }
VM_INLINE float2 V_CALL cosine(float2 x) { return float2(cosf(x.x()), cosf(x.y())); }
VM_INLINE float3 V_CALL cosine(float3 x) { return float3(cosf(x.x()), cosf(x.y()), cosf(x.z())); }
VM_INLINE float4 V_CALL cosine(float4 x) { return float4(cosf(x.x()), cosf(x.y()), cosf(x.z()), cosf(x.w())); }

VM_INLINE void V_CALL sincos(float  x,float*  s,float*  c) { *s = sin(x); *c = cos(x); }
VM_INLINE void V_CALL sincos(float2 x,float2* s,float2* c) { *s = sine(x); *c = cosine(x); }
VM_INLINE void V_CALL sincos(float3 x,float3* s,float3* c) { *s = sine(x); *c = cosine(x); }
VM_INLINE void V_CALL sincos(float4 x,float4* s,float4* c) { *s = sine(x); *c = cosine(x); }

VM_INLINE float  V_CALL sqroot(float a)  { return 1.0f / sqrt(a); }
VM_INLINE float2 V_CALL sqroot(float2 a) { return float2(sqrt(a.x()),sqrt(a.y())); }
VM_INLINE float3 V_CALL sqroot(float3 a) { return float3(sqrt(a.x()),sqrt(a.y()),sqrt(a.z())); }
VM_INLINE float4 V_CALL sqroot(float4 a) { return float4(sqrt(a.x()),sqrt(a.y()),sqrt(a.z()),sqrt(a.w())); }

VM_INLINE float V_CALL  rsqrt(float a)  { return safe_ratio_zero(1.0f , sqrt(a)); }
VM_INLINE float2 V_CALL rsqrt(float2 a) { return 1.0f / sqroot(a); }
VM_INLINE float3 V_CALL rsqrt(float3 a) { return 1.0f / sqroot(a); }
VM_INLINE float4 V_CALL rsqrt(float4 a) { return 1.0f / sqroot(a); }

VM_INLINE float V_CALL  abso(float x){return abs(x);};
VM_INLINE float2 V_CALL abso(float2 x){return float2(abs(x.x()),abs(x.y()));}
VM_INLINE float3 V_CALL abso(float3 x){return float3(abs(x.x()),abs(x.y()),abs(x.z()));}
VM_INLINE float4 V_CALL abso(float4 x){return float4(abs(x.x()),abs(x.y()),abs(x.z()),abs(x.w()));}

//NOTE(Ray):Are these really useful.  Shoul return bool answer for each component.
VM_INLINE bool V_CALL  isfinite(float x)  { return abso(x) < POSITIVE_INFINITY; }
VM_INLINE bool2 V_CALL isfinite(float2 x) { return abso(x) < POSITIVE_INFINITY2; }
VM_INLINE bool3 V_CALL isfinite(float3 x) { return abso(x) < POSITIVE_INFINITY3; }
VM_INLINE bool4 V_CALL isfinite(float4 x) { return abso(x) < POSITIVE_INFINITY4; }

VM_INLINE bool  V_CALL isinf(float x)  { return abso(x) == POSITIVE_INFINITY; }
VM_INLINE bool2 V_CALL isinf(float2 x) { return abso(x) == POSITIVE_INFINITY2; }
VM_INLINE bool3 V_CALL isinf(float3 x) { return abso(x) == POSITIVE_INFINITY3; }
VM_INLINE bool4 V_CALL isinf(float4 x) { return abso(x) == POSITIVE_INFINITY4; }

VM_INLINE uint  V_CALL asuint(float x)  { return (uint)x; }
VM_INLINE uint2 V_CALL asuint(float2 x) { return uint2((uint)x.x(), (uint)x.y()); }
VM_INLINE uint3 V_CALL asuint(float3 x) { return uint3((uint)x.x(), (uint)x.y(), (uint)x.z()); }
VM_INLINE uint4 V_CALL asuint(float4 x) { return uint4((uint)x.x(), (uint)x.y(), (uint)x.z(), (uint)x.w()); }

VM_INLINE float  V_CALL asfloat(uint x) { return (float)x; }
VM_INLINE float2 V_CALL asfloat(uint2 x) { return float2((float)x.x, (float)x.y); }
VM_INLINE float3 V_CALL asfloat(uint3 x) { return float3((float)x.x, (float)x.y, (float)x.z); }
VM_INLINE float4 V_CALL asfloat(uint4 x) { return float4((float)x.x, (float)x.y, (float)x.z, (float)x.w); }

VM_INLINE bool  V_CALL isnan(float x)  { return (asuint(x) & 0x7FFFFFFF) > 0x7F800000; }

#if 0
VM_INLINE bool2 V_CALL isnan(float2 x) { return (asuint(x) & 0x7FFFFFFF) > 0x7F800000; }
#endif

VM_INLINE bool3 V_CALL isnan(float3 x) { return (asuint(x) & 0x7FFFFFFF) > 0x7F800000; }
VM_INLINE bool4 V_CALL isnan(float4 x) { return (asuint(x) & 0x7FFFFFFF) > 0x7F800000; }

VM_INLINE float2 V_CALL lerp(float2 x, float2 y, float2 s) { return x + s * (y - x); }
VM_INLINE float3 V_CALL lerp(float3 x, float3 y, float3 s) { return x + s * (y - x); }
VM_INLINE float4 V_CALL lerp(float4 x, float4 y, float4 s) { return x + s * (y - x); }

VM_INLINE float  V_CALL unlerp(float  a, float  b, float  x) { return (x - a) / (b - a); }
VM_INLINE float2 V_CALL unlerp(float2 a, float2 b, float2 x) { return (x - a) / (b - a); }
VM_INLINE float3 V_CALL unlerp(float3 a, float3 b, float3 x) { return (x - a) / (b - a); }
VM_INLINE float4 V_CALL unlerp(float4 a, float4 b, float4 x) { return (x - a) / (b - a); }

VM_INLINE float  V_CALL remap(float  a, float  b, float  c, float  d, float  x)  { return lerp(c, d, unlerp(a, b, x)); }
VM_INLINE float2 V_CALL remap(float2 a, float2 b, float2 c, float2 d, float2 x) { return lerp(c, d, unlerp(a, b, x)); }
VM_INLINE float3 V_CALL remap(float3 a, float3 b, float3 c, float3 d, float3 x) { return lerp(c, d, unlerp(a, b, x)); }
VM_INLINE float4 V_CALL remap(float4 a, float4 b, float4 c, float4 d, float4 x) { return lerp(c, d, unlerp(a, b, x)); }

VM_INLINE float  V_CALL mad(float  a, float  b, float  c) { return a * b + c; }
VM_INLINE float2 V_CALL mad(float2 a, float2 b, float2 c) { return a * b + c; }
VM_INLINE float3 V_CALL mad(float3 a, float3 b, float3 c) { return a * b + c; }
VM_INLINE float4 V_CALL mad(float4 a, float4 b, float4 c) { return a * b + c; }

VM_INLINE float  V_CALL saturate(float  x) { return clamp(x, 0.0f, 1.0f); }

#if 0
VM_INLINE float2 V_CALL saturate(float2 x) { return clamp(x, float2(0.0f), float2(1.0f)); }
VM_INLINE float3 V_CALL saturate(float3 x) { return clamp(x, float3(0.0f), float3(1.0f)); }
VM_INLINE float4 V_CALL saturate(float4 x) { return clamp(x, float4(0.0f), float4(1.0f)); }
#endif

VM_INLINE float  V_CALL tangent(float  x) { return (float)tan(x); }
VM_INLINE float2 V_CALL tangent(float2 x) { return float2(tangent(x.x()), tangent(x.y())); }
VM_INLINE float3 V_CALL tangent(float3 x) { return float3(tangent(x.x()), tangent(x.y()), tangent(x.z())); }
VM_INLINE float4 V_CALL tangent(float4 x) { return float4(tangent(x.x()), tangent(x.y()), tangent(x.z()), tangent(x.w())); }

VM_INLINE float  V_CALL tanhy(float  x) { return (float)tan(x); }
VM_INLINE float2 V_CALL tanhy(float2 x) { return float2(tanh(x.x()), tanh(x.y())); }
VM_INLINE float3 V_CALL tanhy(float3 x) { return float3(tanh(x.x()), tanh(x.y()), tanh(x.z())); }
VM_INLINE float4 V_CALL tanhy(float4 x) { return float4(tanh(x.x()), tanh(x.y()), tanh(x.z()), tanh(x.w())); }

//VM_INLINE float  atan(float  x) noexcept { return (float)atan(x); }
VM_INLINE float2 V_CALL atan(float2 x) { return float2(atan(x.x()), atan(x.y())); }
VM_INLINE float3 V_CALL atan(float3 x) { return float3(atan(x.x()), atan(x.y()), atan(x.z())); }
VM_INLINE float4 V_CALL atan(float4 x) { return float4(atan(x.x()), atan(x.y()), atan(x.z()), atan(x.w())); }

//VM_INLINE float  atan2(float y,  float x) noexcept  { return (float)atan2(y, x); }
VM_INLINE float2 V_CALL atan2(float2 y, float2 x) { return float2(atan2(y.x(), x.x()), atan2(y.y(), x.y())); }
VM_INLINE float3 V_CALL atan2(float3 y, float3 x) { return float3(atan2(y.x(), x.x()), atan2(y.y(), x.y()), atan2(y.z(), x.z())); }
VM_INLINE float4 V_CALL atan2(float4 y, float4 x) { return float4(atan2(y.x(), x.x()), atan2(y.y(), x.y()), atan2(y.z(), x.z()), atan2(y.w(), x.w())); }

//VM_INLINE float  cos(float  x) noexcept { return cos(x); }
VM_INLINE float2 V_CALL cos(float2 x) { return float2(cos(x.x()), cos(x.y())); }
VM_INLINE float3 V_CALL cos(float3 x) { return float3(cos(x.x()), cos(x.y()), cos(x.z())); }
VM_INLINE float4 V_CALL cos(float4 x) { return float4(cos(x.x()), cos(x.y()), cos(x.z()), cos(x.w())); }

//VM_INLINE float  cosh(float  x) noexcept { return (float)cosh(x); }
VM_INLINE float2 V_CALL cosh(float2 x) { return float2(cosh(x.x()), cosh(x.y())); }
VM_INLINE float3 V_CALL cosh(float3 x) { return float3(cosh(x.x()), cosh(x.y()), cosh(x.z())); }
VM_INLINE float4 V_CALL cosh(float4 x) { return float4(cosh(x.x()), cosh(x.y()), cosh(x.z()), cosh(x.w())); }

//VM_INLINE float  acos(float  x) noexcept { return (float)acos((float)x); }
VM_INLINE float2 V_CALL acos(float2 x) { return float2(acos(x.x()), acos(x.y())); }
VM_INLINE float3 V_CALL acos(float3 x) { return float3(acos(x.x()), acos(x.y()), acos(x.z())); }
VM_INLINE float4 V_CALL acos(float4 x) { return float4(acos(x.x()), acos(x.y()), acos(x.z()), acos(x.w())); }

//VM_INLINE float  sin(float  x) noexcept { return (float)sin((float)x); }
VM_INLINE float2 V_CALL sin(float2 x) { return float2(sin(x.x()), sin(x.y())); }
VM_INLINE float3 V_CALL sin(float3 x) { return float3(sin(x.x()), sin(x.y()), sin(x.z())); }
VM_INLINE float4 V_CALL sin(float4 x) { return float4(sin(x.x()), sin(x.y()), sin(x.z()), sin(x.w())); }

//VM_INLINE float  sinh(float  x) { return (float)sinh((float)x); }
VM_INLINE float2 V_CALL sinh(float2 x) { return float2(sinh(x.x()), sinh(x.y())); }
VM_INLINE float3 V_CALL sinh(float3 x) { return float3(sinh(x.x()), sinh(x.y()), sinh(x.z())); }
VM_INLINE float4 V_CALL sinh(float4 x) { return float4(sinh(x.x()), sinh(x.y()), sinh(x.z()), sinh(x.w())); }

//VM_INLINE float  asin(float x)  { return (float)asin((float)x); }
VM_INLINE float2 V_CALL asin(float2 x) { return float2(asin(x.x()), asin(x.y())); }
VM_INLINE float3 V_CALL asin(float3 x) { return float3(asin(x.x()), asin(x.y()), asin(x.z())); }
VM_INLINE float4 V_CALL asin(float4 x) { return float4(asin(x.x()), asin(x.y()), asin(x.z()), asin(x.w())); }

//VM_INLINE float floor(float x) { return (float)floor((float)x); }
VM_INLINE float2 V_CALL floor(float2 x) { return float2(floor(x.x()), floor(x.y())); }
VM_INLINE float3 V_CALL floor(float3 x) { return float3(floor(x.x()), floor(x.y()), floor(x.z())); }
VM_INLINE float4 V_CALL floor(float4 x) { return float4(floor(x.x()), floor(x.y()), floor(x.z()), floor(x.w())); }

//VM_INLINE float ceil(float x) { return (float)ceil((float)x); }
VM_INLINE float2 V_CALL ceil(float2 x) { return  float2(ceil(x.x()), ceil(x.y())); }
VM_INLINE float3 V_CALL ceil(float3 x) { return  float3(ceil(x.x()), ceil(x.y()), ceil(x.z())); }
VM_INLINE float4 V_CALL ceil(float4 x) { return  float4(ceil(x.x()), ceil(x.y()), ceil(x.z()), ceil(x.w())); }

//VM_INLINE float  round(float x) { return (float)round((float)x); }
VM_INLINE float2 V_CALL round(float2 x) { return float2(round(x.x()), round(x.y())); }
VM_INLINE float3 V_CALL round(float3 x) { return float3(round(x.x()), round(x.y()), round(x.z())); }
VM_INLINE float4 V_CALL round(float4 x) { return float4(round(x.x()), round(x.y()), round(x.z()), round(x.w())); }

//VM_INLINE float trunc(float x) { return (float)trunc((float)x); }
VM_INLINE float2 V_CALL trunc(float2 x) { return float2(trunc(x.x()), trunc(x.y())); }
VM_INLINE float3 V_CALL trunc(float3 x) { return float3(trunc(x.x()), trunc(x.y()), trunc(x.z())); }
VM_INLINE float4 V_CALL trunc(float4 x) { return float4(trunc(x.x()), trunc(x.y()), trunc(x.z()), trunc(x.w())); }

VM_INLINE float V_CALL frac(float x) { return x - floor(x); }
VM_INLINE float2 V_CALL frac(float2 x) { return x - floor(x); }
VM_INLINE float3 V_CALL frac(float3 x) { return x - floor(x); }
VM_INLINE float4 V_CALL frac(float4 x) { return x - floor(x); }

VM_INLINE float V_CALL rcp(float x) { return 1.0f / x; }
VM_INLINE float2 V_CALL rcp(float2 x) { return 1.0f / x; }
VM_INLINE float3 V_CALL rcp(float3 x) { return 1.0f / x; }
VM_INLINE float4 V_CALL rcp(float4 x) { return 1.0f / x; }

VM_INLINE float V_CALL sign(float x) { return x == 0.0f ? 0.0f : (x > 0.0f ? 1.0f : 0.0f) - (x < 0.0f ? 1.0f : 0.0f); }
VM_INLINE float2 V_CALL sign(float2 x) { return float2(sign(x.x()), sign(x.y())); }
VM_INLINE float3 V_CALL sign(float3 x) { return float3(sign(x.x()), sign(x.y()), sign(x.z())); }
VM_INLINE float4 V_CALL sign(float4 x) { return float4(sign(x.x()), sign(x.y()), sign(x.z()), sign(x.w())); }

//VM_INLINE float pow(float x, float y) { return (float)pow((float)x, (float)y); }
VM_INLINE float2 V_CALL pow(float2 x, float2 y) { return  float2(pow(x.x(), y.x()), pow(x.y(), y.y())); }
VM_INLINE float3 V_CALL pow(float3 x, float3 y) { return  float3(pow(x.x(), y.x()), pow(x.y(), y.y()), pow(x.z(), y.z())); }
VM_INLINE float4 V_CALL pow(float4 x, float4 y) { return  float4(pow(x.x(), y.x()), pow(x.y(), y.y()), pow(x.z(), y.z()), pow(x.w(), y.w())); }

//VM_INLINE float exp(float x) { return (float)exp((float)x); }
VM_INLINE float2 V_CALL exp(float2 x) { return  float2(exp(x.x()), exp(x.y())); }
VM_INLINE float3 V_CALL exp(float3 x) { return  float3(exp(x.x()), exp(x.y()), exp(x.z())); }
VM_INLINE float4 V_CALL exp(float4 x) { return  float4(exp(x.x()), exp(x.y()), exp(x.z()), exp(x.w())); }

//VM_INLINE float exp2(float x) { return (float)pow(2.0f, (float)x); }
VM_INLINE float2 V_CALL exp2(float2 x) { return  float2(exp2(x.x()), exp2(x.y())); }
VM_INLINE float3 V_CALL exp2(float3 x) { return  float3(exp2(x.x()), exp2(x.y()), exp2(x.z())); }
VM_INLINE float4 V_CALL exp2(float4 x) { return  float4(exp2(x.x()), exp2(x.y()), exp2(x.z()), exp2(x.w())); }


//VM_INLINE float fmod(float x, float y) { return fmod(x,y); }
VM_INLINE float2 V_CALL fmod(float2 x, float2 y) { return  float2(fmod(x.x(),y.x()), fmod(x.y() , y.y())); }
VM_INLINE float3 V_CALL fmod(float3 x, float3 y) { return  float3(fmod(x.x(), y.x()), fmod(x.y(), y.y()),fmod(x.z(), y.z())); }
VM_INLINE float4 V_CALL fmod(float4 x, float4 y) { return  float4(fmod(x.x(),y.x()), fmod(x.y(),y.y()), fmod(x.z(),y.z()), fmod(x.w(),y.w())); }

VM_INLINE float V_CALL distance(float x, float y) { return abs(y - x); }
VM_INLINE float V_CALL distance(float2 x, float2 y) { return length(y - x); }

VM_INLINE float V_CALL distance(float4 x, float4 y) { return length(y - x); }

VM_INLINE float V_CALL smoothstep(float a, float b, float x)
{
    float t = saturate((x - a) / (b - a));
    return t * t * (3.0f - (2.0f * t));
}

#if 0
VM_INLINE float2 V_CALL smoothstep(float2 a, float2 b, float2 x)
{
    float2 t = saturate((x - a) / (b - a));
    return t * t * (3.0f - (2.0f * t));
}

VM_INLINE float3 V_CALL smoothstep(float3 a, float3 b, float3 x)
{
    float3 t = saturate((x - a) / (b - a));
    return t * t * (3.0f - (2.0f * t));
}
VM_INLINE float4 V_CALL smoothstep(float4 a, float4 b, float4 x)
{
    float4 t = saturate((x - a) / (b - a));
    return t * t * (3.0f - (2.0f * t));
}
#endif


VM_INLINE float  V_CALL select(float  a, float  b, bool c) { return c ? b : a; }
VM_INLINE float2 V_CALL select(float2 a, float2 b, bool c) { return c ? b : a; }
VM_INLINE float3 V_CALL select(float3 a, float3 b, bool c) { return c ? b : a; }
VM_INLINE float4 V_CALL select(float4 a, float4 b, bool c) { return c ? b : a; }

VM_INLINE float2 V_CALL reflect(float2 i, float2 n) { return i - 2.0f * n * dot(i, n); }
VM_INLINE float3 V_CALL reflect(float3 i, float3 n) { return i - 2.0f * dot(i,n) * n; }
VM_INLINE float4 V_CALL reflect(float4 i, float4 n) { return i - 2.0f * n * dot(i, n); }

VM_INLINE float2 V_CALL refract(float2 i, float2 n, float eta)
{
    float ni = dot(n, i);
    float k = 1.0f - eta * eta * (1.0f - ni * ni);
    return select(float2(0.0f), eta * i - (eta * ni + sqrt(k)) * n, k >= 0);
}
VM_INLINE float3 V_CALL refract(float3 i, float3 n, float eta)
{
    float ni = dot(n, i);
    float k = 1.0f - eta * eta * (1.0f - ni * ni);
    return select(float3(0.0f), eta * i - (eta * ni + sqrt(k)) * n, k >= 0);
}
VM_INLINE float4 V_CALL refract(float4 i, float4 n, float eta)
{
    float ni = dot(n, i);
    float k = 1.0f - eta * eta * (1.0f - ni * ni);
    return select(float4(0.0f), eta * i - (eta * ni + sqrt(k)) * n, k >= 0);
}

VM_INLINE float V_CALL radians(float x) { return x * 0.0174532925f; }
VM_INLINE float2 V_CALL radians(float2 x) { return x * 0.0174532925f; }
VM_INLINE float3 V_CALL radians(float3 x) { return x * 0.0174532925f; }
VM_INLINE float4 V_CALL radians(float4 x) { return x * 0.0174532925f; }

VM_INLINE float V_CALL degrees(float x) { return x * 57.295779513f; }
VM_INLINE float2 V_CALL degrees(float2 x) { return x * 57.295779513f; }
VM_INLINE float3 V_CALL degrees(float3 x) { return x * 57.295779513f; }
VM_INLINE float4 V_CALL degrees(float4 x) { return x * 57.295779513f; }
#endif///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct float3x3
{
    float4 c0;
    float4 c1;
    float4 c2;
    VM_INLINE V_CALL float3x3();
    VM_INLINE explicit V_CALL float3x3(float a, float b, float c);
    VM_INLINE explicit V_CALL float3x3(float3 c0, float3 c1, float3 c2);
    VM_INLINE explicit V_CALL float3x3(float m00, float m01, float m02,
                                       float m10, float m11, float m12, 
                                       float m20, float m21, float m22);
	VM_INLINE static uint32_t size();
	V_CALL float3x3(quaternion rotation);
};

struct float4x4
{
    float4 c0;
    float4 c1;
    float4 c2;
    float4 c3;
    VM_INLINE V_CALL float4x4();
	VM_INLINE explicit V_CALL float4x4(float a, float b, float c, float d);
    VM_INLINE explicit V_CALL float4x4(float4 c0, float4 c1, float4 c2,float4 c3);
    VM_INLINE explicit V_CALL float4x4(float m00, float m01, float m02, float m03,
                                float m10, float m11, float m12, float m13,
                                float m20, float m21, float m22, float m23,
                                       float m30, float m31, float m32, float m33);
    VM_INLINE explicit V_CALL float4x4(float3x3 rotation, float3 translation);
	VM_INLINE static uint32_t size();
    VM_INLINE static float4x4 V_CALL identity();
    VM_INLINE static float4x4 V_CALL zero();
	V_CALL float4x4(quaternion rotation,float3 translation);
};

//mul
// float4x4 V_CALL operator * (float4x4 lhs, float4x4 rhs) { return float4x4 (lhs.c0 * rhs.c0, lhs.c1 * rhs.c1, lhs.c2 * rhs.c2, lhs.c3 * rhs.c3); }
float4x4 V_CALL operator * (float4x4 lhs, float rhs);
float4x4 V_CALL operator * (float lhs, float4x4 rhs);

// add
VM_INLINE float4x4 V_CALL operator + (float4x4 lhs, float4x4 rhs);
VM_INLINE float4x4 V_CALL operator + (float4x4 lhs, float rhs);
VM_INLINE float4x4 V_CALL operator + (float lhs, float4x4 rhs);

//sub
VM_INLINE float4x4 V_CALL operator - (float4x4 lhs, float4x4 rhs);
VM_INLINE float4x4 V_CALL operator - (float4x4 lhs, float rhs);
VM_INLINE float4x4 V_CALL operator - (float lhs, float4x4 rhs);

//div
VM_INLINE float4x4 V_CALL operator / (float4x4 lhs, float4x4 rhs);
VM_INLINE float4x4 V_CALL operator / (float4x4 lhs, float rhs);
VM_INLINE float4x4 V_CALL operator / (float lhs, float4x4 rhs);

#ifdef YOYOIMPL////////////////////////////////////////////////////////////////////////////////////////////////

VM_INLINE V_CALL float3x3::float3x3() {}
VM_INLINE V_CALL float3x3::float3x3(float a, float b, float c) { this->c0.setX(a);this->c1.setY(b);this->c2.setZ(c);}
VM_INLINE V_CALL float3x3::float3x3(float3 c0, float3 c1, float3 c2) { this->c0 = float4(c0,0);this->c1 = float4(c1,0);this->c2 = float4(c2,0);}
VM_INLINE V_CALL float3x3::float3x3(float m00, float m01, float m02,
                                    float m10, float m11, float m12, 
                                    float m20, float m21, float m22)
{
    this->c0 = float4(m00, m01, m02, 0);
    this->c1 = float4(m10, m11, m12, 0);
    this->c2 = float4(m20, m21, m22, 0);
}
VM_INLINE uint32_t float3x3::size() { return sizeof(float) * 9; }

VM_INLINE V_CALL float4x4::float4x4() {}

VM_INLINE  V_CALL float4x4::float4x4(float a, float b, float c, float d)
{
    this->c0 = float4(0.0f); this->c0.setX(a); 
    this->c1 = float4(0.0f); this->c1.setY(b);
    this->c2 = float4(0.0f); this->c2.setZ(c); 
    this->c3 = float4(0.0f); this->c3.setW(d);
}

VM_INLINE  V_CALL float4x4::float4x4(float4 c0, float4 c1, float4 c2,float4 c3) { this->c0 = c0;this->c1 = c1;this->c2 = c2;this->c3 = c3; }
VM_INLINE V_CALL float4x4::float4x4(float m00, float m01, float m02, float m03,
                                    float m10, float m11, float m12, float m13,
                                    float m20, float m21, float m22, float m23,
                                    float m30, float m31, float m32, float m33)
{ 
    this->c0 = float4(m00, m01, m02, m03);
    this->c1 = float4(m10, m11, m12, m13);
    this->c2 = float4(m20, m21, m22, m23);
    this->c3 = float4(m30, m31, m32, m33);
}

VM_INLINE V_CALL float4x4::float4x4(float3x3 rotation, float3 translation)
{
    c0 = (rotation.c0);
    c1 = (rotation.c1);
    c2 = (rotation.c2);
    c3 = float4(translation,1.0f);
}
VM_INLINE  uint32_t float4x4::size() { return sizeof(float) * 16; }
VM_INLINE  float4x4 V_CALL float4x4::identity() { return float4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); };
VM_INLINE  float4x4 V_CALL float4x4::zero() { return float4x4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); }


//mul
// float4x4 V_CALL operator * (float4x4 lhs, float4x4 rhs) { return float4x4 (lhs.c0 * rhs.c0, lhs.c1 * rhs.c1, lhs.c2 * rhs.c2, lhs.c3 * rhs.c3); }
 float4x4 V_CALL operator * (float4x4 lhs, float rhs) { return    float4x4 (lhs.c0 * rhs, lhs.c1 * rhs, lhs.c2 * rhs, lhs.c3 * rhs); }
 float4x4 V_CALL operator * (float lhs, float4x4 rhs) { return    float4x4 (lhs * rhs.c0, lhs * rhs.c1, lhs * rhs.c2, lhs * rhs.c3); }

// add
VM_INLINE float4x4 V_CALL operator + (float4x4 lhs, float4x4 rhs) { return float4x4 (lhs.c0 + rhs.c0, lhs.c1 + rhs.c1, lhs.c2 + rhs.c2, lhs.c3 + rhs.c3); }
VM_INLINE float4x4 V_CALL operator + (float4x4 lhs, float rhs) { return    float4x4 (lhs.c0 + rhs, lhs.c1 + rhs, lhs.c2 + rhs, lhs.c3 + rhs); }
VM_INLINE float4x4 V_CALL operator + (float lhs, float4x4 rhs) { return    float4x4 (lhs + rhs.c0, lhs + rhs.c1, lhs + rhs.c2, lhs + rhs.c3); }

//sub
VM_INLINE float4x4 V_CALL operator - (float4x4 lhs, float4x4 rhs) { return float4x4 (lhs.c0 - rhs.c0, lhs.c1 - rhs.c1, lhs.c2 - rhs.c2, lhs.c3 - rhs.c3); }
VM_INLINE float4x4 V_CALL operator - (float4x4 lhs, float rhs) { return  float4x4 (lhs.c0 - rhs, lhs.c1 - rhs, lhs.c2 - rhs, lhs.c3 - rhs); }
VM_INLINE float4x4 V_CALL operator - (float lhs, float4x4 rhs) { return  float4x4 (lhs - rhs.c0, lhs - rhs.c1, lhs - rhs.c2, lhs - rhs.c3); }

//div
VM_INLINE float4x4 V_CALL operator / (float4x4 lhs, float4x4 rhs) { return float4x4 (lhs.c0 / rhs.c0, lhs.c1 / rhs.c1, lhs.c2 / rhs.c2, lhs.c3 / rhs.c3); }
VM_INLINE float4x4 V_CALL operator / (float4x4 lhs, float rhs) { return float4x4 (lhs.c0 / rhs, lhs.c1 / rhs, lhs.c2 / rhs, lhs.c3 / rhs); }
VM_INLINE float4x4 V_CALL operator / (float lhs, float4x4 rhs) { return float4x4 (lhs / rhs.c0, lhs / rhs.c1, lhs / rhs.c2, lhs / rhs.c3); }

#endif ///YOYOYIMPL////////////////////////////////////////////////////////////////////////////////////////////////
/*
YoyoAString YoyoFloat4x4ToString()
        {
            return string.Format("float4x4({0}f, {1}f, {2}f, {3}f,  {4}f, {5}f, {6}f, {7}f,  {8}f, {9}f, {10}f, {11}f,  {12}f, {13}f, {14}f, {15}f)", c0.x, c1.x, c2.x, c3.x, c0.y, c1.y, c2.y, c3.y, c0.z, c1.z, c2.z, c3.z, c0.w, c1.w, c2.w, c3.w);
        }
*/

float4x4 V_CALL mul(float4x4 a, float4x4 b);

float4 V_CALL operator*(float4 a, float4x4 b);

float4 V_CALL mul(float4 a, float4x4 b);

VM_INLINE float4 V_CALL operator*(float4x4 a, float4 b);

VM_INLINE float4 V_CALL mul(float4x4 a, float4 b);

#ifdef YOYOIMPL
float4x4 V_CALL mul(float4x4 a, float4x4 b)
{
	return float4x4(
		a.c0 * b.c0.x() + a.c1 * b.c0.y() + a.c2 * b.c0.z() + a.c3 * b.c0.w(),
		a.c0 * b.c1.x() + a.c1 * b.c1.y() + a.c2 * b.c1.z() + a.c3 * b.c1.w(),
		a.c0 * b.c2.x() + a.c1 * b.c2.y() + a.c2 * b.c2.z() + a.c3 * b.c2.w(),
		a.c0 * b.c3.x() + a.c1 * b.c3.y() + a.c2 * b.c3.z() + a.c3 * b.c3.w());
}

float4 V_CALL operator*(float4 a, float4x4 b)
{
	return float4(
		a.x() * b.c0.x() + a.y() * b.c0.y() + a.z() * b.c0.z() + a.w() * b.c0.w(),
		a.x() * b.c1.x() + a.y() * b.c1.y() + a.z() * b.c1.z() + a.w() * b.c1.w(),
		a.x() * b.c2.x() + a.y() * b.c2.y() + a.z() * b.c2.z() + a.w() * b.c2.w(),
		a.x() * b.c3.x() + a.y() * b.c3.y() + a.z() * b.c3.z() + a.w() * b.c3.w());
}

float4 V_CALL mul(float4 a, float4x4 b)
{
	return a * b;
}

/*
float4 operator *(mat4 A, float4 B)
{
	float4 Result;
	Result.x = A.A00 * B.x + A.A01 * B.y + A.A02 * B.z + A.A03 * B.w;
	Result.y = A.A10 * B.x + A.A11 * B.y + A.A12 * B.z + A.A13 * B.w;
	Result.z = A.A20 * B.x + A.A21 * B.y + A.A22 * B.z + A.A23 * B.w;
	Result.w = A.A30 * B.x + A.A31 * B.y + A.A32 * B.z + A.A33 * B.w;
	return Result;
}
*/

VM_INLINE float4 V_CALL operator*(float4x4 a, float4 b)
{
	return a.c0 * b.x() + a.c1 * b.y() + a.c2 * b.z() + a.c3 * b.w();
}

VM_INLINE float4 V_CALL mul(float4x4 a, float4 b)
{
	return a * b;
}

#endif////////////yOYOYOYIMPL////////////yOYOYOYIMPL////////////yOYOYOYIMPL////////////yOYOYOYIMPL////////////yOYOYOYIMPL




struct quaternion
{
#if YOYO_MATH_SIMD
    __m128 m;
#else
	float m[4];
#endif
    // Constructors.
    VM_INLINE V_CALL quaternion();
    VM_INLINE explicit V_CALL quaternion(const float *p);
	VM_INLINE explicit V_CALL quaternion(float x);
	VM_INLINE explicit V_CALL quaternion(float4 a);
	VM_INLINE explicit V_CALL quaternion(float x, float y, float z, float w);
	VM_INLINE explicit V_CALL quaternion(quaterniondata a);
	VM_INLINE explicit V_CALL quaternion(float3 a, float b);
	VM_INLINE explicit V_CALL quaternion(float2 a, float2 b);
    //VM_INLINE explicit V_CALL quaternion(float x, float y, float z,float w) { m = _mm_set_ps(w, z, y, x); }
    //VM_INLINE explicit V_CALL quaternion(float4 x) { m = _mm_set_ps(x.w(), x.z(), x.y(), x.x()); }
	//VM_INLINE explicit V_CALL quaternion(quaterniondata a) { m = _mm_set_ps(a.w, a.z, a.y, a.x); }
    //VM_INLINE explicit V_CALL quaternion(float x) { m = _mm_set_ps(x, x, x, x); }
    //VM_INLINE explicit V_CALL quaternion(__m128 v) { m = v; }
    //VM_INLINE explicit V_CALL quaternion(float3 a,float b){m = _mm_set_ps(a.x(),a.y(),a.x(),b);}
    //VM_INLINE explicit V_CALL quaternion(float2 a,float2 b){m = _mm_set_ps(a.x(),a.y(),b.x(),b.y());}

 	VM_INLINE quaterniondata toquaterniondata();
//The matrix must be orthonormal.
    VM_INLINE explicit V_CALL quaternion(float3x3 m);
	VM_INLINE static float4 tofloat4(quaternion q);

#ifdef YOYO_USE_PHYSX_EXT
	VM_INLINE explicit V_CALL quaternion(physx::PxQuat a);
	VM_INLINE physx::PxQuat toPhysx();
#endif
        // Construct unit quaternion from rigid-transformation matrix. The matrix must be orthonormal.
	VM_INLINE static uint32_t size();
    V_CALL quaternion(float4x4 m);
	VM_INLINE float V_CALL x() const;
	VM_INLINE float V_CALL y() const;
	VM_INLINE float V_CALL z() const;
	VM_INLINE float V_CALL w() const;

    //VM_INLINE float V_CALL x() const { return _mm_cvtss_f32(m); }
    //VM_INLINE float V_CALL y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    //VM_INLINE float V_CALL z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
    //VM_INLINE float V_CALL w() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3))); }

	VM_INLINE float3 V_CALL xyz() const;
	VM_INLINE float3 V_CALL zyx() const;
	VM_INLINE float3 V_CALL yxz() const;
	VM_INLINE float3 V_CALL yzx() const;

	VM_INLINE float3 V_CALL zxy() const;
	
 	VM_INLINE float4 V_CALL xyzw() const;
#if 0
    //VM_INLINE float3 V_CALL xyz() const { return SHUFFLE3(*this, 0, 1, 2); }
    VM_INLINE float4 V_CALL xyzw() const;
    VM_INLINE float4 V_CALL yzxz() const;
    VM_INLINE float4 V_CALL zxyz() const;
    VM_INLINE float4 V_CALL zxyy() const;
    VM_INLINE float4 V_CALL yzxy() const;
    VM_INLINE float4 V_CALL wwwx() const;
    VM_INLINE float4 V_CALL xyzx() const;
    VM_INLINE float4 V_CALL wwww() const;
    VM_INLINE float4 V_CALL yzxw() const;
    VM_INLINE float4 V_CALL zxyw() const;
    VM_INLINE float2 V_CALL xy() const;
    VM_INLINE float2 V_CALL zw() const;

    VM_INLINE float2 V_CALL xx() const;
    VM_INLINE float2 V_CALL yz() const;
    VM_INLINE float2 V_CALL wx() const;
    VM_INLINE float2 V_CALL xz() const;
    VM_INLINE float2 V_CALL yx() const;
    VM_INLINE float2 V_CALL yw() const;
    VM_INLINE float2 V_CALL zx() const;
    VM_INLINE float2 V_CALL zz() const;
    VM_INLINE float2 V_CALL wz() const;
    VM_INLINE float2 V_CALL wy() const;
#endif
	//TODO(Ray):Try not to use these as much as possible.
#if WINDOWS
	//VM_INLINE float V_CALL operator[] (size_t i) const { return m.m128_f32[i]; };
	//VM_INLINE float& V_CALL operator[] (size_t i) { return m.m128_f32[i]; };
#else
#endif
	
 	VM_INLINE void V_CALL store(float *p) const;
    VM_INLINE quaternion  static V_CALL identity();
	void setX(float x);
	void V_CALL setY(float y);
	void setZ(float z);
	void V_CALL setW(float w); 	
	static quaternion V_CALL look_rotation(float3 forward, float3 up);

};

#ifdef YOYOIMPL

    // Constructors.
    VM_INLINE V_CALL quaternion::quaternion() {}
    VM_INLINE V_CALL quaternion::quaternion(const float *p)
    {
#if YOYO_MATH_SIMD
	    m = _mm_set_ps(p[3], p[2], p[1], p[0]);
#else
		m[0] = p[0];
		m[1] = p[1];
		m[2] = p[2];
		m[3] = p[3];
#endif
    }

	VM_INLINE V_CALL quaternion::quaternion(float x)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(x, x, x, x);
#else
		m[0] = x;
		m[1] = x;
		m[2] = x;
		m[3] = x;
#endif
	}

	VM_INLINE  V_CALL quaternion::quaternion(float4 a)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(a.x(), a.y(), a.z(), a.w());
#else
		m[0] = a.m[0];
		m[1] = a.m[1];
		m[2] = a.m[2];
		m[3] = a.m[3];
#endif
	}

	VM_INLINE V_CALL quaternion::quaternion(float x, float y, float z, float w)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(w, z, y, x);
#else
		m[0] = x;
		m[1] = y;
		m[2] = z;
		m[3] = w;
#endif
	}

	VM_INLINE V_CALL quaternion::quaternion(quaterniondata a)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(a.w, a.z, a.y, a.x);
#else
		m[0] = a.x;
		m[1] = a.y;
		m[2] = a.z;
		m[3] = a.w;
#endif
	}

	VM_INLINE V_CALL quaternion::quaternion(float3 a, float b)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(b, a.z(), a.y(), a.x());
#else
		m[0] = a.x();
		m[1] = a.y();
		m[2] = a.z();
		m[3] = b;
#endif
	}

    VM_INLINE V_CALL quaternion::quaternion(float2 a, float2 b)
	{
#if YOYO_MATH_SIMD
		m = _mm_set_ps(b.y(), b.x(), a.y(), a.x());
#else
		m[0] = a.m[0];
		m[1] = a.m[1];
		m[2] = b.m[0];
		m[3] = b.m[1];
#endif
	}

    //VM_INLINE explicit V_CALL quaternion(float x, float y, float z,float w) { m = _mm_set_ps(w, z, y, x); }
    //VM_INLINE explicit V_CALL quaternion(float4 x) { m = _mm_set_ps(x.w(), x.z(), x.y(), x.x()); }
	//VM_INLINE explicit V_CALL quaternion(quaterniondata a) { m = _mm_set_ps(a.w, a.z, a.y, a.x); }
    //VM_INLINE explicit V_CALL quaternion(float x) { m = _mm_set_ps(x, x, x, x); }
    //VM_INLINE explicit V_CALL quaternion(__m128 v) { m = v; }
    //VM_INLINE explicit V_CALL quaternion(float3 a,float b){m = _mm_set_ps(a.x(),a.y(),a.x(),b);}
    //VM_INLINE explicit V_CALL quaternion(float2 a,float2 b){m = _mm_set_ps(a.x(),a.y(),b.x(),b.y());}

 	VM_INLINE quaterniondata quaternion::toquaterniondata()
    {
#if YOYO_MATH_SIMD
		quaterniondata a;
		a.x = _mm_cvtss_f32(m);
		a.y = _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1)));
		a.z = _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2)));
		a.w = _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3)));
#else
		quaterniondata a;
		a.x = m[0];
		a.y = m[1];
		a.z = m[2];
		a.w = m[3];
#endif
		return a;
    }
//The matrix must be orthonormal.
    VM_INLINE V_CALL quaternion::quaternion(float3x3 m)
    {
        quaternion value;
        float4 u = m.c0;
        float4 v = m.c1;
        float4 w = m.c2;

        if (u.x() >= 0.0f)
        {
            float t = v.y() + w.z();
            if (t >= 0.0f)
            {
#if YOYO_MATH_SIMD
				this->m = _mm_set_ps(v.z() - w.y(), w.x() - u.z(), u.y() - v.x(), 1.0f + u.x() + t);
#else
				this->m[0] = 1.0f + u.x() + t;
				this->m[1] = u.y() - v.x();
				this->m[2] = w.x() - u.z();
				this->m[3] = v.z() - w.y();
#endif
            }
            else
            {
#if YOYO_MATH_SIMD
            	this->m = _mm_set_ps(1.0f + u.x() - t, u.y() + v.x(), w.x() + u.z(), v.z() - w.y());
#else
				this->m[0] = v.z() - w.y();
				this->m[1] = w.x() + u.z();
				this->m[2] = u.y() + v.x();
				this->m[3] = 1.0f + u.x() - t;
#endif
            }
        }
        else
        {
            float t = v.y() - w.z();
            if (t >= 0.0f)
            {
#if YOYO_MATH_SIMD
            	this->m = _mm_set_ps(u.y() + v.x(), 1.0f - u.x() + t, v.z() + w.y(), w.x() - u.z());
#else
				this->m[0] = w.x() - u.z();
				this->m[1] = v.z() + w.y();
				this->m[2] = 1.0f - u.x() + t;
				this->m[3] = u.y() + v.x();
#endif
            }
            else
            {
#if YOYO_MATH_SIMD
            	this->m = _mm_set_ps(w.x() + u.z(), v.z() + w.y(), 1.0f - u.x() - t, u.y() - v.x());
#else
				this->m[0] = u.y() - v.x();
				this->m[1] = 1.0f - u.x() - t;
				this->m[2] = v.z() + w.y();
				this->m[3] = w.x() + u.z();
#endif
            }
        }
        float4 q = normalize(this->xyzw());
#if YOYO_MATH_SIMD
    	this->m =  _mm_set_ps(q.x(),q.y(),q.z(),q.w());
#else
		this->m[0] = q.w();
		this->m[1] = q.z();
		this->m[2] = q.y();
		this->m[3] = q.x();
#endif
    }

	VM_INLINE float4 quaternion::tofloat4(quaternion q) { return float4(q.m);}

#ifdef YOYO_USE_PHYSX_EXT
	VM_INLINE V_CALL quaternion::quaternion(physx::PxQuat a)
    {
#if YOYO_MATH_SIMD
	    m = _mm_set_ps(a.w, a.z, a.y, a.x);
#else
		m[0] = a.x;
		m[1] = a.y;
		m[2] = a.z;
		m[3] = a.w;
#endif
    }

//	VM_INLINE physx::PxQuat toPhysx();
#endif
        // Construct unit quaternion from rigid-transformation matrix. The matrix must be orthonormal.
	VM_INLINE uint32_t quaternion::size() { return sizeof(float) * 4; }
    V_CALL quaternion::quaternion(float4x4 m)
    {
		float3x3 in_mat;
		in_mat.c0 = m.c0;
		in_mat.c1 = m.c1;
		in_mat.c2 = m.c2;
		quaternion result = quaternion(in_mat);
#if YOYO_MATH_SIMD
		this->m = result.m;
#else
		this->m[0] = result.m[0];
		this->m[1] = result.m[1];
		this->m[2] = result.m[2];
		this->m[3] = result.m[3];
#endif
#if 0
        float4 u = m.c0;
        float4 v = m.c1;
        float4 w = m.c2;

        if (u.x() >= 0.0f)
        {
            float t = v.y() + w.z();
            if (t >= 0.0f)
                this->m = _mm_set_ps(v.z() - w.y(), w.x() - u.z(), u.y() - v.x(), 1.0f + u.x() + t);
            else
                this->m = _mm_set_ps(1.0f + u.x() - t,  u.y() + v.x(),  w.x() + u.z(), v.z() - w.y());
        }
        else
        {
            float t = v.y() - w.z();
            if (t >= 0.0f)
                this->m = _mm_set_ps(u.y() + v.x(),  1.0f - u.x() + t,  v.z() + w.y(), w.x() - u.z());
            else
                this->m = _mm_set_ps(w.x() + u.z(),  v.z() + w.y(),  1.0f - u.x() - t, u.y() - v.x());
        }
        float4 q = normalize(float4(this->m));
        
		this->m =  _mm_set_ps(q.x(),q.y(),q.z(),q.w());
#endif
    }

	VM_INLINE float V_CALL quaternion::x() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(m);
#else
		return m[0];
#endif
	}

	VM_INLINE float V_CALL quaternion::y() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1)));
#else
		return m[1];
#endif
	}

	VM_INLINE float V_CALL quaternion::z() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2)));
#else
		return m[2];
#endif
	}
    
	VM_INLINE float V_CALL quaternion::w() const
	{
#if YOYO_MATH_SIMD
		return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3)));
#else
		return m[3];
#endif
	}

    //VM_INLINE float V_CALL x() const { return _mm_cvtss_f32(m); }
    //VM_INLINE float V_CALL y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    //VM_INLINE float V_CALL z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
    //VM_INLINE float V_CALL w() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3))); }

	VM_INLINE float3 V_CALL quaternion::xyz() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE3(*this, 0, 1, 2);
#else
		return float3(m[0], m[1], m[2]);
#endif
	}

	VM_INLINE float3 V_CALL quaternion::zyx() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE3(*this, 0, 1, 2);
#else
		return float3(m[2], m[1], m[0]);
#endif
	}

	VM_INLINE float3 V_CALL quaternion::yxz() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE3(*this, 0, 1, 2);
#else
		return float3(m[1], m[0], m[2]);
#endif
	}

	VM_INLINE float3 V_CALL quaternion::yzx() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE3(*this, 1, 2, 0);
#else
		return float3(m[1], m[2], m[0]);
#endif
	}

	VM_INLINE float3 V_CALL quaternion::zxy() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE3(*this, 2, 0, 1);
#else
		return float3(m[2], m[0], m[1]);
#endif
	}
	
 	VM_INLINE float4 V_CALL quaternion::xyzw() const
	{
#if YOYO_MATH_SIMD
		return SHUFFLE4(*this, 0, 1, 2, 3);
#else
		return float4(m[0], m[1], m[2], m[3]);
#endif
	}

#if 0
    //VM_INLINE float3 V_CALL xyz() const { return SHUFFLE3(*this, 0, 1, 2); }
    VM_INLINE float4 V_CALL xyzw() const { return SHUFFLE4(*this, 0, 1, 2, 3); }
    VM_INLINE float4 V_CALL yzxz() const { return SHUFFLE4(*this, 1, 2, 0, 2); }
    VM_INLINE float4 V_CALL zxyz() const { return SHUFFLE4(*this, 2, 0, 1, 2); }
    VM_INLINE float4 V_CALL zxyy() const { return SHUFFLE4(*this, 2, 0, 1, 1); }
    VM_INLINE float4 V_CALL yzxy() const { return SHUFFLE4(*this, 1, 2, 0, 1); }
    VM_INLINE float4 V_CALL wwwx() const { return SHUFFLE4(*this, 3, 3, 3, 0); }
    VM_INLINE float4 V_CALL xyzx() const { return SHUFFLE4(*this, 0, 1, 2, 0); }
    VM_INLINE float4 V_CALL wwww() const { return SHUFFLE4(*this, 3, 3, 3, 3); }
    VM_INLINE float4 V_CALL yzxw() const { return SHUFFLE4(*this, 1, 2, 0, 3); }
    VM_INLINE float4 V_CALL zxyw() const { return SHUFFLE4(*this, 2, 0, 1, 3); }
    VM_INLINE float2 V_CALL xy() const { return SHUFFLE2(*this, 0, 1); }
    VM_INLINE float2 V_CALL zw() const { return SHUFFLE2(*this, 2, 3); }

    VM_INLINE float2 V_CALL xx() const { return SHUFFLE2(*this, 0, 0); }
    VM_INLINE float2 V_CALL yz() const { return SHUFFLE2(*this, 1, 2); }
    VM_INLINE float2 V_CALL wx() const { return SHUFFLE2(*this, 3, 0); }
    VM_INLINE float2 V_CALL xz() const { return SHUFFLE2(*this, 0, 2); }
    VM_INLINE float2 V_CALL yx() const { return SHUFFLE2(*this, 1, 0); }
    VM_INLINE float2 V_CALL yw() const { return SHUFFLE2(*this, 1, 3); }
    VM_INLINE float2 V_CALL zx() const { return SHUFFLE2(*this, 2, 0); }
    VM_INLINE float2 V_CALL zz() const { return SHUFFLE2(*this, 2, 2); }
    VM_INLINE float2 V_CALL wz() const { return SHUFFLE2(*this, 3, 2); }
    VM_INLINE float2 V_CALL wy() const { return SHUFFLE2(*this, 3, 1); }
#endif
	//TODO(Ray):Try not to use these as much as possible.
#if WINDOWS
	//VM_INLINE float V_CALL operator[] (size_t i) const { return m.m128_f32[i]; };
	//VM_INLINE float& V_CALL operator[] (size_t i) { return m.m128_f32[i]; };
#else
#endif
	
 	VM_INLINE void V_CALL quaternion::store(float *p) const { p[0] = x(); p[1] = y(); p[2] = z(); p[3] = w(); }
    VM_INLINE quaternion  V_CALL quaternion::identity(){return quaternion(0,0,0,1);}

	void quaternion::setX(float x)
	{
#if YOYO_MATH_SIMD
		m = _mm_move_ss(m, _mm_set_ss(x));
#else
		m[0] = x;
#endif
	}

	void V_CALL quaternion::setY(float y)
	{
#if YOYO_MATH_SIMD
		__m128 t = _mm_move_ss(m, _mm_set_ss(y));
		t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
		m = _mm_move_ss(t, m);
#else
		m[1] = y;
#endif
	}

	void quaternion::setZ(float z)
	{
#if YOYO_MATH_SIMD
		__m128 t = _mm_move_ss(m, _mm_set_ss(z));
		t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
		m = _mm_move_ss(t, m);
#else
		m[2] = z;
#endif
	}

	void V_CALL quaternion::setW(float w)
	{
#if YOYO_MATH_SIMD
		__m128 t = _mm_move_ss(m, _mm_set_ss(w));
		t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(0, 2, 1, 0));
		m = _mm_move_ss(t, m);
#else
		m[3] = w;
#endif
	}

#endif
//mod
//VM_INLINE float4x4 operator % (float4x4 lhs, float4x4 rhs) { return new float4x4 (lhs.c0 % rhs.c0, lhs.c1 % rhs.c1, lhs.c2 % rhs.c2, lhs.c3 % rhs.c3); }
//VM_INLINE float4x4 operator % (float4x4 lhs, float rhs) { return new float4x4 (lhs.c0 % rhs, lhs.c1 % rhs, lhs.c2 % rhs, lhs.c3 % rhs); }
//VM_INLINE float4x4 operator % (float lhs, float4x4 rhs) { return new float4x4 (lhs % rhs.c0, lhs % rhs.c1, lhs % rhs.c2, lhs % rhs.c3); }
//inc
//VM_INLINE float4x4 operator ++ (float4x4 val) { return new float4x4 (++val.c0, ++val.c1, ++val.c2, ++val.c3); }
//dec
//VM_INLINE float4x4 operator -- (float4x4 val) { return new float4x4 (--val.c0, --val.c1, --val.c2, --val.c3); }

typedef quaternion boolq;

quaternion V_CALL mul(quaternion q1, quaternion q2);
VM_INLINE quaternion V_CALL operator* (quaternion a, quaternion b);
VM_INLINE quaternion& V_CALL operator*= (quaternion &a, quaternion b);

#ifdef YOYOIMPL/////////////
 quaternion V_CALL mul(quaternion q1, quaternion q2)
{
	
	return quaternion(
		q1.x() * q2.w() + q1.y() * q2.z() - q1.z() * q2.y() + q1.w() * q2.x(),
		-q1.x() * q2.z() + q1.y() * q2.w() + q1.z() * q2.x() + q1.w() * q2.y(),
		q1.x() * q2.y() - q1.y() * q2.x() + q1.z() * q2.w() + q1.w() * q2.z(),
		-q1.x() * q2.x() - q1.y() * q2.y() - q1.z() * q2.z() + q1.w() * q2.w()
	);
    //return quaternion(a.wwww() * b.xyzw() + (a.xyzx() * b.wwwx() + a.yzxy() * b.zxyy()) * float4(1.0f, 1.0f, 1.0f, -1.0f) - a.zxyz() * b.yzxz());
}
VM_INLINE quaternion V_CALL operator* (quaternion a, quaternion b) { a = mul(a,b); return a; }
VM_INLINE quaternion& V_CALL operator*= (quaternion &a, quaternion b) {a = a * b; return a; }
#endif/////////////////

#if 0
//VM_INLINE quaternion abs(quaternion v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }
// Returns a 3-bit code where bit0..bit2 is X..Z
VM_INLINE unsigned V_CALL mask(quaternion v) { return _mm_movemask_ps(v.m) & 7; }
// Once we have a comparison, we can branch based on its results:
VM_INLINE bool V_CALL any(boolq v) { return mask(v) != 0; }
VM_INLINE bool V_CALL all(boolq v) { return mask(v) == 7; }
#endif

//TODO(Rays):Quaternion normalize needs more rigor.
//NOTE(Ray):Going back to the dumb but working quaternion normalization.
inline quaternion normalize(quaternion Q);

VM_INLINE float V_CALL dot(quaternion a, quaternion b);
VM_INLINE float V_CALL length(quaternion q);
VM_INLINE float V_CALL lengthsq(quaternion q);
quaternion V_CALL conjugate(quaternion q);

float3 V_CALL rotate(quaternion q, float3 dir);

quaternion V_CALL nlerp(quaternion q1, quaternion q2, float t);

quaternion V_CALL inverse(quaternion q);

quaternion V_CALL slerp(quaternion q1, quaternion q2, float t);

quaternion V_CALL axis_angle(float3 axis, float angle);

//quaternion V_CALL quaternion::look_rotation(float3 forward, float3 up);

//V_CALL float3x3::float3x3(quaternion rotation);

float3x3 V_CALL transpose(float3x3 v);

//V_CALL float4x4::float4x4(quaternion rotation,float3 translation);

float4x4 V_CALL scale(float s);

float4x4 V_CALL scale(float x, float y, float z);

float4x4 V_CALL scale(float3 scales);

float4x4 V_CALL translate(float3 vector);

float3x3 V_CALL look_rotation(float3 forward, float3 up);

float4x4 V_CALL look_at(float3 eye, float3 target, float3 up);

VM_INLINE float3 V_CALL rotate(float4x4 a, float3 b);

VM_INLINE float3 V_CALL transform(float4x4 a, float3 b);

float4x4 V_CALL transpose(float4x4 v);

VM_INLINE float4 V_CALL movelh(float4 a,float4 b);

VM_INLINE float4 V_CALL movehl(float4 a,float4 b);

//////Begin 
static float4 HorizontalAdd(float4 a, float4 b);

#ifdef YOYOIMPL/////////////
//TODO(Rays):Quaternion normalize needs more rigor.
//NOTE(Ray):Going back to the dumb but working quaternion normalization.
inline quaternion normalize(quaternion Q)
{
	float Root = sqrt(Q.x() * Q.x() + Q.y() * Q.y() + Q.z() * Q.z() + Q.w() * Q.w());
	Q = quaternion(Q.xyzw() / Root);
	return Q;
}


VM_INLINE float V_CALL dot(quaternion a, quaternion b){return dot(a.xyzw(), b.xyzw());}
VM_INLINE float V_CALL length(quaternion q){return sqrt(dot(q.xyzw(), q.xyzw()));}
VM_INLINE float V_CALL lengthsq(quaternion q){return dot(q.xyzw(), q.xyzw());}
 quaternion V_CALL conjugate(quaternion q)
{
    return quaternion(q.xyzw() * float4(-1.0f, -1.0f, -1.0f, 1.0f));
}

float3 V_CALL rotate(quaternion q, float3 dir)
{
    float3 t = 2 * cross(q.xyz(), dir);
    return dir + q.w() * t + cross(q.xyz(), t);
}

 quaternion V_CALL nlerp(quaternion q1, quaternion q2, float t)
{
    float dt = dot(q1, q2);
    if(dt < 0.0f)
    {
        q2 = quaternion(-q2.xyzw());
    }

    return normalize(quaternion(lerp(q1.xyzw(), q2.xyzw(), t)));
}

 quaternion V_CALL inverse(quaternion q)
{
    return conjugate(normalize(q));
}

 quaternion V_CALL slerp(quaternion q1, quaternion q2, float t)
{
    float dt = dot(q1, q2);
    if (dt < 0.0f)
    {
        dt = -dt;
        q2 = quaternion(-q2.xyzw());
    }

    if (dt < 0.9995f)
    {
        float angle = acos(dt);
        float s = rsqrt(1.0f - dt * dt);    // 1.0f / sin(angle)
        float w1 = sine(angle * (1.0f - t)) * s;
        float w2 = sine(angle * t) * s;
        return quaternion(q1.xyzw() * w1 + q2.xyzw() * w2);
    }
    else
    {
        // if the angle is small, use linear interpolation
        return nlerp(q1, q2, t);
    }
}

 quaternion V_CALL axis_angle(float3 axis, float angle)
{
    float sina, cosa;
    sincos(0.5f * radians(angle),&sina,&cosa);
    return quaternion(float4(normalize(axis) * sina, cosa));
}

quaternion V_CALL quaternion::look_rotation(float3 forward, float3 up)
{
    float3 dir = normalize(forward);
    float3 rightdir = cross(up, dir);
    float3 updir = cross(dir, rightdir);
#if 0
    float m00 = vector2.x();
    float m01 = vector2.y();
    float m02 = vector2.z();
    float m10 = vector3.x();
    float m11 = vector3.y();
    float m12 = vector3.z();
    float m20 = vector.x();
    float m21 = vector.y();
    float m22 = vector.z();
#else
	float m00 = rightdir.x();
	float m01 = updir.x();
	float m02 = dir.x();

	float m10 = rightdir.y();
	float m11 = updir.y();
	float m12 = dir.y();

	float m20 = rightdir.z();
	float m21 = updir.z();
	float m22 = dir.z();
#endif
    float num8 = (m00 + m11) + m22;
    float4 q;
    if (num8 > 0.0)
    {
        float num = sqrt(num8 + 1.0f);
        q.setW(num * 0.5f);
        num = 0.5f / num;
        q.setX((m12 - m21) * num);
        q.setY((m20 - m02) * num);
        q.setZ((m01 - m10) * num);
        return normalize(quaternion(q));
    }
    if ((m00 >= m11) && (m00 >= m22))
    {
        float num7 = sqrt(((1.0f + m00) - m11) - m22);
        float num4 = 0.5f / num7;
        q.setX(0.5f * num7);
        q.setY((m01 + m10) * num4);
        q.setZ((m02 + m20) * num4);
        q.setW((m12 - m21) * num4);
        return normalize(quaternion(q));
    }
    if (m11 > m22)
    {
        float num6 = sqrt(((1.0f + m11) - m00) - m22);
        float num3 = 0.5f / num6;
        q.setX((m10 + m01) * num3);
        q.setY(0.5f * num6);
        q.setZ((m21 + m12) * num3);
        q.setW((m20 - m02) * num3);
        return normalize(quaternion(q));
    }
    float num5 = sqrt(((1.0f + m22) - m00) - m11);
    float num2 = 0.5f / num5;
    q.setX((m20 + m02) * num2);
    q.setY((m21 + m12) * num2);
    q.setZ(0.5f * num5);
    q.setW((m01 - m10) * num2);
    return normalize(quaternion(q));
}

V_CALL float3x3::float3x3(quaternion rotation)
{
	//rotation = normalize(rotation);
	float x = rotation.x() * 2.0f;
	float y = rotation.y() * 2.0f;
	float z = rotation.z() * 2.0f;
	float xx = rotation.x() * x;
	float yy = rotation.y() * y;
	float zz = rotation.z() * z;
	float xy = rotation.x() * y;
	float xz = rotation.x() * z;
	float yz = rotation.y() * z;
	float wx = rotation.w() * x;
	float wy = rotation.w() * y;
	float wz = rotation.w() * z;

	c0 = float4(1.0f - (yy + zz), xy + wz, xz - wy,0.0f);
	c1 = float4(xy - wz, 1.0f - (xx + zz), yz + wx,0.0f);
	c2 = float4(xz + wy, yz - wx, 1.0f - (xx + yy),0.0f);
}

 float3x3 V_CALL transpose(float3x3 v)
{
	return float3x3(v.c0.x(), v.c1.x(), v.c2.x(),
		            v.c0.y(), v.c1.y(), v.c2.y(),
		            v.c0.z(), v.c1.z(), v.c2.z());
}

 V_CALL float4x4::float4x4(quaternion rotation,float3 translation)
{
    float3x3 rot = float3x3(rotation);
	rot = transpose(rot);
	c0 = rot.c0;// .xyz(), 0.0f);
	c1 = rot.c1;// .xyz(), 0.0f);
	c2 = rot.c2;// .xyz(), 0.0f);
    c3 = float4(translation,1.0f);
}

 float4x4 V_CALL scale(float s)
{
    return float4x4(s,    0.0f, 0.0f, 0.0f,
                    0.0f, s,    0.0f, 0.0f,
                    0.0f, 0.0f, s,    0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);
}

 float4x4 V_CALL scale(float x, float y, float z)
{
    return float4x4(x,    0.0f, 0.0f, 0.0f,
                    0.0f, y,    0.0f, 0.0f,
                    0.0f, 0.0f, z,    0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);
}

 float4x4 V_CALL scale(float3 scales)
{
    return scale(scales.x(), scales.y(), scales.z());
}

 float4x4 V_CALL translate(float3 vector)
{
    return float4x4(float4(1.0f, 0.0f, 0.0f, 0.0f),
                    float4(0.0f, 1.0f, 0.0f, 0.0f),
                    float4(0.0f, 0.0f, 1.0f, 0.0f),
                    float4(vector.x(), vector.y(), vector.z(), 1.0f));
}

 float3x3 V_CALL look_rotation(float3 forward, float3 up)
{
    const float epsilon = 0.000001F;
    float3 z = forward;
    // compute u0
    float mag = length(z);
    if (mag < epsilon)
        return float3x3(1,1,1);
    z /= mag;

    float3 x = cross(up, z);
    mag = length(x);
    if (mag < epsilon)
        return float3x3(1,1,1);
    x /= mag;

    float3 y = cross(z, x);
    float yLength = length(y);
    if (yLength < 0.9F || yLength > 1.1F)
        return float3x3(1,1,1);
    
    return float3x3(x, y, z);
}

 float4x4 V_CALL look_at(float3 eye, float3 target, float3 up)
{
    float3x3 rot = look_rotation(target - eye, up);

    float4x4 matrix;
    matrix.c0 = rot.c0;
    matrix.c1 = rot.c1;
    matrix.c2 = rot.c2;
    matrix.c3 = float4(eye, 1.0F);
    return matrix;
}

VM_INLINE float3 V_CALL rotate(float4x4 a, float3 b)
{
    return (a.c0 * b.x() + a.c1 * b.y() + a.c2 * b.z()).xyz();
}

VM_INLINE float3 V_CALL transform(float4x4 a, float3 b)
{
    return (a.c0 * b.x() + a.c1 * b.y() + a.c2 * b.z() + a.c3).xyz();
}


 float4x4 V_CALL transpose(float4x4 v)
{
	return float4x4(v.c0.x(), v.c1.x(), v.c2.x(), v.c3.x(),
		            v.c0.y(), v.c1.y(), v.c2.y(), v.c3.y(),
		            v.c0.z(), v.c1.z(), v.c2.z(), v.c3.z(),
		            v.c0.w(), v.c1.w(), v.c2.w(), v.c3.w());
	/*
    return float4x4(
                    v.c0.x(), v.c0.y(), v.c0.z(), v.c0.w(),
                    v.c1.x(), v.c1.y(), v.c1.z(), v.c1.w(),
                    v.c2.x(), v.c2.y(), v.c2.z(), v.c2.w(),
                    v.c3.x(), v.c3.y(), v.c3.z(), v.c3.w());*/
}

VM_INLINE float4 V_CALL movelh(float4 a,float4 b)
{
    return float4(a.xy(),b.xy());
}

VM_INLINE float4 V_CALL movehl(float4 a,float4 b)
{
    return float4(b.zw(),a.zw());
}

//////Begin 
static float4 HorizontalAdd(float4 a, float4 b)
{
	//TODO(Ray):Compress these once we verified all is good.
	float x = a.x() + a.y();
	float y = a.z() + a.w();
	float z = b.x() + b.y();
	float w = b.z() + b.w();
	return float4(x, y, z, w);
}
#endif//YOYOYIMPL/////////////////////////////////////////

///NOTE(RAY):This algorithm was ripped from this article
//https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html

#define MakeShuffleMask(x,y,z,w)           (x | (y<<2) | (z<<4) | (w<<6))

// vec(0, 1, 2, 3) -> (vec[x], vec[y], vec[z], vec[w])
#define VecSwizzle(vec, x,y,z,w)           _mm_shuffle_ps(vec, vec, MakeShuffleMask(x,y,z,w))
#define VecSwizzle1(vec, x)                _mm_shuffle_ps(vec, vec, MakeShuffleMask(x,x,x,x))
// special swizzle
#define VecSwizzle_0101(vec)               _mm_movelh_ps(vec, vec)
#define VecSwizzle_2323(vec)               _mm_movehl_ps(vec, vec)
#define VecSwizzle_0022(vec)               _mm_moveldup_ps(vec)
#define VecSwizzle_1133(vec)               _mm_movehdup_ps(vec)

// return (vec1[x], vec1[y], vec2[z], vec2[w])
#define VecShuffle(vec1, vec2, x,y,z,w)    _mm_shuffle_ps(vec1, vec2, MakeShuffleMask(x,y,z,w))
// special shuffle
#define VecShuffle_0101(vec1, vec2)        _mm_movelh_ps(vec1, vec2)
#define VecShuffle_2323(vec1, vec2)        _mm_movehl_ps(vec2, vec1)

#if YOYO_MATH_SIMD
// for row major matrix
// we use __m128 to represent 2x2 matrix as A = | A0  A1 |
//                                              | A2  A3 |
// 2x2 row major Matrix multiply A*B
VM_INLINE __m128 Mat2Mul(__m128 vec1, __m128 vec2);
// 2x2 row major Matrix adjugate multiply (A#)*B
VM_INLINE __m128 Mat2AdjMul(__m128 vec1, __m128 vec2);
// 2x2 row major Matrix multiply adjugate A*(B#)
VM_INLINE __m128 Mat2MulAdj(__m128 vec1, __m128 vec2);

#else

VM_INLINE float4 Mat2Mul(float4 vec1, float4 vec2);

VM_INLINE float4 Mat2AdjMul(float4 vec1, float4 vec2);

VM_INLINE float4 Mat2MulAdj(float4 vec1, float4 vec2);

#endif
float4x4 V_CALL inverse(float4x4 in_matrix);

float4x4 V_CALL init_pers_proj_matrix(float2 buffer_dim, float fov_y = 68.0f, float2 far_near = float2(0.5f, 1000.0f));

float4x4 V_CALL init_ortho_proj_matrix(float2 size, float near_clip_plane = 0.1f, float far_clip_plane = 1000.0f);

float4x4 V_CALL init_screen_space_matrix(float2 buffer_dim);

float4x4 V_CALL set_matrix(float3 p,quaternion r,float3 s);

float3 V_CALL world_local_p(float4x4 m,float3 a);

float3 V_CALL local_world_p(float4x4 m,float3 a);

VM_INLINE float V_CALL mul(float2 x);
float3 V_CALL mul(quaternion q, float3 dir);

float3 V_CALL forward(quaternion q);

float3 V_CALL up(quaternion q);

float3 V_CALL right(quaternion q);

float3 V_CALL project_on_plane(float3 v, float3 plane_normal);

float4x4 V_CALL set_camera_view(float3 p, float3 d, float3 u);

float3 V_CALL screen_to_world_point(float4x4 projection_matrix,float4x4 cam_matrix,float2 buffer_dim, float2 screen_xy, float z_depth);

float2 V_CALL world_to_screen_point(float4x4 projection_matrix,float4x4 camera_matrix,float2 buffer_dim, float3 p);

#ifdef YOYOIMPL

#if YOYO_MATH_SIMD
// for row major matrix
// we use __m128 to represent 2x2 matrix as A = | A0  A1 |
//                                              | A2  A3 |
// 2x2 row major Matrix multiply A*B
VM_INLINE __m128 Mat2Mul(__m128 vec1, __m128 vec2)
{
    return
    _mm_add_ps(_mm_mul_ps(                     vec1, VecSwizzle(vec2, 0,3,0,3)),
               _mm_mul_ps(VecSwizzle(vec1, 1,0,3,2), VecSwizzle(vec2, 2,1,2,1)));
}

// 2x2 row major Matrix adjugate multiply (A#)*B
VM_INLINE __m128 Mat2AdjMul(__m128 vec1, __m128 vec2)
{
    return
    _mm_sub_ps(_mm_mul_ps(VecSwizzle(vec1, 3,3,0,0), vec2),
               _mm_mul_ps(VecSwizzle(vec1, 1,1,2,2), VecSwizzle(vec2, 2,3,0,1)));
    
}

// 2x2 row major Matrix multiply adjugate A*(B#)
VM_INLINE __m128 Mat2MulAdj(__m128 vec1, __m128 vec2)
{
    return
    _mm_sub_ps(_mm_mul_ps(                     vec1, VecSwizzle(vec2, 3,0,3,0)),
               _mm_mul_ps(VecSwizzle(vec1, 1,0,3,2), VecSwizzle(vec2, 2,1,2,1)));
}
#else

VM_INLINE float4 Mat2Mul(float4 vec1, float4 vec2)
{
    float4 a = vec1 * vec2.xwxw();
    float4 b = vec1.yxwz() * vec2.yxyx();
    return (a + b);
}

VM_INLINE float4 Mat2AdjMul(float4 vec1, float4 vec2)
{
    float4 a = vec1.wwxx() * vec2;
    float4 b = vec1.yyzz() * vec2.zwxy();
    return (a-b);
}

VM_INLINE float4 Mat2MulAdj(float4 vec1, float4 vec2)
{
    float4 a = vec1 * vec2.wxwx();
    float4 b = vec1.yxwz() * vec2.zyzy();
    return (a - b);
}
#endif

// Inverse function is the same no matter column major or row major
// this version treats it as row major
float4x4 V_CALL inverse(float4x4 in_matrix)
{
    // use block matrix method
    // A is a matrix, then i(A) or iA means inverse of A, A# (or A_ in code) means adjugate of A, |A| (or detA in code) is determinant, tr(A) is trace
    
    // sub matrices
#if YOYO_MATH_SIMD
    __m128 A = VecShuffle_0101(in_matrix.c0.m, in_matrix.c1.m);
    __m128 B = VecShuffle_2323(in_matrix.c0.m, in_matrix.c1.m);
    __m128 C = VecShuffle_0101(in_matrix.c2.m, in_matrix.c3.m);
    __m128 D = VecShuffle_2323(in_matrix.c2.m, in_matrix.c3.m);
#else
	//23_->01 23->01
	float4 A = float4(in_matrix.c1.zw(), in_matrix.c0.zw());//lhps(in_matrix.c0.m, in_matrix.c1.m);
 	float4 B = float4(in_matrix.c1.xy(), in_matrix.c0.xy());// hlps(in_matrix.c0.m, in_matrix.c1.m);
 	
 	float4 C = float4(in_matrix.c3.zw(), in_matrix.c2.zw());// lhps(in_matrix.c2.m, in_matrix.c3.m);
	float4 D = float4(in_matrix.c3.xy(), in_matrix.c2.xy()) ;// VecShuffle_2323(in_matrix.c2.m, in_matrix.c3.m);
#endif

#if YOYO_MATH_SIMD 	
    __m128 detA = _mm_set1_ps(in_matrix.c0.x() * in_matrix.c1.y() - in_matrix.c0.y() * in_matrix.c1.x());
    __m128 detB = _mm_set1_ps(in_matrix.c0.z() * in_matrix.c1.w() - in_matrix.c0.w() * in_matrix.c1.z());
    __m128 detC = _mm_set1_ps(in_matrix.c2.x() * in_matrix.c3.y() - in_matrix.c2.y() * in_matrix.c3.x());
    __m128 detD = _mm_set1_ps(in_matrix.c2.z() * in_matrix.c3.w() - in_matrix.c2.w() * in_matrix.c3.z());
#else
	float4 detA = float4(in_matrix.c0.x() * in_matrix.c1.y() - in_matrix.c0.y() * in_matrix.c1.x());
	float4 detB = float4(in_matrix.c0.z() * in_matrix.c1.w() - in_matrix.c0.w() * in_matrix.c1.z());
	float4 detC = float4(in_matrix.c2.x() * in_matrix.c3.y() - in_matrix.c2.y() * in_matrix.c3.x());
	float4 detD = float4(in_matrix.c2.z() * in_matrix.c3.w() - in_matrix.c2.w() * in_matrix.c3.z());
#endif

#if 0 // for determinant, float version is faster
    // determinant as (|A| |B| |C| |D|)
    __m128 detSub = _mm_sub_ps(
                               _mm_mul_ps(VecShuffle(in_matrix.c0.m, in_matrix.c2.m, 0,2,0,2), VecShuffle(in_matrix.c1.m, in_matrix.c3.m, 1,3,1,3)),
                               _mm_mul_ps(VecShuffle(in_matrix.c0.m, in_matrix.c2.m, 1,3,1,3), VecShuffle(in_matrix.c1.m, in_matrix.c3.m, 0,2,0,2))
                               );
    __m128 detA = VecSwizzle1(detSub, 0);
    __m128 detB = VecSwizzle1(detSub, 1);
    __m128 detC = VecSwizzle1(detSub, 2);
    __m128 detD = VecSwizzle1(detSub, 3);
#endif
	float4x4 r;
#if YOYO_MATH_SIMD
    // let iM = 1/|M| * | X  Y |
    //                  | Z  W |
    
    // D#C
    __m128 D_C = Mat2AdjMul(D, C);
    // A#B
    __m128 A_B = Mat2AdjMul(A, B);
    // X# = |D|A - B(D#C)
    __m128 X_ = _mm_sub_ps(_mm_mul_ps(detD, A), Mat2Mul(B, D_C));
    // W# = |A|D - C(A#B)
    __m128 W_ = _mm_sub_ps(_mm_mul_ps(detA, D), Mat2Mul(C, A_B));
    
    // |M| = |A|*|D| + ... (continue later)
    __m128 detM = _mm_mul_ps(detA, detD);
    
    // Y# = |B|C - D(A#B)#
    __m128 Y_ = _mm_sub_ps(_mm_mul_ps(detB, C), Mat2MulAdj(D, A_B));
    // Z# = |C|B - A(D#C)#
    __m128 Z_ = _mm_sub_ps(_mm_mul_ps(detC, B), Mat2MulAdj(A, D_C));
    
    // |M| = |A|*|D| + |B|*|C| ... (continue later)
    detM = _mm_add_ps(detM, _mm_mul_ps(detB, detC));
    
    // tr((A#B)(D#C))
    __m128 tr = _mm_mul_ps(A_B, VecSwizzle(D_C, 0,2,1,3));
    tr = _mm_hadd_ps(tr, tr);
    tr = _mm_hadd_ps(tr, tr);
    // |M| = |A|*|D| + |B|*|C| - tr((A#B)(D#C)
    detM = _mm_sub_ps(detM, tr);
    
    const __m128 adjSignMask = _mm_setr_ps(1.f, -1.f, -1.f, 1.f);
    // (1/|M|, -1/|M|, -1/|M|, 1/|M|)
    __m128 rDetM = _mm_div_ps(adjSignMask, detM);
    
    X_ = _mm_mul_ps(X_, rDetM);
    Y_ = _mm_mul_ps(Y_, rDetM);
    Z_ = _mm_mul_ps(Z_, rDetM);
    W_ = _mm_mul_ps(W_, rDetM);


    // apply adjugate and store, here we combine adjugate shuffle and store shuffle
    r = float4x4(float4(VecShuffle(X_, Y_, 3,1,3,1)),
                          float4(VecShuffle(X_, Y_, 2,0,2,0)),
                          float4(VecShuffle(Z_, W_, 3,1,3,1)),
                          float4(VecShuffle(Z_, W_, 2,0,2,0)));
    
#else
	// let iM = 1/|M| * | X  Y |
  //                  | Z  W |

  // D#C
	float4 D_C = Mat2AdjMul(D, C);
	// A#B
	float4 A_B = Mat2AdjMul(A, B);
	// X# = |D|A - B(D#C)
    float4 X_ = (detD *  A) - Mat2Mul(B, D_C);
	// W# = |A|D - C(A#B)
	float4 W_ = ((detA * D) - Mat2Mul(C, A_B));

	// |M| = |A|*|D| + ... (continue later)
	float4 detM = (detA * detD);

	// Y# = |B|C - D(A#B)#
	float4 Y_ = ((detB * C) - Mat2MulAdj(D, A_B));
	// Z# = |C|B - A(D#C)#
	float4 Z_ = ((detC * B) - Mat2MulAdj(A, D_C));

	// |M| = |A|*|D| + |B|*|C| ... (continue later)
	detM = (detM + (detB * detC));

	// tr((A#B)(D#C))
	float4 tr = (A_B * (D_C.xzyw()));

    tr = HorizontalAdd(tr,tr);
    tr = HorizontalAdd(tr,tr);

	// |M| = |A|*|D| + |B|*|C| - tr((A#B)(D#C)
    detM = (detM - tr);

	const float4 adjSignMask = float4(1.0f,-1.0f,-1.0f,1.0f);
	// (1/|M|, -1/|M|, -1/|M|, 1/|M|)
	float4 rDetM = (adjSignMask / detM);

	X_ = (X_ * rDetM);
	Y_ = (Y_ * rDetM);
	Z_ = (Z_ * rDetM);
	W_ = (W_ * rDetM);

	// apply adjugate and store, here we combine adjugate shuffle and store shuffle
    float4 c0 = float4(X_.wy(), Y_.wy());//, 3, 1, 3, 1));
    float4 c1 = float4(X_.zx(), Y_.zx());//, 2, 0, 2, 0);
    float4 c2 = float4(Z_.wx(), W_.wx());//, 3, 1, 3, 1);
    float4 c3 = float4(Z_.zx(), W_.zx());//, 2, 0, 2, 0);
	r = float4x4((c0),
		float4(c1),
		float4(c2),
		float4(c3));
#endif
 	return r;
}
//END INVERSE

float4x4 V_CALL init_pers_proj_matrix(float2 buffer_dim, float fov_y , float2 far_near)
{
    float near_clip_plane = far_near.x();//0.5f;
    float far_clip_plane = far_near.y();//1000.0f;
	float tangent = tanf((fov_y / 2) * DEG2RAD); // tangent of half fovY
	float aspect_ratio = buffer_dim.x() / buffer_dim.y();
	float z_depth_range = far_clip_plane - near_clip_plane;

    //NOTE(Ray):safe ratios were in old math lib consider migrating themto new lib
    /*
	float a = SafeRatio1(1.0f, tangent * aspect_ratio);
	float b = SafeRatio1(1.0f, tangent);
	float Z = -SafeRatio1(far_clip_plane + near_clip_plane, z_depth_range);
	float z2 = -SafeRatio1(2.0f * far_clip_plane * near_clip_plane, z_depth_range);
    */
    float a = 1.0f / (tangent * aspect_ratio);
    float b = 1.0f / tangent;
    float z = -((far_clip_plane + near_clip_plane) / z_depth_range);
    float z2 = -((2.0f * far_clip_plane * near_clip_plane) / z_depth_range);
	float4x4 result = float4x4(a, b, z,0);

    result.c2.setW(-1);
	result.c3.setX(0);
	result.c3.setY(0);
	result.c3.setZ(z2);
	return result;
}

float4x4 V_CALL init_ortho_proj_matrix(float2 size, float near_clip_plane , float far_clip_plane)
{
	float r = size.x();
	float l = -r;
	float t = size.y();
	float b = -t;
	float zero = 2.0f / (r - l);//SafeRatio1(2.0f, r - l);
	float five = 2.0f / (t - b);//SafeRatio1(2.0f, t - b);
	float ten = -2.0f / (far_clip_plane - near_clip_plane);//SafeRatio1(-2.0f, far_clip_plane - near_clip_plane);

	float4x4 result = float4x4(zero, five, ten,1);

	result.c3.setX(-((r + l)  / (r - l)));
	result.c3.setY(-((t + b)  / (t - b)));
	result.c3.setZ( 0 );
	return result;
}

float4x4 V_CALL init_screen_space_matrix(float2 buffer_dim)
{
	float2 ab = 2.0f / buffer_dim;
	float4x4 result = float4x4(ab.x(), ab.y(), 1,1);
	result.c3.setX(-1);
	result.c3.setY(-1);
	return result;
}

 float4x4 V_CALL set_matrix(float3 p,quaternion r,float3 s)
{
	float4x4 rotation = float4x4(r,p);
	float4x4 scale_matrix = scale(s);
	return mul(rotation,scale_matrix);
}

float3 V_CALL world_local_p(float4x4 m,float3 a)
{
    return (inverse(m) * float4(a,1)).xyz();
}

 float3 V_CALL local_world_p(float4x4 m,float3 a)
{
    return (m * float4(a,1)).xyz();
}

VM_INLINE float V_CALL mul(float2 x);

VM_INLINE float V_CALL mul(float2 x)
{
	return x.x() * x.y();
}


float3 V_CALL mul(quaternion q, float3 dir)
{
	float3 qv = q.xyz();
	float3 t = 2 * cross(dir,qv);
	return dir + q.w() * t + cross(t, qv);
}

 float3 V_CALL forward(quaternion q)
{
	return normalize(mul(q, float3(0, 0, 1)));
}

 float3 V_CALL up(quaternion q)
{
	return normalize(mul(q, float3(0, 1, 0)));
}

 float3 V_CALL right(quaternion q)
{
	return normalize(mul(q, float3(1, 0, 0)));
}

 float3 V_CALL project_on_plane(float3 v, float3 plane_normal)
{
	return float4(v - dot(v, plane_normal) * plane_normal,0).xyz();
}

 float4x4 V_CALL set_camera_view(float3 p, float3 d, float3 u)
{
	float3 cam_right = (cross(u, d));
	float3 cam_up = (cross(d, cam_right));
	d = normalize(d);
 
	return (float4x4(cam_right.x(),     cam_up.x(),     d.x(),     0,
                     cam_right.y(),     cam_up.y(),     d.y(),     0,
                     cam_right.z(),     cam_up.z(),     d.z(),     0,
                    -dot(cam_right, p),-dot(cam_up, p),-dot(d, p),1.0f));
}

 float3 V_CALL screen_to_world_point(float4x4 projection_matrix,float4x4 cam_matrix,float2 buffer_dim, float2 screen_xy, float z_depth)
{
	float4x4 pc_mat = mul(projection_matrix,cam_matrix);
	float4x4 inv_pc_mat = transpose(inverse(pc_mat));
	float4 p = float4(
        2.0f * screen_xy.x() / buffer_dim.x() - 1.0f,
        2.0f * screen_xy.y() / buffer_dim.y() - 1.0f,
        z_depth,
        1.0f);

	float4 w_div = mul(p , inv_pc_mat);
	float w = safe_ratio_zero(1.0f, w_div.w());
    return w_div.xyz() * w;
}

 float2 V_CALL world_to_screen_point(float4x4 projection_matrix,float4x4 camera_matrix,float2 buffer_dim, float3 p)
{
	float4 input_p = float4(p,1.0f);
    float aspect_ratio = buffer_dim.x() / buffer_dim.y();
	float4x4 view_projection_matrix = mul(projection_matrix , camera_matrix);
	float4 clip = mul(input_p,view_projection_matrix);
	//w divide value should be z of output.
	clip.setW(clip.z());

	 //TODO(Ray):Use swizzlers here to help cut down on operations
	//float3 div_w = clip.xyz() / clip.www();
	float3 NDC = float3(
        (safe_ratio_zero( clip.x() , clip.w()) + 1) * aspect_ratio,
    	(safe_ratio_zero( clip.y() , clip.w()) + 1) * aspect_ratio,
         safe_ratio_zero( clip.z() , clip.w()));

	float2 Result = float2( NDC.x() * safe_ratio_zero(buffer_dim.x() , NDC.z()),
                           NDC.y() *  safe_ratio_zero(buffer_dim.y() , NDC.z()));
//	Result.x = NDC.x * buffer_dim.x / NDC.z;
//	Result.y = NDC.y * buffer_dim.y / NDC.z;
	return Result;
}

#endif//YOYOIMPL
 /*
  *	function GetBias(time,bias)
{
  return (time / ((((1.0/bias) - 2.0)*(1.0 - time))+1.0));
}

function GetGain(time,gain)
{
  if(time < 0.5)
    return GetBias(time * 2.0,gain)/2.0;
  else
    return GetBias(time * 2.0 - 1.0,1.0 - gain)/2.0 + 0.5;
}
  */
#define YOYO_MATH_H
#endif
