#pragma once

#include "Core/Base.h"

// Paths is a structure defining the leading paths to engine/game requited files and directories.
// It's solely used for organization purposes.
struct FPaths
{
	// Content-related
	static FString GetContentDirectory();
	static FString GetEngineAssetsDirectory();

	// Cache/Temp-related
	static FString GetCacheDirectory();
	static FString GetShaderCacheDirectory();
	static FString GetAPIShaderCacheDirectory(const FString& API);

	// Source-related (currently reserved for future work)
};
