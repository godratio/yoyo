#if !defined(YOYOYO_H)

#if WINDOWS
#define Assert(Expression) if(!(Expression)){*(int *)0 = 0;};
#else
#define Assert(Expression) if(!(Expression)){__builtin_trap();};
#endif

#include "yoyoyo_math.h"

//TODO(Ray):Move this dependence from here to yoyomemory
#include "../api/api_include.h"

#include "meow_hash_yoyo/meow_intrinsics.h"
#include "meow_hash_yoyo/meow_hash.h"

#define ArrayCount(Array) sizeof(Array) / sizeof((Array)[0])

#define KiloBytes(Value) (Value * 1024LL)
#define MegaBytes(Value) (KiloBytes(Value) * 1024LL)
#define GigaBytes(Value) (MegaBytes(Value) * 1024LL)
#define TeraBytes(Value) (GigaBytes(Value) * 1024LL)



#include "yoyoyo_ascii_vector.h"
#include "yoyoyo_hashtable.h"


#include "yoyoyo_math_ext.h"
#include "yoyoyo_scene.h"


#define YOYOYO_H
#endif
