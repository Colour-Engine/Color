#pragma once

#include "Misc/IntTypes.h"

namespace ExitCode
{
	typedef int32 Type;
	enum : Type
	{
		Successful = 0,
		Failure    = 1,
		FatalLog   = 2
	};
}

void RtExit(ExitCode::Type ExitCode);
