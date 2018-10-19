#pragma once

#include "yoyoyo_math.h"
#include "yoyoyo_ascii_vector.h"

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

struct ObjectTransformBuffer
{
	yoyoyo_ascii_vector object_transforms;
};


static void InitObjectTransformBuffer(ObjectTransformBuffer* buffer,int start_size)
{
	if(buffer == nullptr)
	{
		buffer->object_transforms = YoyoInitVector(start_size, sizeof(ObjectTransform), false);
	}
}

struct SceneObject
{
	ObjectTransform ot;
	ObjectTransformBuffer children;
};

enum SceneObjectType
{
	scene_object,
	scene_light
};

struct SceneObjectBuffer
{
	vector SceneObjects;
	SceneObjectType type;
};
