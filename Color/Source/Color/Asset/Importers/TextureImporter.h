#pragma once

#include "Asset/AssetMetadata.h"
#include "Renderer/Texture.h"

class FTextureImporter
{
public:
	static TRef<FTexture2D> ImportTexture2D(HAsset Asset, const FAssetMetadata& Metadata);
	static TRef<FTexture2D> LoadTexture2D(const FString& Filepath);
};
