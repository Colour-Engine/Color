#pragma once

#include <stdarg.h>

class FRuntimeErrorManager
{
public:
	static void RuntimeErrorLog(uint8 Level, const char* Format, va_list Arguments);
	static void RuntimeErrorLog(uint8 Level, const char* Format, ...);

	static void RtError(const char* Format, ...);
	static void RtFatal(const char* Format, ...);
};
