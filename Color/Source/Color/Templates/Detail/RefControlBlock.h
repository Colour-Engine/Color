#pragma once

#include "Misc/IntTypes.h"

/**
 * The default control block that the standard TRef and TWeakRef uses.
 */
struct FRefControlBlock
{
	void*  pObject   = nullptr;
	uint32 uRefs     = 0;
	uint32 uWeakRefs = 0;
};
