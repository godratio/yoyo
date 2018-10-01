#pragma once
#include "yoyoyo_math.h"
#include <list>

float4x4 float4x4Serialize(float4x4 m)
{
	return float4x4(float4(m.c0.m), float4(m.c1.m), float4(m.c2.m), float4(m.c3.m));
}

//NOTE(Ray):If these become a perf bottle neck look to seperate these out.
struct ObjectTransform
{
    float3 p;
    quaternion r;
    float3 s;
    float4x4 m;
    float3 forward;
    float3 up;
    float3 right;
};


ObjectTransform ObjectTransformSerialize(ObjectTransform* ot)
{
	ObjectTransform new_ot = {};
	new_ot.p = float3(ot->p.m);
	new_ot.r = quaternion(ot->r.m);
	new_ot.s = float3(ot->s.m);
	new_ot.m = float4x4Serialize(ot->m);// float4x4(float4(ot->m.c0.m), float4(ot->m.c1.m), float4(ot->m.c2.m), float4(ot->m.c3.m));
	new_ot.forward = float3(ot->forward.m);
	new_ot.right = float3(ot->right.m);
	new_ot.up = float3(ot->up.m);
	return new_ot;
}

//NOTE(Ray):These are convienent methods for those using yoyoyo data formats for abstractions
float4x4 V_CALL YoyoSetTransformMatrix(ObjectTransform* ot)
{
	ot->m = set_matrix(ot->p,ot->r,ot->s);
	return ot->m;
}

float3 V_CALL YoyoWorldPToLocalP(ObjectTransform* ot,float3 a)
{
	return world_local_p(YoyoSetTransformMatrix(ot),a);
}

float3 V_CALL YoyoLocalToWorldP(ObjectTransform* ot,float3 a)
{
	return local_world_p(YoyoSetTransformMatrix(ot),a);
}

void V_CALL YoyoUpdateLocalaxis(ObjectTransform* ot)
{
	ot->up = up(ot->r);
	ot->right = right(ot->r);
	ot->forward = forward(ot->r);
}

void V_CALL YoyoUpdateMatrix(ObjectTransform* ot)
{
	ot->m = YoyoSetTransformMatrix(ot);
}

void V_CALL YoyoUpdateObjectTransform(ObjectTransform* ot)
{
	YoyoUpdateLocalaxis(ot);
	YoyoUpdateMatrix(ot);
}

 static void InitObjectTranform(ObjectTransform* ot)
 {
     ot->r = quaternion::look_rotation(float3(0,1,0),float3(0,0,1));
	 ot->s = float3(1);
	 ot->p = float3(0);
     YoyoUpdateObjectTransform(ot);
 }

float4x4 V_CALL YoyoSetCameraView(ObjectTransform* ot)
{
    YoyoUpdateObjectTransform(ot);
    return set_camera_view(ot->p,ot->forward,ot->up);
}

float3 V_CALL YoyoTransformDir(float4x4 a, float3 b)
{
	return (mul(float4(b, 0) , transpose(a))).xyz();
}
float3 V_CALL YoyoTransformP(float4x4 a, float3 b)
{
	return (mul(float4(b, 1), transpose(a))).xyz();
}

//Physics
bool V_CALL YoyoIntersectSegmentTriangle(float3 p, float3 q, float3 a, float3 b, float3 c, float3 np,
	float* u, float* v, float* w, float* t, float3* hit_point, float3* calc_n)
{
	float3 ab = b - a;
	float3 ac = c - a;
	float3 qp = p - q;

	// Compute triangle normal. Can be precalculated or cached if
	// intersecting multiple segments against the same triangle
	float3 n = cross(ac, ab);
	*calc_n = normalize(n);
	//    n = AbsoluteValue(n);
	//    v3 n = nt;
	// Compute denominator d. If d <= 0, segment is parallel to or points
	// away from triangle, so exit early
	float d = dot(qp, n);
	if (d <= 0.0f) return 0;

	// Compute intersection t value of pq with plane of triangle. A ray
	// intersects if 0 <= t. Segment intersects if 0 <= t <= 1. Delay
	// dividing by d until intersection has been found to pierce triangle
	float3 ap = p - a;
	*t = dot(ap, n);
	if (*t < 0.0f) return 0;
	//NOTE(Ray):For segment; exclude this code line for a ray test
	if (*t > d) return 0;

	// Compute barycentric coordinate components and test if within bounds
	float3 e = cross(ap, qp);
	*v = dot(ac, e);
	if (*v < 0.0f || *v > d) return 0;
	*w = -dot(ab, e);
	if (*w < 0.0f || *v + *w > d) return 0;

	// Segment/ray intersects triangle. Perform delayed division and
	// compute the last barycentric coordinate component
	float ood = 1.0f / d;
	*t *= ood;
	*v *= ood;
	*w *= ood;
	*u = 1.0f - *v - *w;

	*hit_point = float3((*u * a.x() + *v * b.x() + *w * c.x()),
		                (*u * a.y() + *v * b.y() + *w * c.y()),
		                (*u * a.z() + *v * b.z() + *w * c.z()));
	return 1;
}

struct YoyoRay
{
	float3 Origin;
	float3 Dir;
};

static YoyoRay YoyoRaycastFromScreen(float4x4 projection_matrix,float4x4 camera_matrix, float2 buffer_dim, float2 mouse_p)
{
	float3 DP = screen_to_world_point(projection_matrix, camera_matrix, buffer_dim, mouse_p, 0);
	float3 DPEnd = screen_to_world_point(projection_matrix, camera_matrix, buffer_dim, mouse_p, 1.0f);
	float3 RayDir = normalize(DPEnd - DP);
	YoyoRay Result;
	Result.Origin = DP;
	Result.Dir = RayDir;
	return Result;
}

#ifdef YOYO_USE_PHYSX_EXT

//PHsyx extensions
 physx::PxVec3 float3::toPhysx()
{
#if WINDOWS
	return physx::PxVec3(m.m128_f32[0], m.m128_f32[1], m.m128_f32[2]);
#else
#endif
}
 physx::PxVec4 float4::toPhysx()
 {
#if WINDOWS
	 return physx::PxVec4(m.m128_f32[0], m.m128_f32[1], m.m128_f32[2],m.m128_f32[3]);
#else
#endif
 }

 physx::PxQuat quaternion::toPhysx()
 {
#if WINDOWS
	 return physx::PxQuat(m.m128_f32[0], m.m128_f32[1], m.m128_f32[2],m.m128_f32[3]);
#else
#endif
 }

#endif

/*
float3 YoyoScreenToWorldPoint(RenderCommandList* list, float2 buffer_dim, float2 screen_xy, float z_depth)
{
	return screen_to_world_point(list->projection_matrix,list->cam_matrix,buffer_dim,screen_xy,z_depth);
}

float3[] GetScreenRect(RenderCommandList* list,float2 buffer_dim)
{
	float ZD = 0.0f;

	float3 WPLL = ScreenToWorldPoint(list,
		buffer_dim,Float2(0, 0), ZD);

	float3 WPLR = ScreenToWorldPoint(list,buffer_dim,
		Float2(buffer_dim.x, 0), ZD);

	float3 WPUL = ScreenToWorldPoint(list,buffer_dim,
		Float2(0, buffer_dim.y), ZD);

	float3 WPUR = ScreenToWorldPoint(list,buffer_dim,
		Float2(buffer_dim.x, buffer_dim.y), ZD);

	rectangle3 Result = { WPLL,WPLR,WPUL,WPUR };
	return Result;
}


*/



