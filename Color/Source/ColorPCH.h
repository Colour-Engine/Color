#pragma once

#include "Core/Base.h"

#ifdef CL_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <io.h> // unistd.h-like
#endif

#ifdef CL_PLATFORM_LINUX
	#include <linux/limits.h>
	#include <unistd.h>
#endif

#ifdef CL_INCLUDE_STL_IN_PCH
	#include <unordered_map>
	#include <unordered_set>
	#include <string_view>
	#include <filesystem>
	#include <functional>
	#include <optional>
	#include <iostream>
	#include <fstream>
	#include <sstream>
	#include <variant>
	#include <thread>
	#include <chrono>
	#include <random>
	#include <string>
	#include <vector>
	#include <memory>
	#include <bitset>
	#include <limits>
	#include <atomic>
	#include <array>
	#include <queue>
	#include <deque>
	#include <stack>
	#include <list>
	#include <map>
	#include <set>
	#include <any>
	#include <new>
#endif

#include <stdio.h>
#include <stdlib.h>
	