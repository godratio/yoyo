//TODO(Ray):Build tests almost all of this is untested
#pragma once

#include <stdint.h>
#include <math.h>
#include <xmmintrin.h>

//TODO(Ray):look up proper define for compilers 
#if WINDOWS
#define VM_INLINE   __forceinline
#else
#define VM_INLINE __attribute__((always_inline))
#endif

#define M_PI        3.14159265358979323846f
#define M_TAO       6.28318530717f

#define DEG2RAD   M_PI/180
#define RAD2DEG   180 / M_PI

#define DEGREE_TO_RADIAN(a) a * DEG2RAD
#define RADIAN_TO_ANGLE(a)  a * RAD2DEG

//#define DEG2RAD(_a) ((_a)*M_PI/180.0f)
//#define RAD2DEG(_a) ((_a)*180.0f/M_PI)
#define U16MAX 65535
#define INT_MIN     (-2147483647 - 1)
#define INT_MAX     2147483647
#define U32MAX      ((u32)-1)
#define FLT_MAX     3.402823466e+38F
#define FLT_MIN     -FLT_MAX

// Shuffle helpers.
// Examples: SHUFFLE3(v, 0,1,2) leaves the vector unchanged.
//           SHUFFLE3(v, 0,0,0) splats the X coord out.
#define SHUFFLE2(V, X,Y) float2(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(Y,Y,Y,X)))
#define SHUFFLE3(V, X,Y,Z) float3(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(Z,Z,Y,X)))
#define SHUFFLE4(V, X,Y,Z,W) float4(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(W,Z,Y,X)))

struct float2
{
    __m128 m;
    // Constructors.
    VM_INLINE float2() {}
    VM_INLINE explicit float2(const float *p) { m = _mm_set_ps(p[1], p[1], p[1], p[0]); }
    VM_INLINE explicit float2(float x, float y) { m = _mm_set_ps(y, y, y, x); }
    VM_INLINE explicit float2(__m128 v) { m = v; }
    VM_INLINE float x() const { return _mm_cvtss_f32(m); }
    VM_INLINE float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    VM_INLINE float2 yx() const { return SHUFFLE2(*this, 1, 0); }
    VM_INLINE void store(float *p) const { p[0] = x(); p[1] = y(); }
    void setX(float x)
    {
        m = _mm_move_ss(m, _mm_set_ss(x));
    }
    void setY(float y)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(y));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
        m = _mm_move_ss(t, m);
    }
    //VM_INLINE float operator[] (size_t i) const { return m.m128_f32[i]; };
    //VM_INLINE float& operator[] (size_t i) { return m.m128_f32[i]; };
    //VM_INLINE float3 float3i(int x, int y, int z) { return float3((float)x, (float)y, (float)z); }
};

struct float3
{
    __m128 m;
    // Constructors.
    VM_INLINE float3() {}
    VM_INLINE explicit float3(const float *p) { m = _mm_set_ps(p[2], p[2], p[1], p[0]); }
    VM_INLINE explicit float3(float x, float y, float z) { m = _mm_set_ps(z, z, y, x); }
    VM_INLINE explicit float3(__m128 v) { m = v; }
    VM_INLINE float x() const { return _mm_cvtss_f32(m); }
    VM_INLINE float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    VM_INLINE float z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
    VM_INLINE float3 yzx() const { return SHUFFLE3(*this, 1, 2, 0); }
    VM_INLINE float3 zxy() const { return SHUFFLE3(*this, 2, 0, 1); }
    VM_INLINE void store(float *p) const { p[0] = x(); p[1] = y(); p[2] = z(); }
    void setX(float x)
    {
        m = _mm_move_ss(m, _mm_set_ss(x));
    }
    void setY(float y)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(y));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
        m = _mm_move_ss(t, m);
    }
    void setZ(float z)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(z));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
        m = _mm_move_ss(t, m);
    }
    //VM_INLINE float operator[] (size_t i) const { return m.m128_f32[i]; };
    //VM_INLINE float& operator[] (size_t i) { return m.m128_f32[i]; };
    //VM_INLINE float3 float3i(int x, int y, int z) { return float3((float)x, (float)y, (float)z); }
};

struct float4
{
    __m128 m;
    // Constructors.
    VM_INLINE float4() {}
    VM_INLINE explicit float4(const float *p) { m = _mm_set_ps(p[3], p[2], p[1], p[0]); }
    VM_INLINE explicit float4(float x, float y, float z,float w) { m = _mm_set_ps(w, z, y, x); }
    VM_INLINE explicit float4(float x) { m = _mm_set_ps(x, x, x, x); }
    VM_INLINE explicit float4(__m128 v) { m = v; }
    VM_INLINE explicit float4(float2 a,float2 b){m = _mm_set_ps(a.x(),a.y(),b.x(),b.y());}

    VM_INLINE float x() const { return _mm_cvtss_f32(m); }
    VM_INLINE float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    VM_INLINE float z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
    VM_INLINE float w() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3))); }

    VM_INLINE float3 xyz() const { return SHUFFLE3(*this, 2, 1, 0); }
    VM_INLINE float4 yzxw() const { return SHUFFLE4(*this, 1, 2, 0, 3); }
    VM_INLINE float4 zxyw() const { return SHUFFLE4(*this, 2, 0, 1, 3); }
    VM_INLINE float2 xy() const { return SHUFFLE2(*this, 1, 0); }
    VM_INLINE float2 zw() const { return SHUFFLE2(*this, 3, 2); }

    VM_INLINE float2 xx() const { return SHUFFLE2(*this, 0, 0); }
    VM_INLINE float2 yz() const { return SHUFFLE2(*this, 2, 1); }
    VM_INLINE float2 wx() const { return SHUFFLE2(*this, 3, 0); }
    VM_INLINE float2 xz() const { return SHUFFLE2(*this, 0, 2); }
    VM_INLINE float2 yx() const { return SHUFFLE2(*this, 1, 0); }
    VM_INLINE float2 yw() const { return SHUFFLE2(*this, 1, 3); }
    VM_INLINE float2 zx() const { return SHUFFLE2(*this, 2, 0); }
    VM_INLINE float2 zz() const { return SHUFFLE2(*this, 2, 2); }
    VM_INLINE float2 wz() const { return SHUFFLE2(*this, 3, 2); }
    VM_INLINE float2 wy() const { return SHUFFLE2(*this, 3, 1); }
    
    VM_INLINE void store(float *p) const { p[0] = x(); p[1] = y(); p[2] = z(); p[3] = w(); }
    void setX(float x)
    {
        m = _mm_move_ss(m, _mm_set_ss(x));
    }
    void setY(float y)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(y));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
        m = _mm_move_ss(t, m);
    }
    void setZ(float z)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(z));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
        m = _mm_move_ss(t, m);
    }
    void setW(float w)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(w));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(0, 2, 1, 0));
        m = _mm_move_ss(t, m);
    }
    //VM_INLINE float operator[] (size_t i) const { return m.m128_f32[i]; };
    //VM_INLINE float& operator[] (size_t i) { return m.m128_f32[i]; };
    //VM_INLINE float3 float3i(int x, int y, int z) { return float3((float)x, (float)y, (float)z); }
};

struct float4x4
{
    float4 c0;
    float4 c1;
    float4 c2;
    float4 c3;
    VM_INLINE float4x4() {}
    VM_INLINE explicit float4x4(float4 c0, float4 c1, float4 c2,float4 c3) { this->c0 = c0;this->c1 = c1;this->c2 = c2;this->c3 = c3; }
    VM_INLINE explicit float4x4(float m00, float m01, float m02, float m03,
                                float m10, float m11, float m12, float m13,
                                float m20, float m21, float m22, float m23,
                                float m30, float m31, float m32, float m33)
        { 
            this->c0 = float4(m00, m10, m20, m30);
            this->c1 = float4(m01, m11, m21, m31);
            this->c2 = float4(m02, m12, m22, m32);
            this->c3 = float4(m03, m13, m23, m33);
        }
    

};

/*
YoyoAString YoyoFloat4x4ToString()
        {
            return string.Format("float4x4({0}f, {1}f, {2}f, {3}f,  {4}f, {5}f, {6}f, {7}f,  {8}f, {9}f, {10}f, {11}f,  {12}f, {13}f, {14}f, {15}f)", c0.x, c1.x, c2.x, c3.x, c0.y, c1.y, c2.y, c3.y, c0.z, c1.z, c2.z, c3.z, c0.w, c1.w, c2.w, c3.w);
        }
*/

/*
/// <summary>float4x4 identity transform.</summary>
public static readonly float4x4 identity = new float4x4(1.0f, 0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f, 1.0f);

/// <summary>float4x4 zero value.</summary>
public static readonly float4x4 zero = new float4x4(0.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 0.0f);
*/


typedef float2 bool2;
VM_INLINE float2 operator+ (float2 a, float2 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
VM_INLINE float2 operator- (float2 a, float2 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
VM_INLINE float2 operator* (float2 a, float2 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
VM_INLINE float2 operator/ (float2 a, float2 b) { a.m = _mm_div_ps(a.m, b.m); return a; }
VM_INLINE float2 operator* (float2 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float2 operator/ (float2 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float2 operator* (float a, float2 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }
VM_INLINE float2 operator/ (float a, float2 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }
VM_INLINE float2& operator+= (float2 &a, float2 b) { a = a + b; return a; }
VM_INLINE float2& operator-= (float2 &a, float2 b) { a = a - b; return a; }
VM_INLINE float2& operator*= (float2 &a, float2 b) { a = a * b; return a; }
VM_INLINE float2& operator/= (float2 &a, float2 b) { a = a / b; return a; }
VM_INLINE float2& operator*= (float2 &a, float b) { a = a * b; return a; }
VM_INLINE float2& operator/= (float2 &a, float b) { a = a / b; return a; }
VM_INLINE bool2 operator==(float2 a, float2 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
VM_INLINE bool2 operator!=(float2 a, float2 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
VM_INLINE bool2 operator< (float2 a, float2 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
VM_INLINE bool2 operator> (float2 a, float2 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
VM_INLINE bool2 operator<=(float2 a, float2 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
VM_INLINE bool2 operator>=(float2 a, float2 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
VM_INLINE float2 min(float2 a, float2 b) { a.m = _mm_min_ps(a.m, b.m); return a; }
VM_INLINE float2 max(float2 a, float2 b) { a.m = _mm_max_ps(a.m, b.m); return a; }
VM_INLINE float2 operator- (float2 a) { return float2(_mm_setzero_ps()) - a; }
//VM_INLINE float2 abs(float2 v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }
// Returns a 3-bit code where bit0..bit2 is X..Z
VM_INLINE unsigned mask(float2 v) { return _mm_movemask_ps(v.m) & 7; }
// Once we have a comparison, we can branch based on its results:
VM_INLINE bool any(bool2 v) { return mask(v) != 0; }
VM_INLINE bool all(bool2 v) { return mask(v) == 7; }


typedef float3 bool3;
VM_INLINE float3 operator+ (float3 a, float3 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
VM_INLINE float3 operator- (float3 a, float3 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
VM_INLINE float3 operator* (float3 a, float3 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
VM_INLINE float3 operator/ (float3 a, float3 b) { a.m = _mm_div_ps(a.m, b.m); return a; }
VM_INLINE float3 operator* (float3 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float3 operator/ (float3 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float3 operator* (float a, float3 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }
VM_INLINE float3 operator/ (float a, float3 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }
VM_INLINE float3& operator+= (float3 &a, float3 b) { a = a + b; return a; }
VM_INLINE float3& operator-= (float3 &a, float3 b) { a = a - b; return a; }
VM_INLINE float3& operator*= (float3 &a, float3 b) { a = a * b; return a; }
VM_INLINE float3& operator/= (float3 &a, float3 b) { a = a / b; return a; }
VM_INLINE float3& operator*= (float3 &a, float b) { a = a * b; return a; }
VM_INLINE float3& operator/= (float3 &a, float b) { a = a / b; return a; }
VM_INLINE bool3 operator==(float3 a, float3 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
VM_INLINE bool3 operator!=(float3 a, float3 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
VM_INLINE bool3 operator< (float3 a, float3 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
VM_INLINE bool3 operator> (float3 a, float3 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
VM_INLINE bool3 operator<=(float3 a, float3 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
VM_INLINE bool3 operator>=(float3 a, float3 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
VM_INLINE float3 min(float3 a, float3 b) { a.m = _mm_min_ps(a.m, b.m); return a; }
VM_INLINE float3 max(float3 a, float3 b) { a.m = _mm_max_ps(a.m, b.m); return a; }
VM_INLINE float3 operator- (float3 a) { return float3(_mm_setzero_ps()) - a; }
//VM_INLINE float3 abs(float3 v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }
// Returns a 3-bit code where bit0..bit2 is X..Z
VM_INLINE unsigned mask(float3 v) { return _mm_movemask_ps(v.m) & 7; }
// Once we have a comparison, we can branch based on its results:
VM_INLINE bool any(bool3 v) { return mask(v) != 0; }
VM_INLINE bool all(bool3 v) { return mask(v) == 7; }

VM_INLINE float hmin(float3 v) {
    v = min(v, SHUFFLE3(v, 1, 0, 2));
    return min(v, SHUFFLE3(v, 2, 0, 1)).x();
}

VM_INLINE float hmax(float3 v) {
    v = max(v, SHUFFLE3(v, 1, 0, 2));
    return max(v, SHUFFLE3(v, 2, 0, 1)).x();
}
VM_INLINE float3 cross(float3 a, float3 b) {
    // x  <-  a.y*b.z - a.z*b.y
    // y  <-  a.z*b.x - a.x*b.z
    // z  <-  a.x*b.y - a.y*b.x
    // We can save a shuffle by grouping it in this wacky order:
    return (a.zxy()*b - a*b.zxy()).zxy();
}

typedef float4 bool4;

VM_INLINE float4 operator+ (float4 a, float4 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
VM_INLINE float4 operator+ (float4 a, float b) { a.m = _mm_add_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float4 operator+ (float  a, float4 b) { b.m = _mm_add_ps( _mm_set1_ps(a),b.m); return b; }

VM_INLINE float4 operator- (float4 a, float4 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
VM_INLINE float4 operator- (float4 a, float b) { a.m = _mm_sub_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float4 operator- (float  a, float4 b) { b.m = _mm_sub_ps( _mm_set1_ps(a),b.m); return b; }

VM_INLINE float4 operator* (float4 a, float4 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
VM_INLINE float4 operator* (float4 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float4 operator* (float a, float4 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }

VM_INLINE float4 operator/ (float4 a, float4 b) { a.m = _mm_div_ps(a.m, b.m); return a; }
VM_INLINE float4 operator/ (float4 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
VM_INLINE float4 operator/ (float a, float4 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }

VM_INLINE float4& operator+= (float4 &a, float4 b) { a = a + b; return a; }
VM_INLINE float4& operator-= (float4 &a, float4 b) { a = a - b; return a; }

VM_INLINE float4& operator*= (float4 &a, float4 b) { a = a * b; return a; }
VM_INLINE float4& operator*= (float4 &a, float b) { a = a * b; return a; }

VM_INLINE float4& operator/= (float4 &a, float4 b) { a = a / b; return a; }
VM_INLINE float4& operator/= (float4 &a, float b) { a = a / b; return a; }

VM_INLINE bool4 operator==(float4 a, float4 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
VM_INLINE bool4 operator!=(float4 a, float4 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
VM_INLINE bool4 operator< (float4 a, float4 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
VM_INLINE bool4 operator> (float4 a, float4 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
VM_INLINE bool4 operator<=(float4 a, float4 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
VM_INLINE bool4 operator>=(float4 a, float4 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
VM_INLINE float4 min(float4 a, float4 b) { a.m = _mm_min_ps(a.m, b.m); return a; }
VM_INLINE float4 max(float4 a, float4 b) { a.m = _mm_max_ps(a.m, b.m); return a; }
VM_INLINE float4 operator- (float4 a) { return float4(_mm_setzero_ps()) - a; }
//VM_INLINE float4 abs(float4 v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }
// Returns a 3-bit code where bit0..bit2 is X..Z
VM_INLINE unsigned mask(float4 v) { return _mm_movemask_ps(v.m) & 7; }
// Once we have a comparison, we can branch based on its results:
VM_INLINE bool any(bool4 v) { return mask(v) != 0; }
VM_INLINE bool all(bool4 v) { return mask(v) == 7; }


//mul
VM_INLINE float4x4 operator * (float4x4 lhs, float4x4 rhs) { return float4x4 (lhs.c0 * rhs.c0, lhs.c1 * rhs.c1, lhs.c2 * rhs.c2, lhs.c3 * rhs.c3); }
VM_INLINE float4x4 operator * (float4x4 lhs, float rhs) { return    float4x4 (lhs.c0 * rhs, lhs.c1 * rhs, lhs.c2 * rhs, lhs.c3 * rhs); }
VM_INLINE float4x4 operator * (float lhs, float4x4 rhs) { return    float4x4 (lhs * rhs.c0, lhs * rhs.c1, lhs * rhs.c2, lhs * rhs.c3); }

// add
VM_INLINE float4x4 operator + (float4x4 lhs, float4x4 rhs) { return float4x4 (lhs.c0 + rhs.c0, lhs.c1 + rhs.c1, lhs.c2 + rhs.c2, lhs.c3 + rhs.c3); }
VM_INLINE float4x4 operator + (float4x4 lhs, float rhs) { return    float4x4 (lhs.c0 + rhs, lhs.c1 + rhs, lhs.c2 + rhs, lhs.c3 + rhs); }
VM_INLINE float4x4 operator + (float lhs, float4x4 rhs) { return    float4x4 (lhs + rhs.c0, lhs + rhs.c1, lhs + rhs.c2, lhs + rhs.c3); }

//sub
VM_INLINE float4x4 operator - (float4x4 lhs, float4x4 rhs) { return float4x4 (lhs.c0 - rhs.c0, lhs.c1 - rhs.c1, lhs.c2 - rhs.c2, lhs.c3 - rhs.c3); }
VM_INLINE float4x4 operator - (float4x4 lhs, float rhs) { return  float4x4 (lhs.c0 - rhs, lhs.c1 - rhs, lhs.c2 - rhs, lhs.c3 - rhs); }
VM_INLINE float4x4 operator - (float lhs, float4x4 rhs) { return  float4x4 (lhs - rhs.c0, lhs - rhs.c1, lhs - rhs.c2, lhs - rhs.c3); }

//div
VM_INLINE float4x4 operator / (float4x4 lhs, float4x4 rhs) { return float4x4 (lhs.c0 / rhs.c0, lhs.c1 / rhs.c1, lhs.c2 / rhs.c2, lhs.c3 / rhs.c3); }
VM_INLINE float4x4 operator / (float4x4 lhs, float rhs) { return float4x4 (lhs.c0 / rhs, lhs.c1 / rhs, lhs.c2 / rhs, lhs.c3 / rhs); }
VM_INLINE float4x4 operator / (float lhs, float4x4 rhs) { return float4x4 (lhs / rhs.c0, lhs / rhs.c1, lhs / rhs.c2, lhs / rhs.c3); }


//mod
//VM_INLINE float4x4 operator % (float4x4 lhs, float4x4 rhs) { return new float4x4 (lhs.c0 % rhs.c0, lhs.c1 % rhs.c1, lhs.c2 % rhs.c2, lhs.c3 % rhs.c3); }
//VM_INLINE float4x4 operator % (float4x4 lhs, float rhs) { return new float4x4 (lhs.c0 % rhs, lhs.c1 % rhs, lhs.c2 % rhs, lhs.c3 % rhs); }
//VM_INLINE float4x4 operator % (float lhs, float4x4 rhs) { return new float4x4 (lhs % rhs.c0, lhs % rhs.c1, lhs % rhs.c2, lhs % rhs.c3); }
//inc
//VM_INLINE float4x4 operator ++ (float4x4 val) { return new float4x4 (++val.c0, ++val.c1, ++val.c2, ++val.c3); }
//dec
//VM_INLINE float4x4 operator -- (float4x4 val) { return new float4x4 (--val.c0, --val.c1, --val.c2, --val.c3); }

VM_INLINE float3 rotate(float4x4 a, float3 b)
{
    return (a.c0 * b.x() + a.c1 * b.y() + a.c2 * b.z()).xyz();
}

VM_INLINE float3 transform(float4x4 a, float3 b)
{
    return (a.c0 * b.x() + a.c1 * b.y() + a.c2 * b.z() + a.c3).xyz();
}

VM_INLINE float4x4 transpose(float4x4 v)
{
    return float4x4(
                    v.c0.x(), v.c0.y(), v.c0.z(), v.c0.w(),
                    v.c1.x(), v.c1.y(), v.c1.z(), v.c1.w(),
                    v.c2.x(), v.c2.y(), v.c2.z(), v.c2.w(),
                    v.c3.x(), v.c3.y(), v.c3.z(), v.c3.w());
}

VM_INLINE float4 movelh(float4 a,float4 b)
{
    return float4(a.x(),a.y(),b.x(),b.y());
}

VM_INLINE float4 movehl(float4 a,float4 b)
{
    return float4(b.z(),b.w(),a.z(),a.w());
}

VM_INLINE float4x4 inverse(float4x4 m)
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

    float4 rcp_denom_ppnn = float4(1.0f) / denom;
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

//hlsl funcs
VM_INLINE float2 clamp(float2 t, float2 a, float2 b) { return min(max(t, a), b); }
VM_INLINE float sum(float2 v) { return v.x() + v.y();}
VM_INLINE float dot(float2 a, float2 b) { return sum(a*b); }
VM_INLINE float length(float2 v) { return sqrtf(dot(v, v)); }
VM_INLINE float lengthSq(float2 v) { return dot(v, v); }
VM_INLINE float2 normalize(float2 v) { return v * (1.0f / length(v)); }
VM_INLINE float2 lerp(float2 a, float2 b, float t) { return a + (b-a)*t; }

VM_INLINE float3 clamp(float3 t, float3 a, float3 b) { return min(max(t, a), b); }
VM_INLINE float sum(float3 v) { return v.x() + v.y() + v.z(); }
VM_INLINE float dot(float3 a, float3 b) { return sum(a*b); }
VM_INLINE float length(float3 v) { return sqrtf(dot(v, v)); }
VM_INLINE float lengthSq(float3 v) { return dot(v, v); }
VM_INLINE float3 normalize(float3 v) { return v * (1.0f / length(v)); }
VM_INLINE float3 lerp(float3 a, float3 b, float t) { return a + (b-a)*t; }

VM_INLINE float4 clamp(float4 t, float4 a, float4 b) { return min(max(t, a), b); }
VM_INLINE float sum(float4 v) { return v.x() + v.y() + v.z(); }
VM_INLINE float dot(float4 a, float4 b) { return sum(a*b); }
VM_INLINE float length(float4 v) { return sqrtf(dot(v, v)); }
VM_INLINE float lengthSq(float4 v) { return dot(v, v); }
VM_INLINE float4 normalize(float4 v) { return v * (1.0f / length(v)); }
VM_INLINE float4 lerp(float4 a, float4 b, float t) { return a + (b-a)*t; }

