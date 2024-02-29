#pragma once

#include "Misc/IntTypes.h"

struct FMemory
{
	static void* Copy(void* Dest, const void* Src, uint_t Count);
	static void* Move(void* Dest, const void* Src, uint_t Count);
};
