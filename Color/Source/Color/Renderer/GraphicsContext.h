#pragma once

#include "Core/Base.h"

class FGraphicsContext
{
public:
	virtual ~FGraphicsContext() = default;

	virtual bool Init() = 0;
	virtual void SwapBuffers() = 0;

	static TScope<FGraphicsContext> New(void* NativeWindowHandle);
};
