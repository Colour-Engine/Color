#pragma once

#include "Asset/AssetManagerBase.h"
#include "Asset/AssetMetadata.h"

#include <unordered_map>

using FAssetRegistry = std::map<HAsset, FAssetMetadata>;

class FEditorAssetManager : public FAssetManagerBase
{
public:
	// Imports an asset from a filepath, recognizing the asset type via the asset file's extension.
	void ImportAsset(const FString& Filepath);

	virtual EAssetUnloadResult UnloadAsset(HAsset Handle) override;
	virtual TRef<FAsset> GetAsset(HAsset Handle) override;

	virtual bool IsAssetHandleValid(HAsset Handle) const override;
	virtual bool IsAssetLoaded(HAsset Handle) const override;
	virtual EAssetType GetAssetType(HAsset Handle) const override;

	const FAssetMetadata& GetMetadata(HAsset Handle) const;
	const FString& GetFilepath(HAsset Handle) const;

	const FAssetRegistry& GetAssetRegistry() const { return AssetRegistry; }
	const FAssetMap& GetLoadedAssets() const { return LoadedAssets; }

	void SerializeAssetRegistry();
	void DeserializeAssetRegistry();
private:
	FAssetRegistry AssetRegistry;
	FAssetMap LoadedAssets;
};
