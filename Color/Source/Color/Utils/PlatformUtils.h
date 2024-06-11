#pragma once

#include "Core/Base.h"

#include <glm/vec2.hpp>

class FPlatformUtils
{
public:
	virtual ~FPlatformUtils() = default;

	static float GetTime();
	static glm::ivec2 GetMonitorSize();
	static int32 GetMonitorWidth();
	static int32 GetMonitorHeight();
protected:
	virtual float GetTime_Impl() const = 0;
	virtual glm::ivec2 GetMonitorSize_Impl() const = 0;
private:
	static TScope<FPlatformUtils> Instance;
};
