#pragma once

#include "Asset/AssetMetadata.h"

class FAssetImporter
{
public:
	static TRef<FAsset> ImportAsset(HAsset Handle, const FAssetMetadata& Metadata);
};
