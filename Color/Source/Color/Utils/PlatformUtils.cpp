#include "ColorPCH.h"
#include "PlatformUtils.h"

#include "Platform/Windows/WindowsPlatformUtils.h"
#include "Platform/Linux/LinuxPlatformUtils.h"

TScope<FPlatformUtils> FPlatformUtils::Instance = MakeScope<ConcatWithPlatformNameDef(PlatformUtils)>();

float FPlatformUtils::GetTime()
{
	return Instance->GetTime_Impl();
}
