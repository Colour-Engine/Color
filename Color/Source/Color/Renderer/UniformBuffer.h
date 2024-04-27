#pragma once

#include "Core/Base.h"

class FUniformBuffer
{
public:
	virtual ~FUniformBuffer() = default;
	virtual void SetData(const void* Data, uint32 Size, uint32 Offset = 0) = 0;

	static TRef<FUniformBuffer> New(uint32 Size, uint32 Binding);
};
