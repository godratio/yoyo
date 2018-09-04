#pragma once
#include "yoyoyo_math.h"

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

//NOTE(Ray):These are convienent methods for those using yoyoyo data formats for abstractions
float4x4 YoyoSetTransformMatrix(ObjectTransform* ot)
{
	ot->m = set_matrix(ot->p,ot->r,ot->s);
	return ot->m;
}

float3 YoyoWorldPToLocalP(ObjectTransform* ot,float3 a)
{
	return world_local_p(YoyoSetTransformMatrix(ot),a);
}

float3 YoyoLocalToWorldP(ObjectTransform* ot,float3 a)
{
	return local_world_p(YoyoSetTransformMatrix(ot),a);
}

void YoyoUpdateLocalaxis(ObjectTransform* ot)
{
	ot->up = up(ot->r);
	ot->right = right(ot->r);
	ot->forward = forward(ot->r);
}

void YoyoUpdateMatrix(ObjectTransform* ot)
{
	ot->m = YoyoSetTransformMatrix(ot);
}

void YoyoUpdateObjectTransform(ObjectTransform* ot)
{
	YoyoUpdateLocalaxis(ot);
	YoyoUpdateMatrix(ot);
}

float4x4 YoyoSetCameraView(ObjectTransform* ot)
{
    YoyoUpdateObjectTransform(ot);
    return set_camera_view(ot->p,ot->forward,ot->up);
}

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

internal ray RaycastFromScreen(RenderCommandList* list,float2 buffer_dim, float2 mouse_p)
{
	float3 DP = ScreenToWorldPoint(list,buffer_dim, Float2(mouse_p.x, mouse_p.y), 0);
	float3 DPEnd = ScreenToWorldPoint(list,buffer_dim, Float2(mouse_p.x, mouse_p.y), 1.0f);
	float3 RayDir = Normalize(DPEnd - DP);
	ray Result;
	Result.Origin = DP;
	Result.Dir = RayDir;
	return Result;
}
*/



