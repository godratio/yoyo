#pragma once

#include "yoyoyo_math.h"
#include "yoyoyo_ascii_vector.h"
#include "yoyoyo_math_ext.h"
#include "yoyoyo.h"

struct ObjectTransformBuffer
{
	YoyoVector object_transforms;
};

namespace ObjectTransformCode
{
    static void InitObjectTransformBuffer(ObjectTransformBuffer* buffer,int start_size)
    {
        if(buffer == nullptr)
        {
			Assert(false);
			return;
        }
		ObjectTransformBuffer buff;
		*buffer = buff;
    	buffer->object_transforms = YoyoInitVector(start_size, ObjectTransform, false);
    }

    static void AddObjectTransform(ObjectTransformBuffer* buffer,float3 p,float3 s,quaternion r)
    {
        ObjectTransform new_ot;
        new_ot.p = p;
        new_ot.s = s;
        new_ot.r = r;
		YoyoUpdateObjectTransform(&new_ot);
        YoyoPushBack(&buffer->object_transforms,new_ot);
    }

	static void UpdateObjectTransforms(ObjectTransformBuffer* buffer)
    {
        ObjectTransform* ot;
        while(ot = YoyoIterateVector(&buffer->object_transforms,ObjectTransform))
        {
            YoyoUpdateObjectTransform(ot);
        }
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
	YoyoVector scene_objects;
	SceneObjectType type;
};

namespace SceneObjectCode
{
    static void InitSceneObject(SceneObjectBuffer* buffer,int size)
    {
		//NOTE(RAY):Caller is responsible for nullness
		if (buffer == nullptr) 
		{
			Assert(false);
			return;
		}
    	SceneObjectBuffer new_buff;
    	*buffer = new_buff;
    	buffer->scene_objects = YoyoInitVector(size,SceneObject,false);
    }

    static SceneObject* AddSceneObject(SceneObjectBuffer* buffer,ObjectTransform* ot)
    {
        SceneObject so;
		so.children = {};
        so.ot = *ot;
		
        YoyoPushBack(&buffer->scene_objects,so);        
		return YoyoPeekVectorElement(SceneObject,&buffer->scene_objects);
    }

	static void AddChildToSceneObject(SceneObjectBuffer* buffer,SceneObject* so,ObjectTransform* new_child)
    {
		if(so->children.object_transforms.count == 0)//Assuming uninited vector be careful here for leaks!!
		{
			so->children.object_transforms = YoyoInitVector(1, ObjectTransform, false);
		}

		YoyoPushBack(&so->children.object_transforms, *new_child);
    }

    static void UpdateSceneObjects(SceneObjectBuffer* buffer,float3* position_sum,quaternion* rotation_product)
    {
        SceneObject* so;
        while(so = YoyoIterateVector(&buffer->scene_objects,SceneObject))
        {
			YoyoUpdateObjectTransform(&so->ot);
            ObjectTransform parent_ot = so->ot;
            ObjectTransform* child_ot;
            while(child_ot = YoyoIterateVector(&so->children.object_transforms,ObjectTransform))
            {
                child_ot->p = *position_sum += rotate(*rotation_product, child_ot->local_p);
                child_ot->r = *rotation_product = *rotation_product * child_ot->local_r;
                child_ot->s = parent_ot.s * child_ot->local_s;
                YoyoUpdateObjectTransform(child_ot);
				//NOTE(Ray):This is depth first recursion once we get to a leaf with no children we should exit out.
				//This is bad for several reasons 
				//reason1: if we get to have 10000 game objects could blow the stack
				//reason2: do we need another reason? 
				//Why are we doing this? because its the easiest way at the moment will rework this later if need be.
				//Another option would be to have a data structure that has a pointer to the parent similar to teh skeletal animation system.
				//Lets go this route see how it goes.
				UpdateSceneObjects(buffer, position_sum, rotation_product);
            }
			YoyoResetVectorIterator(&so->children.object_transforms);
        }
		YoyoResetVectorIterator(&buffer->scene_objects);
    }
}

enum SceneState
{
    scene_state_default,//default state nothing happened
    scene_state_loading,
    scene_state_loaded,
    scene_state_unloading,
    scene_state_unloaded,
    scene_state_serializing,
    scene_state_serialized
};

struct SceneStatus
{
    uint32_t state_flags;
    uint32_t object_count;
};

struct Scene
{
    uint32_t buffer_count;
    SceneObjectBuffer scene_object_buffer;
    uint32_t state_flags;    
};

struct SceneBuffer
{
    YoyoVector buffer;
};

namespace SceneCode
{
    static void InitScene(SceneBuffer* buffer,int size)
    {
        if(buffer == nullptr)
		{
			Assert(false);
			return;
        }
		SceneBuffer buff;
		*buffer = buff;
    	buffer->buffer = YoyoInitVector(size, Scene,false);
    }

	//NOTE(Ray):All empty scenes in fact all scenes must have one scene object that is the root object
    static Scene* CreateEmptyScene(SceneBuffer* buffer)
    {
        Scene scene;
        scene.buffer_count = 0;
	    SceneObjectCode::InitSceneObject(&scene.scene_object_buffer,1);
        scene.state_flags = scene_state_default;
        YoyoPushBack(&buffer->buffer,scene);
        return YoyoPeekVectorElement(Scene,&buffer->buffer);
    }

    //NOTE(Ray):For updating all scenes?
    static void UpdateSceneBuffer(SceneBuffer* buffer)
    {
        Scene* scene;
        float3 sum = float3(0);
        quaternion product = quaternion::identity();
        while(scene = YoyoIterateVector(&buffer->buffer,Scene,false))
        {
			SceneObjectCode::UpdateSceneObjects(&scene->scene_object_buffer, &sum, &product);
        }
		YoyoResetVectorIterator(&buffer->buffer);
    }
}


