#pragma once

#include "Misc/MiscDefines.h"

enum class EPlatform
{
	Windows,
	Linux
};

#ifdef CL_PLATFORM_WINDOWS
	inline constexpr EPlatform CPlatform = EPlatform::Windows;
	#define CL_PLATFORM_NAME Windows
#elif defined CL_PLATFORM_LINUX
	inline constexpr EPlatform CPlatform = EPlatform::Linux;
	#define CL_PLATFORM_NAME Linux
#else
	#error Unsupported platform!
#endif

#define CL_PLATFORM_NAME_STRING              CL_STRINGIFY(CL_PLATFORM_NAME)
#define ConcatWithPlatformName(X)            CL_CONCAT(CL_PLATFORM_NAME, X)
#define ConcatWithPlatformNamePfx(Prefix, X) CL_CONCAT(Prefix, CL_CONCAT(CL_PLATFORM_NAME, X))
#define ConcatWithPlatformNameDef(X)         ConcatWithPlatformNamePfx(F, X)
