#include "ColorPCH.h"
#include "WindowsPlatformUtils.h"

glm::ivec2 FWindowsPlatformUtils::GetMonitorSize_Impl() const
{
	return { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
}
