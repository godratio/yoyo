#pragma once

//TODO(Ray):Move this dependence from here to yoyomemory
#include "../api/api_include.h"

#include "meow_hash/meow_intrinsics.h"
#include "meow_hash/meow_hash.h"

#define Assert(Expression) if(!(Expression)){*(int *)0 = 0;};
#define ArrayCount(Array) sizeof(Array) / sizeof((Array)[0])

#define KiloBytes(Value) (Value * 1024LL)
#define MegaBytes(Value) (KiloBytes(Value) * 1024LL)
#define GigaBytes(Value) (MegaBytes(Value) * 1024LL)
#define TeraBytes(Value) (GigaBytes(Value) * 1024LL)

#include "yoyoyo_ascii_vector.h"
#include "yoyoyo_math.h"
#include "yoyoyo_hashtable.h"
#include "yoyoyo_scene.h"
