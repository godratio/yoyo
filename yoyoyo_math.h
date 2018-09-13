#pragma once
//TODO(Ray):almost all of this is untested
#include <stdint.h>
#include <math.h>
#include <xmmintrin.h>
#if OSX
#include <x86intrin.h>
#elif WINDOWS
#include <intrin.h>
#endif
//TODO(Ray):look up proper define for compilers 
#if WINDOWS
#define VM_INLINE   __forceinline
#define V_CALL __vectorcall
#else
#define VM_INLINE __attribute__((always_inline))
#define V_CALL __vectorcall
#endif

#define M_PI        3.14159265358979323846f
#define M_TAO       6.28318530717f

#define DEG2RAD   M_PI/(float)180
#define RAD2DEG   (float)180 / M_PI

#define DEGREE_TO_RADIAN(a) a * DEG2RAD
#define RADIAN_TO_ANGLE(a)  a * RAD2DEG

//#define DEG2RAD(_a) ((_a)*M_PI/180.0f)
//#define RAD2DEG(_a) ((_a)*180.0f/M_PI)
#define U16MAX 65535
#define INT_MIN     (-2147483647 - 1)
#define INT_MAX     2147483647
#define U32MAX      ((u32)-1)
#define FLT_MAX     3.402823466e+38F
//#define FLT_MIN     -FLT_MAX
#define POSITIVE_INFINITY  INFINITY;
#define POSITIVE_INFINITY2  float2(INFINITY,INFINITY);
#define POSITIVE_INFINITY3  float3(INFINITY,INFINITY,INFINITY);
#define POSITIVE_INFINITY4  float4(INFINITY,INFINITY,INFINITY,INFINITY);

// Shuffle helpers.
// Examples: SHUFFLE3(v, 0,1,2) leaves the vector unchanged.
//           SHUFFLE3(v, 0,0,0) splats the X coord out.
#define SHUFFLEN(V, X,Y) floatn(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(Y,Y,Y,X)))

#define SHUFFLE2(V, X,Y) float2(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(Y,Y,Y,X)))
#define SHUFFLE3(V, X,Y,Z) float3(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(Z,Z,Y,X)))
#define SHUFFLE4(V, X,Y,Z,W) float4(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(W,Z,Y,X)))

//Shuffle without dereferencing the this pointer
#define SHUFFLE2m(m, X,Y) float2(_mm_shuffle_ps(m, m, _MM_SHUFFLE(Y,Y,Y,X)))
#define SHUFFLE3m(m, X,Y,Z) float3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(Z,Z,Y,X)))
#define SHUFFLE4m(m, X,Y,Z,W) float4(_mm_shuffle_ps(m, (m, _MM_SHUFFLE(W,Z,Y,X)))
//NOTE(Ray):Used only for making pointers to memory that correlates to one of our math types.
union float2data
{
	struct
	{
		float x, y;
	};
	float indexed[2] = {};
};

union float3data
{
	struct
	{
		float x, y, z;
	};
	float indexed[3] = {};
};

struct float4data
{
	float x, y, z, w;
};


struct float4;


struct  float2
{
    __m128 m;
    // Constructors.
    VM_INLINE float2() {}
    VM_INLINE explicit V_CALL float2(const float *p) { m = _mm_set_ps(p[1], p[1], p[1], p[0]); }
    VM_INLINE explicit V_CALL float2(float x) { m = _mm_set_ps(x, x, x, x); }
    VM_INLINE explicit V_CALL float2(float x, float y) { m = _mm_set_ps(y, y, y, x); }
    VM_INLINE explicit V_CALL float2(__m128 v) { m = v; }

    VM_INLINE float V_CALL x() const { return _mm_cvtss_f32(m); }
    VM_INLINE float V_CALL y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }

	VM_INLINE float V_CALL u() const { return _mm_cvtss_f32(m); }
	VM_INLINE float V_CALL v() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }

	VM_INLINE float2 V_CALL yx() const { return SHUFFLE2(*this, 1, 0); }
    VM_INLINE float2 V_CALL xy() const { return SHUFFLE2(*this, 0, 1); }

	VM_INLINE float2 V_CALL vu() const { return SHUFFLE2(*this, 1, 0); }
	VM_INLINE float2 V_CALL uv() const { return SHUFFLE2(*this, 0, 1); }
	float4 V_CALL xyxy() const;
    //NOTE(Ray):Use these as lil as possible
#if WINDOWS
    VM_INLINE float operator[] (size_t i) const { return m.m128_f32[i]; };
	VM_INLINE float& operator[] (size_t i) { return m.m128_f32[i]; };
#elif OSX || IOS
    VM_INLINE float operator[] (size_t i) const { return m[i]; };
   // VM_INLINE float& operator[] (size_t i) { return m[i]; };
#endif
    VM_INLINE void store(float *p) const { p[0] = x(); p[1] = y(); }
	VM_INLINE float* to_array() { float result[2] = { x(),y()}; return result; }
    void setX(float x)
    {
        m = _mm_move_ss(m, _mm_set_ss(x));
    }
    void V_CALL setY(float y)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(y));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
        m = _mm_move_ss(t, m);
    }
	static uint32_t V_CALL size() { return sizeof(float) * 2; }
    //VM_INLINE float operator[] (size_t i) const { return m.m128_f32[i]; };
    //VM_INLINE float& operator[] (size_t i) { return m.m128_f32[i]; };
    //VM_INLINE float3 float3i(int x, int y, int z) { return float3((float)x, (float)y, (float)z); }
};

struct float3
{
    __m128 m;
    // Constructors.
    VM_INLINE float3() {}
    VM_INLINE explicit V_CALL float3(const float *p) { m = _mm_set_ps(p[2], p[2], p[1], p[0]); }
    VM_INLINE explicit V_CALL float3(float x) { m = _mm_set_ps(x, x, x, x); }
	VM_INLINE explicit V_CALL float3(float2 xy, float z) { m = _mm_set_ps(z, z, xy.y(), xy.x()); }
    VM_INLINE explicit V_CALL float3(float x, float y, float z) { m = _mm_set_ps(z, z, y, x); }
    VM_INLINE explicit V_CALL float3(__m128 v) { m = v; }

    VM_INLINE float V_CALL x() const { return _mm_cvtss_f32(m); }
    VM_INLINE float V_CALL y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    VM_INLINE float V_CALL z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
	VM_INLINE float2 V_CALL xy() const { return SHUFFLE2(*this,0, 1); }
    VM_INLINE float3 V_CALL yzx() const { return SHUFFLE3(*this, 1, 2, 0); }
    VM_INLINE float3 V_CALL zxy() const { return SHUFFLE3(*this, 2, 0, 1); }

    //NOTE(Ray):Use these as lil as possible
#if WINDOWS
    VM_INLINE float V_CALL operator[] (size_t i) const { return m.m128_f32[i]; }

#ifdef YOYO_USE_PHYSX_EXT
	physx::PxVec3 float3::toPhysx();
#endif
	VM_INLINE float& V_CALL operator[] (size_t i) { return m.m128_f32[i]; };
	VM_INLINE float* V_CALL to_array() { return m.m128_f32; }
#else
#endif
    VM_INLINE void V_CALL store(float *p) const { p[0] = x(); p[1] = y(); p[2] = z(); }
	float4 V_CALL xyxy() const;

    void V_CALL setX(float x)
    {
        m = _mm_move_ss(m, _mm_set_ss(x));
    }
    void V_CALL setY(float y)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(y));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
        m = _mm_move_ss(t, m);
    }
    void V_CALL setZ(float z)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(z));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
        m = _mm_move_ss(t, m);
    }
	static uint32_t size() { return sizeof(float) * 3; }
    //VM_INLINE float operator[] (size_t i) const { return m.m128_f32[i]; };
    //VM_INLINE float& operator[] (size_t i) { return m.m128_f32[i]; };
    //VM_INLINE float3 float3i(int x, int y, int z) { return float3((float)x, (float)y, (float)z); }
};

struct float4
{
    __m128 m;
    // Constructors.
    VM_INLINE V_CALL float4() {}
    VM_INLINE explicit V_CALL float4(const float *p) { m = _mm_set_ps(p[3], p[2], p[1], p[0]); }
    VM_INLINE explicit V_CALL float4(float x, float y, float z,float w) { m = _mm_set_ps(w, z, y, x); }
    VM_INLINE explicit V_CALL float4(float x) { m = _mm_set_ps(x, x, x, x); }
    VM_INLINE explicit V_CALL float4(__m128 v) { m = v; }
    VM_INLINE explicit V_CALL float4(float3 a,float b){m = _mm_set_ps(b,a.z(),a.y(),a.x());}
    VM_INLINE explicit V_CALL float4(float2 a,float2 b){m = _mm_set_ps(b.y(),b.x(),a.y(),a.x());}

    VM_INLINE float V_CALL x() const { return _mm_cvtss_f32(m); }
    VM_INLINE float V_CALL y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    VM_INLINE float V_CALL z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
    VM_INLINE float V_CALL w() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3))); }

	VM_INLINE float V_CALL r() const { return _mm_cvtss_f32(m); }
	VM_INLINE float V_CALL g() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
	VM_INLINE float V_CALL b() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
	VM_INLINE float V_CALL a() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3))); }

	VM_INLINE float V_CALL left() const { return _mm_cvtss_f32(m); }
	VM_INLINE float V_CALL right() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
	VM_INLINE float V_CALL top() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
	VM_INLINE float V_CALL bottom() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3))); }

    VM_INLINE float3 V_CALL xyz() const { return SHUFFLE3(*this, 0, 1, 2); }
	VM_INLINE float3 V_CALL xzy() const { return SHUFFLE3(*this, 1, 2, 0); }
	
	VM_INLINE float3 V_CALL zxy() const { return SHUFFLE3(*this, 2, 0, 1); }
	VM_INLINE float3 V_CALL zyx() const { return SHUFFLE3(*this, 2, 1, 0); }

	VM_INLINE float3 V_CALL yzx() const { return SHUFFLE3(*this, 1, 2, 0); }
	VM_INLINE float3 V_CALL yxz() const { return SHUFFLE3(*this, 1, 0, 2); }

	VM_INLINE float3 V_CALL yxw() const { return SHUFFLE3(*this, 1, 0, 3); }
	VM_INLINE float3 V_CALL wzy() const { return SHUFFLE3(*this, 3, 2, 1); }
	VM_INLINE float3 V_CALL zwx() const { return SHUFFLE3(*this, 2, 3, 0); }

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
	VM_INLINE float4 V_CALL xyxy() const { return SHUFFLE4(*this, 0, 1, 0, 1); }
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

	//NOTE(Ray):Should avoid using these whenever possible
#if WINDOWS
	VM_INLINE float V_CALL operator[] (size_t i) const { return m.m128_f32[i]; };
	VM_INLINE float& V_CALL operator[] (size_t i) { return m.m128_f32[i]; };
		VM_INLINE float* V_CALL to_array() { return m.m128_f32; }
#else

#endif

    VM_INLINE void V_CALL store(float *p) const { p[0] = x(); p[1] = y(); p[2] = z(); p[3] = w(); }

    void V_CALL setX(float x)
    {
        m = _mm_move_ss(m, _mm_set_ss(x));
    }
    void V_CALL setY(float y)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(y));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
        m = _mm_move_ss(t, m);
    }
    void V_CALL setZ(float z)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(z));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
        m = _mm_move_ss(t, m);
    }
    void V_CALL setW(float w)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(w));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(0, 2, 1, 0));
        m = _mm_move_ss(t, m);
    }
	static uint32_t size() { return sizeof(float) * 4; }
};

VM_INLINE float4 V_CALL float2::xyxy() const { return SHUFFLE4(*this, 0, 1, 0, 1); }
VM_INLINE float4 V_CALL float3::xyxy() const { return SHUFFLE4(*this, 0, 1, 0, 1); }

typedef float2 bool2;
VM_INLINE float2 V_CALL operator+ (float2 a, float2 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
VM_INLINE float2 V_CALL operator+ (float2 a, float b) { a.m = _mm_add_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float2 V_CALL operator+ (float  a, float2 b) { b.m = _mm_add_ps( _mm_set1_ps(a),b.m); return b; }

VM_INLINE float2 V_CALL operator- (float2 a, float2 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
VM_INLINE float2 V_CALL operator- (float2 a, float b) { a.m = _mm_sub_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float2 V_CALL operator- (float  a, float2 b) { b.m = _mm_sub_ps( _mm_set1_ps(a),b.m); return b; }

VM_INLINE float2 V_CALL operator* (float2 a, float2 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
VM_INLINE float2 V_CALL operator/ (float2 a, float2 b) { a.m = _mm_div_ps(a.m, b.m); return a; }
VM_INLINE float2 V_CALL operator* (float2 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float2 V_CALL operator/ (float2 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float2 V_CALL operator* (float a, float2 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }
VM_INLINE float2 V_CALL operator/ (float a, float2 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }
VM_INLINE float2& V_CALL operator+= (float2 &a, float2 b) { a = a + b; return a; }
VM_INLINE float2& V_CALL operator-= (float2 &a, float2 b) { a = a - b; return a; }
VM_INLINE float2& V_CALL operator*= (float2 &a, float2 b) { a = a * b; return a; }
VM_INLINE float2& V_CALL operator/= (float2 &a, float2 b) { a = a / b; return a; }
VM_INLINE float2& V_CALL operator*= (float2 &a, float b) { a = a * b; return a; }
VM_INLINE float2& V_CALL operator/= (float2 &a, float b) { a = a / b; return a; }
VM_INLINE bool2 V_CALL operator==(float2 a, float2 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
VM_INLINE bool2 V_CALL operator!=(float2 a, float2 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
VM_INLINE bool2 V_CALL operator< (float2 a, float2 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
VM_INLINE bool2 V_CALL operator> (float2 a, float2 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
VM_INLINE bool2 V_CALL operator<=(float2 a, float2 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
VM_INLINE bool2 V_CALL operator>=(float2 a, float2 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
VM_INLINE float2 V_CALL operator- (float2 a) { return float2(_mm_setzero_ps()) - a; }
//VM_INLINE float2 abs(float2 v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }
// Returns a 3-bit code where bit0..bit2 is X..Z
VM_INLINE unsigned V_CALL mask(float2 v) { return _mm_movemask_ps(v.m) & 7; }
// Once we have a comparison, we can branch based on its results:
VM_INLINE bool V_CALL any(bool2 v) { return mask(v) != 0; }
VM_INLINE bool V_CALL all(bool2 v) { return mask(v) == 7; }

typedef float3 bool3;
VM_INLINE float3 V_CALL operator+ (float3 a, float3 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
VM_INLINE float3 V_CALL operator+ (float3 a, float b) { a.m = _mm_add_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float3 V_CALL operator+ (float  a, float3 b) { b.m = _mm_add_ps( _mm_set1_ps(a),b.m); return b; }

VM_INLINE float3 V_CALL operator- (float3 a, float3 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
VM_INLINE float3 V_CALL operator- (float3 a, float b) { a.m = _mm_sub_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float3 V_CALL operator- (float  a, float3 b) { b.m = _mm_sub_ps( _mm_set1_ps(a),b.m); return b; }

VM_INLINE float3 V_CALL operator* (float3 a, float3 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
VM_INLINE float3 V_CALL operator/ (float3 a, float3 b) { a.m = _mm_div_ps(a.m, b.m); return a; }
VM_INLINE float3 V_CALL operator* (float3 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float3 V_CALL operator/ (float3 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float3 V_CALL operator* (float a, float3 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }
VM_INLINE float3 V_CALL operator/ (float a, float3 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }
VM_INLINE float3& V_CALL operator+= (float3 &a, float3 b) { a = a + b; return a; }
VM_INLINE float3& V_CALL operator-= (float3 &a, float3 b) { a = a - b; return a; }
VM_INLINE float3& V_CALL operator*= (float3 &a, float3 b) { a = a * b; return a; }
VM_INLINE float3& V_CALL operator/= (float3 &a, float3 b) { a = a / b; return a; }
VM_INLINE float3& V_CALL operator*= (float3 &a, float b) { a = a * b; return a; }
VM_INLINE float3& V_CALL operator/= (float3 &a, float b) { a = a / b; return a; }
VM_INLINE bool3 V_CALL operator==(float3 a, float3 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
VM_INLINE bool3 V_CALL operator!=(float3 a, float3 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
VM_INLINE bool3 V_CALL operator< (float3 a, float3 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
VM_INLINE bool3 V_CALL operator> (float3 a, float3 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
VM_INLINE bool3 V_CALL operator<=(float3 a, float3 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
VM_INLINE bool3 V_CALL operator>=(float3 a, float3 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
VM_INLINE float3 V_CALL operator- (float3 a) { return float3(_mm_setzero_ps()) - a; }
//VM_INLINE float3 abs(float3 v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }
// Returns a 3-bit code where bit0..bit2 is X..Z
VM_INLINE unsigned V_CALL mask(float3 v) { return _mm_movemask_ps(v.m) & 7; }
// Once we have a comparison, we can branch based on its results:
VM_INLINE bool V_CALL any(bool3 v) { return mask(v) != 0; }
VM_INLINE bool V_CALL all(bool3 v) { return mask(v) == 7; }

typedef float4 bool4;
VM_INLINE float4 V_CALL operator+ (float4 a, float4 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
VM_INLINE float4 V_CALL operator+ (float4 a, float b) { a.m = _mm_add_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float4 V_CALL operator+ (float  a, float4 b) { b.m = _mm_add_ps( _mm_set1_ps(a),b.m); return b; }

VM_INLINE float4 V_CALL operator- (float4 a, float4 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
VM_INLINE float4 V_CALL operator- (float4 a, float b) { a.m = _mm_sub_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float4 V_CALL operator- (float  a, float4 b) { b.m = _mm_sub_ps( _mm_set1_ps(a),b.m); return b; }

VM_INLINE float4 V_CALL operator* (float4 a, float4 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
VM_INLINE float4 V_CALL operator* (float4 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float4 V_CALL operator* (float a, float4 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }

VM_INLINE float4 V_CALL operator/ (float4 a, float4 b) { a.m = _mm_div_ps(a.m, b.m); return a; }
VM_INLINE float4 V_CALL operator/ (float4 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float4 V_CALL operator/ (float a, float4 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }

VM_INLINE float4& V_CALL operator+= (float4 &a, float4 b) { a = a + b; return a; }
VM_INLINE float4& V_CALL operator-= (float4 &a, float4 b) { a = a - b; return a; }

VM_INLINE float4& V_CALL operator*= (float4 &a, float4 b) { a = a * b; return a; }
VM_INLINE float4& V_CALL operator*= (float4 &a, float b) { a = a * b; return a; }

VM_INLINE float4& V_CALL operator/= (float4 &a, float4 b) { a = a / b; return a; }
VM_INLINE float4& V_CALL operator/= (float4 &a, float b) { a = a / b; return a; }

VM_INLINE bool4 V_CALL operator==(float4 a, float4 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
VM_INLINE bool4 V_CALL operator!=(float4 a, float4 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
VM_INLINE bool4 V_CALL operator< (float4 a, float4 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
VM_INLINE bool4 V_CALL operator> (float4 a, float4 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
VM_INLINE bool4 V_CALL operator<=(float4 a, float4 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
VM_INLINE bool4 V_CALL operator>=(float4 a, float4 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
VM_INLINE float4 V_CALL operator- (float4 a) { return float4(_mm_setzero_ps()) - a; }
VM_INLINE unsigned V_CALL mask(float4 v) { return _mm_movemask_ps(v.m) & 7; }
VM_INLINE bool V_CALL any(bool4 v) { return mask(v) != 0; }
VM_INLINE bool V_CALL all(bool4 v) { return mask(v) == 7; }

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
//HLSL INT TYPES
typedef uint32_t uint;
struct uint2
{
    uint x;
    uint y;
	VM_INLINE explicit V_CALL uint2(uint a) { this->x = a; this->y = a;}
	VM_INLINE explicit V_CALL uint2(uint a, uint b, uint c) { this->x = a; this->y = b; }
    VM_INLINE explicit V_CALL uint2(float2 a){this->x = (uint)a.x();this->y = (uint)a.y();}
    VM_INLINE explicit V_CALL uint2(float a,float b){this->x = (uint)a;this->y = (uint)b;}
};
struct uint3
{
    uint x;
    uint y;
    uint z;
	VM_INLINE explicit V_CALL uint3(uint a) { this->x = a; this->y = a; this->z = a; }
	VM_INLINE explicit V_CALL uint3(uint a,uint b,uint c) { this->x = a; this->y = b; this->z = c; }
    VM_INLINE explicit V_CALL uint3(float3 a){this->x = (uint)a.x();this->y = (uint)a.y();this->z = (uint)a.z();}
    VM_INLINE explicit V_CALL uint3(float a,float b,float c){this->x = (uint)a;this->y = (uint)b;this->z = (uint)c;}
};
struct uint4
{
    uint x;
    uint y;
    uint z;
    uint w;
	VM_INLINE explicit V_CALL uint4(uint a) { this->x = a; this->y = a; this->z = a; this->w = a; }
	VM_INLINE explicit V_CALL uint4(uint a, uint b, uint c,uint d) { this->x = a; this->y = b; this->z = c; this->w = d; }
    VM_INLINE explicit V_CALL uint4(float4 a){this->x = (uint)a.x();this->y = (uint)a.y();this->z = (uint)a.z();this->w = (uint)a.w();}
    VM_INLINE explicit V_CALL uint4(float a,float b,float c,float d){this->x = (uint)a;this->y = (uint)b;this->z = (uint)c;this->w = (uint)d;}
};

VM_INLINE uint2 V_CALL operator & (uint2 lhs, uint2 rhs) { return uint2 (lhs.x & rhs.x, lhs.y & rhs.y); }
VM_INLINE uint2 V_CALL operator & (uint2 lhs, uint rhs)  { return uint2 (lhs.x & rhs, lhs.y & rhs); }
VM_INLINE uint2 V_CALL operator & (uint lhs, uint2 rhs)  { return uint2 (lhs & rhs.x, lhs & rhs.y); }

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

VM_INLINE float3 V_CALL cross(float3 a, float3 b)
{
	return float3(a.y() * b.z() - a.z() * b.y(),
		          a.z() * b.x() - a.x() * b.z(),
		          a.x() * b.y() - a.y() * b.x());
}

//VM_INLINE float3 V_CALL cross(float3 x, float3 y) { return (x * y.yzx() - x.yzx() * y).yzx(); }
/*
VM_INLINE float3 V_CALL cross(float3 a, float3 b)
{
    // x  <-  a.y*b.z - a.z*b.y
    // y  <-  a.z*b.x - a.x*b.z
    // z  <-  a.x*b.y - a.y*b.x
    // We can save a shuffle by grouping it in this wacky order:
    return (a.zxy()*b - a*b.zxy()).zxy();
}
*/

VM_INLINE float4 V_CALL minimum(float4 a, float4 b) { a.m = _mm_min_ps(a.m, b.m); return a; }
VM_INLINE float4 V_CALL maximum(float4 a, float4 b) { a.m = _mm_max_ps(a.m, b.m); return a; }

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
VM_INLINE float2 V_CALL clamp(float2 t, float2 a, float2 b) { return minimum(maximum(t, a), b); }
VM_INLINE float3 V_CALL clamp(float3 t, float3 a, float3 b) { return minimum(maximum(t, a), b); }
VM_INLINE float4 V_CALL clamp(float4 t, float4 a, float4 b) { return minimum(maximum(t, a), b); }

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

VM_INLINE float V_CALL  rsqrt(float a)  { return 1.0f / sqrt(a); }
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
VM_INLINE bool2 V_CALL isnan(float2 x) { return (asuint(x) & 0x7FFFFFFF) > 0x7F800000; }
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
VM_INLINE float2 V_CALL saturate(float2 x) { return clamp(x, float2(0.0f), float2(1.0f)); }
VM_INLINE float3 V_CALL saturate(float3 x) { return clamp(x, float3(0.0f), float3(1.0f)); }
VM_INLINE float4 V_CALL saturate(float4 x) { return clamp(x, float4(0.0f), float4(1.0f)); }

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

VM_INLINE float  V_CALL select(float  a, float  b, bool c) { return c ? b : a; }
VM_INLINE float2 V_CALL select(float2 a, float2 b, bool c) { return c ? b : a; }
VM_INLINE float3 V_CALL select(float3 a, float3 b, bool c) { return c ? b : a; }
VM_INLINE float4 V_CALL select(float4 a, float4 b, bool c) { return c ? b : a; }

VM_INLINE float2 V_CALL reflect(float2 i, float2 n) { return i - 2.0f * n * dot(i, n); }
VM_INLINE float3 V_CALL reflect(float3 i, float3 n) { return i - 2.0f * n * dot(i, n); }
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
 
struct quaternion;
struct float3x3
{
    float4 c0;
    float4 c1;
    float4 c2;
    VM_INLINE V_CALL float3x3() {}
    VM_INLINE explicit V_CALL float3x3(float a, float b, float c) { this->c0.setX(a);this->c1.setY(b);this->c2.setZ(c);}
    VM_INLINE explicit V_CALL float3x3(float3 c0, float3 c1, float3 c2) { this->c0 = float4(c0,0);this->c1 = float4(c1,0);this->c2 = float4(c2,0);}
    VM_INLINE explicit V_CALL float3x3(float m00, float m01, float m02,
                                       float m10, float m11, float m12, 
                                       float m20, float m21, float m22)
    {
        this->c0 = float4(m00, m01, m02, 0);
        this->c1 = float4(m10, m11, m12, 0);
        this->c2 = float4(m20, m21, m22, 0);
    }
	V_CALL float3x3(quaternion rotation);
};

struct float4x4
{
    float4 c0;
    float4 c1;
    float4 c2;
    float4 c3;
    VM_INLINE V_CALL float4x4() {}
	VM_INLINE explicit V_CALL float4x4(float a, float b, float c, float d)
    {
        this->c0 = float4(0.0f); this->c0.setX(a); 
        this->c1 = float4(0.0f); this->c1.setY(b);
		this->c2 = float4(0.0f); this->c2.setZ(c); 
		this->c3 = float4(0.0f); this->c3.setW(d);
    }
    VM_INLINE explicit V_CALL float4x4(float4 c0, float4 c1, float4 c2,float4 c3) { this->c0 = c0;this->c1 = c1;this->c2 = c2;this->c3 = c3; }
    VM_INLINE explicit V_CALL float4x4(float m00, float m01, float m02, float m03,
                                float m10, float m11, float m12, float m13,
                                float m20, float m21, float m22, float m23,
                                float m30, float m31, float m32, float m33)
    { 
        this->c0 = float4(m00, m01, m02, m03);
        this->c1 = float4(m10, m11, m12, m13);
        this->c2 = float4(m20, m21, m22, m23);
        this->c3 = float4(m30, m31, m32, m33);
    }

    VM_INLINE explicit V_CALL float4x4(float3x3 rotation, float3 translation)
    {
        c0 = (rotation.c0);
        c1 = (rotation.c1);
        c2 = (rotation.c2);
        c3 = float4(translation,1.0f);
    }

	VM_INLINE static float4x4 V_CALL float4x4::identity() { return float4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); };
	VM_INLINE static float4x4 V_CALL float4x4::zero() { return float4x4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); }

	V_CALL float4x4(quaternion rotation,float3 translation);
};

/*
YoyoAString YoyoFloat4x4ToString()
        {
            return string.Format("float4x4({0}f, {1}f, {2}f, {3}f,  {4}f, {5}f, {6}f, {7}f,  {8}f, {9}f, {10}f, {11}f,  {12}f, {13}f, {14}f, {15}f)", c0.x, c1.x, c2.x, c3.x, c0.y, c1.y, c2.y, c3.y, c0.z, c1.z, c2.z, c3.z, c0.w, c1.w, c2.w, c3.w);
        }
*/

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

//mod
//VM_INLINE float4x4 operator % (float4x4 lhs, float4x4 rhs) { return new float4x4 (lhs.c0 % rhs.c0, lhs.c1 % rhs.c1, lhs.c2 % rhs.c2, lhs.c3 % rhs.c3); }
//VM_INLINE float4x4 operator % (float4x4 lhs, float rhs) { return new float4x4 (lhs.c0 % rhs, lhs.c1 % rhs, lhs.c2 % rhs, lhs.c3 % rhs); }
//VM_INLINE float4x4 operator % (float lhs, float4x4 rhs) { return new float4x4 (lhs % rhs.c0, lhs % rhs.c1, lhs % rhs.c2, lhs % rhs.c3); }
//inc
//VM_INLINE float4x4 operator ++ (float4x4 val) { return new float4x4 (++val.c0, ++val.c1, ++val.c2, ++val.c3); }
//dec
//VM_INLINE float4x4 operator -- (float4x4 val) { return new float4x4 (--val.c0, --val.c1, --val.c2, --val.c3); }

struct quaternion
{
    __m128 m;
    // Constructors.
    VM_INLINE V_CALL quaternion() {}
    VM_INLINE explicit V_CALL quaternion(const float *p) { m = _mm_set_ps(p[3], p[2], p[1], p[0]); }
    VM_INLINE explicit V_CALL quaternion(float x, float y, float z,float w) { m = _mm_set_ps(w, z, y, x); }
    VM_INLINE explicit V_CALL quaternion(float4 x) { m = _mm_set_ps(x.w(), x.z(), x.y(), x.x()); }
    
    VM_INLINE explicit V_CALL quaternion(float x) { m = _mm_set_ps(x, x, x, x); }
    VM_INLINE explicit V_CALL quaternion(__m128 v) { m = v; }
    VM_INLINE explicit V_CALL quaternion(float3 a,float b){m = _mm_set_ps(a.x(),a.y(),a.x(),b);}
    VM_INLINE explicit V_CALL quaternion(float2 a,float2 b){m = _mm_set_ps(a.x(),a.y(),b.x(),b.y());}

//The matrix must be orthonormal.
    VM_INLINE explicit V_CALL quaternion(float3x3 m)
    {
        quaternion value;
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
    }

        // Construct unit quaternion from rigid-transformation matrix. The matrix must be orthonormal.

     V_CALL quaternion(float4x4 m)
    {
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
    }
    
    VM_INLINE float V_CALL x() const { return _mm_cvtss_f32(m); }
    VM_INLINE float V_CALL y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    VM_INLINE float V_CALL z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
    VM_INLINE float V_CALL w() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3))); }

    VM_INLINE float3 V_CALL xyz() const { return SHUFFLE3(*this, 0, 1, 2); }
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
	
	//TODO(Ray):Try not to use these as much as possible.
#if WINDOWS
	VM_INLINE float V_CALL operator[] (size_t i) const { return m.m128_f32[i]; };
	VM_INLINE float& V_CALL operator[] (size_t i) { return m.m128_f32[i]; };
    VM_INLINE void V_CALL store(float *p) const { p[0] = x(); p[1] = y(); p[2] = z(); p[3] = w(); }
#else
#endif
    VM_INLINE quaternion  static V_CALL identity(){return quaternion(0,0,0,1);}
    void V_CALL setX(float x)
    {
        m = _mm_move_ss(m, _mm_set_ss(x));
    }
    void V_CALL setY(float y)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(y));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
        m = _mm_move_ss(t, m);
    }
    void V_CALL setZ(float z)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(z));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
        m = _mm_move_ss(t, m);
    }
    void V_CALL setW(float w)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(w));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(0, 2, 1, 0));
        m = _mm_move_ss(t, m);
    }
	static quaternion V_CALL look_rotation(float3 forward, float3 up);
	
    //VM_INLINE float3 float3i(int x, int y, int z) { return float3((float)x, (float)y, (float)z); }
};

typedef quaternion boolq;

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

//VM_INLINE quaternion abs(quaternion v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }
// Returns a 3-bit code where bit0..bit2 is X..Z
VM_INLINE unsigned V_CALL mask(quaternion v) { return _mm_movemask_ps(v.m) & 7; }
// Once we have a comparison, we can branch based on its results:
VM_INLINE bool V_CALL any(boolq v) { return mask(v) != 0; }
VM_INLINE bool V_CALL all(boolq v) { return mask(v) == 7; }
//TODO(Rays):Quaternion normalize needs more rigor.
VM_INLINE quaternion V_CALL normalize(quaternion v) { return quaternion(v.xyzw() * (1.0f / length(v.xyzw()))); }
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
    float3 dir = normalize(forward);//normalizeSafe(direction);
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
        return quaternion(q);
    }
    if ((m00 >= m11) && (m00 >= m22))
    {
        float num7 = sqrt(((1.0f + m00) - m11) - m22);
        float num4 = 0.5f / num7;
        q.setX(0.5f * num7);
        q.setY((m01 + m10) * num4);
        q.setZ((m02 + m20) * num4);
        q.setW((m12 - m21) * num4);
        return quaternion(q);
    }
    if (m11 > m22)
    {
        float num6 = sqrt(((1.0f + m11) - m00) - m22);
        float num3 = 0.5f / num6;
        q.setX((m10 + m01) * num3);
        q.setY(0.5f * num6);
        q.setZ((m21 + m12) * num3);
        q.setW((m20 - m02) * num3);
        return quaternion(q);
    }
    float num5 = sqrt(((1.0f + m22) - m00) - m11);
    float num2 = 0.5f / num5;
    q.setX((m20 + m02) * num2);
    q.setY((m21 + m12) * num2);
    q.setZ(0.5f * num5);
    q.setW((m01 - m10) * num2);
    return quaternion(q);
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

//////Begin inverse
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
// for row major matrix
// we use __m128 to represent 2x2 matrix as A = | A0  A1 |
//                                              | A2  A3 |
// 2x2 row major Matrix multiply A*B
__forceinline __m128 Mat2Mul(__m128 vec1, __m128 vec2)
{
    return
    _mm_add_ps(_mm_mul_ps(                     vec1, VecSwizzle(vec2, 0,3,0,3)),
               _mm_mul_ps(VecSwizzle(vec1, 1,0,3,2), VecSwizzle(vec2, 2,1,2,1)));
}
// 2x2 row major Matrix adjugate multiply (A#)*B
__forceinline __m128 Mat2AdjMul(__m128 vec1, __m128 vec2)
{
    return
    _mm_sub_ps(_mm_mul_ps(VecSwizzle(vec1, 3,3,0,0), vec2),
               _mm_mul_ps(VecSwizzle(vec1, 1,1,2,2), VecSwizzle(vec2, 2,3,0,1)));
    
}
// 2x2 row major Matrix multiply adjugate A*(B#)
__forceinline __m128 Mat2MulAdj(__m128 vec1, __m128 vec2)
{
    return
    _mm_sub_ps(_mm_mul_ps(                     vec1, VecSwizzle(vec2, 3,0,3,0)),
               _mm_mul_ps(VecSwizzle(vec1, 1,0,3,2), VecSwizzle(vec2, 2,1,2,1)));
}

// Inverse function is the same no matter column major or row major
// this version treats it as row major
float4x4 V_CALL inverse(float4x4 in_matrix)
{
    // use block matrix method
    // A is a matrix, then i(A) or iA means inverse of A, A# (or A_ in code) means adjugate of A, |A| (or detA in code) is determinant, tr(A) is trace
    
    // sub matrices
    __m128 A = VecShuffle_0101(in_matrix.c0.m, in_matrix.c1.m);
    __m128 B = VecShuffle_2323(in_matrix.c0.m, in_matrix.c1.m);
    __m128 C = VecShuffle_0101(in_matrix.c2.m, in_matrix.c3.m);
    __m128 D = VecShuffle_2323(in_matrix.c2.m, in_matrix.c3.m);
    
    __m128 detA = _mm_set1_ps(in_matrix.c0.x() * in_matrix.c1.y() - in_matrix.c0.y() * in_matrix.c1.x());
    __m128 detB = _mm_set1_ps(in_matrix.c0.z() * in_matrix.c1.w() - in_matrix.c0.w() * in_matrix.c1.z());
    __m128 detC = _mm_set1_ps(in_matrix.c2.x() * in_matrix.c3.y() - in_matrix.c2.y() * in_matrix.c3.x());
    __m128 detD = _mm_set1_ps(in_matrix.c2.z() * in_matrix.c3.w() - in_matrix.c2.w() * in_matrix.c3.z());
    
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
    float4x4 r = float4x4(float4(VecShuffle(X_, Y_, 3,1,3,1)),
                          float4(VecShuffle(X_, Y_, 2,0,2,0)),
                          float4(VecShuffle(Z_, W_, 3,1,3,1)),
                          float4(VecShuffle(Z_, W_, 2,0,2,0)));
    return r;
}
//END INVERSE

//NOTE(Ray):Leaving non working inverse here for now.
float4x4 V_CALL inverse_old_not_working(float4x4 m)
{
    float4 c0 = m.c0;
    float4 c1 = m.c1;
    float4 c2 = m.c2;
    float4 c3 = m.c3;

    float4 r0y_r1y_r0x_r1x = movelh(c1, c0);
    float4 r0z_r1z_r0w_r1w = movelh(c2, c3);
    float4 r2y_r3y_r2x_r3x = movehl(c0, c1);
    float4 r2z_r3z_r2w_r3w = movehl(c3, c2);

    float4 r1y_r2y_r1x_r2x = float4(c1.yz(),c0.yz());//shuffle(c1, c0, ShuffleComponent.LeftY, ShuffleComponent.LeftZ, ShuffleComponent.RightY, ShuffleComponent.RightZ);
    float4 r1z_r2z_r1w_r2w = float4(c2.yz(),c3.yz());//shuffle(c2, c3, ShuffleComponent.LeftY, ShuffleComponent.LeftZ, ShuffleComponent.RightY, ShuffleComponent.RightZ);
    float4 r3y_r0y_r3x_r0x = float4(c1.wx(),c0.wx());//shuffle(c1, c0, ShuffleComponent.LeftW, ShuffleComponent.LeftX, ShuffleComponent.RightW, ShuffleComponent.RightX);
    float4 r3z_r0z_r3w_r0w = float4(c2.wx(),c3.wx());//shuffle(c2, c3, ShuffleComponent.LeftW, ShuffleComponent.LeftX, ShuffleComponent.RightW, ShuffleComponent.RightX);

    float4 r0_wzyx = float4(r0z_r1z_r0w_r1w.zx(),r0y_r1y_r0x_r1x.xz());//shuffle(r0z_r1z_r0w_r1w, r0y_r1y_r0x_r1x, ShuffleComponent.LeftZ, ShuffleComponent.LeftX, ShuffleComponent.RightX, ShuffleComponent.RightZ);
    float4 r1_wzyx = float4(r0z_r1z_r0w_r1w.wy(),r0y_r1y_r0x_r1x.yw());//shuffle(r0z_r1z_r0w_r1w, r0y_r1y_r0x_r1x, ShuffleComponent.LeftW, ShuffleComponent.LeftY, ShuffleComponent.RightY, ShuffleComponent.RightW);
    float4 r2_wzyx = float4(r2z_r3z_r2w_r3w.zw(),r2y_r3y_r2x_r3x.xz());//shuffle(r2z_r3z_r2w_r3w, r2y_r3y_r2x_r3x, ShuffleComponent.LeftZ, ShuffleComponent.LeftX, ShuffleComponent.RightX, ShuffleComponent.RightZ);
    float4 r3_wzyx = float4(r2z_r3z_r2w_r3w.wy(),r2y_r3y_r2x_r3x.yw());//shuffle(r2z_r3z_r2w_r3w, r2y_r3y_r2x_r3x, ShuffleComponent.LeftW, ShuffleComponent.LeftY, ShuffleComponent.RightY, ShuffleComponent.RightW);
    float4 r0_xyzw = float4(r0y_r1y_r0x_r1x.zx(),r0z_r1z_r0w_r1w.xz());//shuffle(r0y_r1y_r0x_r1x, r0z_r1z_r0w_r1w, ShuffleComponent.LeftZ, ShuffleComponent.LeftX, ShuffleComponent.RightX, ShuffleComponent.RightZ);

    // Calculate remaining inner term pairs. inner terms have zw=-xy, so we only have to calculate xy and can pack two pairs per vector.
    float4 inner12_23 = r1y_r2y_r1x_r2x * r2z_r3z_r2w_r3w - r1z_r2z_r1w_r2w * r2y_r3y_r2x_r3x;
    float4 inner02_13 = r0y_r1y_r0x_r1x * r2z_r3z_r2w_r3w - r0z_r1z_r0w_r1w * r2y_r3y_r2x_r3x;
    float4 inner30_01 = r3z_r0z_r3w_r0w * r0y_r1y_r0x_r1x - r3y_r0y_r3x_r0x * r0z_r1z_r0w_r1w;

    // Expand inner terms back to 4 components. zw signs still need to be flipped
    float4 inner12 = float4(inner12_23.xz(),inner12_23.zx());//shuffle(inner12_23, inner12_23, ShuffleComponent.LeftX, ShuffleComponent.LeftZ, ShuffleComponent.RightZ, ShuffleComponent.RightX);
    float4 inner23 = float4(inner12_23.yw(),inner12_23.wy());//shuffle(inner12_23, inner12_23, ShuffleComponent.LeftY, ShuffleComponent.LeftW, ShuffleComponent.RightW, ShuffleComponent.RightY);

    float4 inner02 = float4(inner02_13.xz(),inner02_13.zx());//shuffle(inner02_13, inner02_13, ShuffleComponent.LeftX, ShuffleComponent.LeftZ, ShuffleComponent.RightZ, ShuffleComponent.RightX);
    float4 inner13 = float4(inner02_13.yw(),inner02_13.wy());// shuffle(inner02_13, inner02_13, ShuffleComponent.LeftY, ShuffleComponent.LeftW, ShuffleComponent.RightW, ShuffleComponent.RightY);

    // Calculate minors
    float4 minors0 = r3_wzyx * inner12 - r2_wzyx * inner13 + r1_wzyx * inner23;

    float4 denom = r0_xyzw * minors0;

    // Horizontal sum of denominator. Free sign flip of z and w compensates for missing flip in inner terms.
    denom = denom + float4(denom.yx(),denom.wz());//shuffle(denom, denom, ShuffleComponent.LeftY, ShuffleComponent.LeftX, ShuffleComponent.RightW, ShuffleComponent.RightZ);   // x+y        x+y            z+w            z+w
    denom = denom - float4(denom.zz(),denom.xx());//shuffle(denom, denom, ShuffleComponent.LeftZ, ShuffleComponent.LeftZ, ShuffleComponent.RightX, ShuffleComponent.RightX);   // x+y-z-w  x+y-z-w        z+w-x-y        z+w-x-y
    float4 rcp_denom_ppnn = safe_ratio4_one(float4(1.0f),denom);
	float4x4 res;
    res.c0 = minors0 * rcp_denom_ppnn;

    float4 inner30 = float4(inner30_01.xz(),inner30_01.zx());//shuffle(inner30_01, inner30_01, ShuffleComponent.LeftX, ShuffleComponent.LeftZ, ShuffleComponent.RightZ, ShuffleComponent.RightX);
    float4 inner01 = float4(inner30_01.yw(),inner30_01.wy());//shuffle(inner30_01, inner30_01, ShuffleComponent.LeftY, ShuffleComponent.LeftW, ShuffleComponent.RightW, ShuffleComponent.RightY);

    float4 minors1 = r2_wzyx * inner30 - r0_wzyx * inner23 - r3_wzyx * inner02;
    res.c1 = minors1 * rcp_denom_ppnn;

    float4 minors2 = r0_wzyx * inner13 - r1_wzyx * inner30 - r3_wzyx * inner01;
    res.c2 = minors2 * rcp_denom_ppnn;

    float4 minors3 = r1_wzyx * inner02 - r0_wzyx * inner12 + r2_wzyx * inner01;
    res.c3 = minors3 * rcp_denom_ppnn;

    return res;
}

float4x4 V_CALL init_pers_proj_matrix(float2 buffer_dim, float fov_y = 68.0f, float2 far_near = float2(0.5f, 1000.0f))
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

float4x4 V_CALL init_ortho_proj_matrix(float2 size, float near_clip_plane = 0.1f, float far_clip_plane = 1000.0f)
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
	return (mul(q, float3(0, 0, 1)));
}

 float3 V_CALL up(quaternion q)
{
	return (mul(q, float3(0, 1, 0)));
}

 float3 V_CALL right(quaternion q)
{
	return (mul(q, float3(1, 0, 0)));
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

