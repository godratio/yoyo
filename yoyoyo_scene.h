#pragma once

#include "yoyoyo_math.h"
#include "yoyoyo_ascii_vector.h"
#include "yoyoyo_math_ext.h"
#include "yoyoyo.h"

struct ObjectTransformBuffer
{
	YoyoVector object_transforms;
    uint64_t next_free_index = 0;
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
        for(int i = 0;i < start_size;++i)
        {
            ObjectTransform* ot = (ObjectTransform*)buffer->object_transforms.base + i;
            ot->is_free = true;
        }
    }

//This is the retained mode api    
    static void AddObjectTransform(ObjectTransformBuffer* buffer,float3 p,float3 s,quaternion r)
    {
        ObjectTransform new_ot;
        new_ot.p = p;
        new_ot.s = s;
        new_ot.r = r;
		YoyoUpdateObjectTransform(&new_ot);
        YoyoPushBack(&buffer->object_transforms,new_ot);
    }

    static void SetNextOT(ObjectTransformBuffer* buffer)
    {
        float iteration = 0;
        //find and set the next non used projectile
        for (int i = buffer->next_free_index; iteration < buffer->object_transforms.total_count; ++iteration)
        {
            int wrapped_i = i % (int)buffer->object_transforms.total_count;
            ObjectTransform* new_ot = (ObjectTransform*)buffer->object_transforms.base + wrapped_i;
            if (new_ot->is_free)
            {
                buffer->next_free_index = wrapped_i;
                break;
            }
            ++i;
        }
    }
    
    //This is the free list or immediate mode api    
    static ObjectTransform* AddObjectTransformIM(ObjectTransformBuffer* buffer,float3 p,float3 s,quaternion r)
    {
        ObjectTransform* new_ot = YoyoGetVectorElement(ObjectTransform,&buffer->object_transforms,buffer->next_free_index);
        if(new_ot)
        {
            new_ot->p = p;
            new_ot->s = s;
            new_ot->r = r;
            new_ot->is_free = false;
            YoyoUpdateObjectTransform(new_ot);
            SetNextOT(buffer);
        }
        return new_ot;
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


enum SceneObjectType
{
	scene_object,
	scene_light
};

struct SceneObjectBuffer
{
	YoyoVector buffer;
	SceneObjectType type;
};

struct SceneObject
{
	ObjectTransform ot;
	SceneObjectBuffer children;
    int handle;
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
    	buffer->buffer = YoyoInitVector(size,SceneObject,false);
		buffer->buffer.allow_resize = false;
    }

    static SceneObject* GetSceneObject(SceneObjectBuffer*so,int handle)
    {
        if(so)
        {
            //TODO(Ray):Bounds chcking
            return YoyoGetVectorElement(SceneObject,&so->buffer,handle);
        }
        return 0;
    }
    //RETURNS HANDLE into scene object 
    static int AddSceneObject(SceneObjectBuffer* so,ObjectTransform* ot)
    {
		SceneObject new_so;
		new_so.children = {};
        new_so.ot = *ot;
        uint32_t handle = YoyoPushBack(&so->buffer,new_so);
        SceneObject* new_so_ptr = GetSceneObject(so,handle);
        new_so_ptr->handle = handle;
        return handle;
    }

    static int AddSceneObject(SceneObjectBuffer* so,float3 p,quaternion r,float3 s)
    {
        ObjectTransform ot;
        ot.p = p;
        ot.r = r;
        ot.s = s;
        return AddSceneObject(so,&ot);
    }


    static void UpdateSceneObject(SceneObject* so,float3 p,quaternion r,float3 s)
    {
//        if(so)
        {
            so->ot.p = p;
            so->ot.r = r;
            so->ot.s = s;                            
        }
    }
    
	//NOTE(Ray):When adding a chid ot p is local position and p is offset from parents ot p.
	static int AddChildToSceneObject(int so_index,SceneObjectBuffer* buffer,ObjectTransform* new_child)
    {
		SceneObject* so = YoyoGetVectorElement(SceneObject, &buffer->buffer, so_index);
		if(so->children.buffer.count == 0)//Assuming uninited vector be careful here for leaks!!
		{
			so->children.buffer = YoyoInitVector(10, SceneObject, false);
		}

		//and the local p is the absolute p relative to the parent p.
		new_child->local_p = new_child->p;
		new_child->local_s = new_child->s;
		new_child->local_r = new_child->r;

    	//New child p in this contex is the local p relative to the parent p reference frame
//		new_child->p  += rotate(so->ot.r, new_child->p);
		//Rotations add
//		new_child->r = so->ot.r * new_child->r;
//		new_child->s = so->ot.s * new_child->s;
		//SceneObject* new_so = AddSceneObject(&so->children, new_child);
		SceneObject new_so;
		new_so.children = {};
		new_so.ot = *new_child;

        int handle = YoyoPushBack(&so->children.buffer, new_so);

        SceneObject* new_so_ptr = GetSceneObject(&so->children,handle);
        new_so_ptr->handle = handle;
		return handle;
    }

//NOTE(Ray):When adding a chid ot p is local position and p is offset from parents ot p.
	static int AddChildToSceneObject(int so_index,SceneObjectBuffer* buffer,float3 p,quaternion r,float3 s)
    {
        ObjectTransform new_child;
        new_child.p = p;
        new_child.r = r;
        new_child.s = s;
        return AddChildToSceneObject(so_index,buffer,&new_child);
    }
    
	static void UpdateChildren(SceneObject* parent_so,float3* position_sum,quaternion* rotation_product)
    {
		SceneObject* child_so;
		while (child_so = YoyoIterateVector(&parent_so->children.buffer, SceneObject))
		{
			ObjectTransform* ot = &child_so->ot;
			float3 current_p_sum = *position_sum;
			quaternion current_r_product = *rotation_product;
			ot->p = current_p_sum += rotate((current_r_product), ot->local_p);
			ot->r = current_r_product = current_r_product * ot->local_r;
			//TODO(Ray):Properly handle child scaling.
			ot->s = parent_so->ot.s * ot->local_s;
			YoyoUpdateObjectTransform(ot);
			//NOTE(Ray):This is depth first recursion once we get to a leaf with no children we should exit out.
			//This is bad for several reasons 
			//reason1: if we get to have 10000 game objects could blow the stack
			//reason2: do we need another reason? 
			//Why are we doing this? because its the easiest way at the moment will rework this later if need be.
			//Another option would be to have a data structure that has a pointer to the parent similar to teh skeletal animation system.
			//Lets go this route see how it goes.
			//Keep updating children children all the way down.
			UpdateChildren(child_so, &current_p_sum, &current_r_product);
		}
		YoyoResetVectorIterator(&parent_so->children.buffer);
    }

    static void UpdateSceneObjects(SceneObjectBuffer* buffer,float3* position_sum,quaternion* rotation_product)
    {
        SceneObject* so;
        while(so = YoyoIterateVector(&buffer->buffer,SceneObject))
        {
			ObjectTransform* parent_ot = &so->ot;
			YoyoUpdateObjectTransform(parent_ot);
            float3 current_p_sum = *position_sum;
			current_p_sum += parent_ot->p;
			*rotation_product = parent_ot->r;
            UpdateChildren(so, &current_p_sum, rotation_product);
        }
		YoyoResetVectorIterator(&buffer->buffer);
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
    static Scene* CreateEmptyScene(SceneBuffer* buffer,int max_scene_objects = 10000)
    {
        Scene scene;
        scene.buffer_count = 0;
	    SceneObjectCode::InitSceneObject(&scene.scene_object_buffer,max_scene_objects);
        scene.state_flags = scene_state_default;
        YoyoPushBack(&buffer->buffer,scene);
        return YoyoPeekVectorElement(Scene,&buffer->buffer);
    }

    //NOTE(Ray):For updating all scenes?
    static void UpdateSceneBuffer(SceneBuffer* buffer)
    {
        Scene* scene;
        
        quaternion product = quaternion::identity();
        while(scene = YoyoIterateVector(&buffer->buffer,Scene))
        {
            float3 sum = float3(0);
			SceneObjectCode::UpdateSceneObjects(&scene->scene_object_buffer, &sum, &product);
        }
		YoyoResetVectorIterator(&buffer->buffer);
    }
}


