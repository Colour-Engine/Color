#pragma once

#include "Asset/Asset.h"

struct FAssetMetadata
{
	EAssetType Type = EAssetType::None;
	FString Filepath; // Relative to FProjectInfo::AssetDirectory

	FString Format() const
	{
		return FString::Format("AssetMetadata{ Type: '%s', Filepath: '%s' }", AssetTypeToString(Type), *Filepath);
	}

	bool operator==(const FAssetMetadata&) const = default;
	bool operator!=(const FAssetMetadata&) const = default;
};

inline FAssetMetadata CNullAssetMetadata =
{
	EAssetType::None,
	"."
};
