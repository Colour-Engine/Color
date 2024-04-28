#pragma once

#include "Core/Base.h"

#ifdef CL_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#ifdef CL_PLATFORM_LINUX
	#include <linux/limits.h>
	#include <unistd.h>
#endif

// Fine. I'll use it.
// Actually, no. Let's not use it.
// Actually, yes (ok ok I'll stop and use it).
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
