#pragma once

#include "Asset/AssetMetadata.h"
#include "Scene/Scene.h"

class FSceneImporter
{
public:
	static TRef<FScene> ImportScene(HAsset Handle, const FAssetMetadata& Metadata);
	static TRef<FScene> LoadScene(const FString& Filepath);
};
