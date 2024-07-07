#pragma once

#include "Asset/Asset.h"

#include <unordered_map>

using FAssetMap = std::unordered_map<HAsset, TRef<FAsset>>;

class FAssetManagerBase
{
public:
	virtual EAssetUnloadResult UnloadAsset(HAsset Handle) = 0;
	virtual TRef<FAsset> GetAsset(HAsset Handle) = 0;

	virtual bool IsAssetHandleValid(HAsset Handle) const = 0;
	virtual bool IsAssetLoaded(HAsset Handle) const = 0;
	virtual EAssetType GetAssetType(HAsset Handle) const = 0;
};
