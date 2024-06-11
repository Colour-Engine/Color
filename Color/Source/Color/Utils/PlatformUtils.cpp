#include "ColorPCH.h"
#include "PlatformUtils.h"

#include "Platform/Windows/WindowsPlatformUtils.h"
#include "Platform/Linux/LinuxPlatformUtils.h"

TScope<FPlatformUtils> FPlatformUtils::Instance = MakeScope<ConcatWithPlatformNameDef(PlatformUtils)>();

float FPlatformUtils::GetTime()
{
	return Instance->GetTime_Impl();
}

glm::ivec2 FPlatformUtils::GetMonitorSize()
{
	return Instance->GetMonitorSize_Impl();
}

int32 FPlatformUtils::GetMonitorWidth()
{
	return GetMonitorSize().x;
}

int32 FPlatformUtils::GetMonitorHeight()
{
	return GetMonitorSize().y;
}
