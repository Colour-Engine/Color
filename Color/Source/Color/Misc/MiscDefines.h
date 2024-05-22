#pragma once

#ifdef CL_PLATFORM_WINDOWS
	#define CL_PLATFORM_DEBUGBREAK() __debugbreak()
#elif defined CL_PLATFORM_LINUX
	#include <signal.h>
	#define CL_PLATFORM_DEBUGBREAK() raise(SIGTRAP)
#else
	#error "This platform doesn't support debugbreak!"
#endif

#define CL_BIT(X) (1 << X)

#define CL_CONCAT_IMPL(L, R) L##R
#define CL_CONCAT(L, R)      CL_CONCAT_IMPL(L, R)

#define CL_STRINGIFY_IMPL(X) #X
#define CL_STRINGIFY(X)      CL_STRINGIFY_IMPL(X)

#define CL_NODISCARD [[nodiscard]]
