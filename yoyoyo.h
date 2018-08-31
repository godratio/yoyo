#pragma once

#define Assert(Expression) if(!(Expression)){*(int *)0 = 0;};
#define ArrayCount(Array) sizeof(Array) / sizeof((Array)[0])


#define KiloBytes(Value) (Value * 1024LL)
#define MegaBytes(Value) (KiloBytes(Value) * 1024LL)
#define GigaBytes(Value) (MegaBytes(Value) * 1024LL)
#define TeraBytes(Value) (GigaBytes(Value) * 1024LL)



