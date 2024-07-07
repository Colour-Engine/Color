#pragma once

#include "Core/Base.h"

using HAsset = uint64;
inline constexpr HAsset HASSET_INVALID_HANDLE = 0;
HAsset GenerateAssetHandle();

enum class EAssetType
{
	None = 0,
	Scene, Texture2D
};

enum class EAssetUnloadResult
{
	// Asset with the handle wasn't even loaded.
	NotLoaded = 0,

	// Asset was successfully removed and its resources were properly released.
	Success = 1,

	// Asset was successfully removed from LoadedAssets storage, but there were other TRefs referencing it,
	// meaning its resources weren't released but will be so when all TRefs referencing it go out of scope.
	AssetReferencedElsewhere = 2
};

const char* AssetTypeToString(EAssetType Type);
EAssetType AssetTypeFromString(const char* String);

// Helper macro to use in public space of FAsset inheritants. Defines both static & virtual type getter functions.
#define IMPLEMENT_ASSET(EnumeratorValue) \
	static ::EAssetType GetStaticAssetType() { return ::EAssetType::##EnumeratorValue; } \
	virtual ::EAssetType GetAssetType() const override { return GetStaticAssetType(); }

class FAsset
{
public:
	virtual ~FAsset();

	bool   bAssetTryUnloadOnDestruction = true;
	HAsset AssetHandle = HASSET_INVALID_HANDLE;
	void GenerateAssetHandle(bool bAllowOverride = false);

	virtual EAssetType GetAssetType() const = 0;
};
