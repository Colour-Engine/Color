#pragma once

#include "Misc/IntTypes.h"

// The purpose of RenderCaps is to specify the limits of rendering capability.
// Currently, all the limits are set in stone single values but in the future,
// the caps will be dynamically computed by analyzing the hardware capabilities of the GPU.
struct FRenderCaps
{
	inline static uint32 MaxQuads = 20000;
	inline static uint32 MaxVertices = MaxQuads * 4;
	inline static uint32 MaxIndices = MaxQuads * 6;
	inline static uint32 MaxTextureSlots = 32;
};
