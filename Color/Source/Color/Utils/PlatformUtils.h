#pragma once

#include "Core/Base.h"

class FPlatformUtils
{
public:
	virtual ~FPlatformUtils() = default;

	static float GetTime();
protected:
	virtual float GetTime_Impl() const = 0;
private:
	static TScope<FPlatformUtils> Instance;
};
