#include "ColorPCH.h"
#include "Asset.h"

#include "Project/Project.h"
#include "Misc/Random.h"

HAsset GenerateAssetHandle()
{
	static FRandomGenerator64 HandleGenerator;
	return HandleGenerator.UInt64();
}

const char* AssetTypeToString(EAssetType Type)
{
	switch (Type)
	{
	case EAssetType::None:      return "EAssetType::None";
	case EAssetType::Scene:     return "Scene";
	case EAssetType::Texture2D: return "Texture2D";
	}

	unreachable();
	return nullptr;
}

EAssetType AssetTypeFromString(const char* String)
{
	if (FStringUtility::Cmp(String, "Scene") == 0)
	{
		return EAssetType::Scene;
	}
	if (FStringUtility::Cmp(String, "Texture2D") == 0)
	{
		return EAssetType::Texture2D;
	}

	return EAssetType::None;
}

FAsset::~FAsset()
{
	if (bAssetTryUnloadOnDestruction && FProject::GetActive())
	{
		FProject::GetActiveAssetManager()->UnloadAsset(AssetHandle);
	}
}

void FAsset::GenerateAssetHandle(bool bAllowOverride)
{
	if (AssetHandle == HASSET_INVALID_HANDLE || bAllowOverride)
	{
		AssetHandle = ::GenerateAssetHandle();
	}
}
