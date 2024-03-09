#pragma once

#include "Misc/MiscDefines.h"

enum class EPlatform
{
	Windows,
	Linux
};

#ifdef CL_PLATFORM_WINDOWS
	inline constexpr EPlatform CPlatform = EPlatform::Windows;
	inline constexpr bool CIsStablePlatform = true;
	#define CL_PLATFORM_NAME Windows
#elif defined CL_PLATFORM_LINUX
	inline constexpr EPlatform CPlatform = EPlatform::Linux;
	inline constexpr bool CIsStablePlatform = false;
	#define CL_PLATFORM_NAME Linux
#else
	#error Unsupported platform!
#endif
inline constexpr bool CIsExperimentalPlatform = !CIsStablePlatform;

#define CL_PLATFORM_NAME_STRING              CL_STRINGIFY(CL_PLATFORM_NAME)
#define ConcatWithPlatformName(X)            CL_CONCAT(CL_PLATFORM_NAME, X)
#define ConcatWithPlatformNamePfx(Prefix, X) CL_CONCAT(Prefix, CL_CONCAT(CL_PLATFORM_NAME, X))
#define ConcatWithPlatformNameDef(X)         ConcatWithPlatformNamePfx(F, X)
