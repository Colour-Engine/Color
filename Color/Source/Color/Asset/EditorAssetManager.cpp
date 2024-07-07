#include "ColorPCH.h"
#include "EditorAssetManager.h"

#include "Core/Archive.h"
#include "Project/Project.h"

#include "Utils/ArchiveHelpers.h"
#include "Asset/AssetImporter.h"
#include "Utils/FileSystem.h"

static std::unordered_map<FString, EAssetType> ExtensionAssetTypeMap =
{
	{ ".clscene", EAssetType::Scene     },

	// Texture2D extensions (all extensions below are supported by stb_image)
	{ ".png",     EAssetType::Texture2D },
	{ ".jpg",     EAssetType::Texture2D },
	{ ".jpeg",    EAssetType::Texture2D },
	{ ".tga",     EAssetType::Texture2D },
	{ ".bmp",     EAssetType::Texture2D },
};

static EAssetType GetAssetTypeFromExtension(const FString& Extension)
{
	if (!ExtensionAssetTypeMap.contains(Extension))
	{
		CL_CORE_ERROR("GetAssetTypeFromExtension failure, specified extension ('%s') couldn't be matched to any asset type.");
		return EAssetType::None;
	}

	return ExtensionAssetTypeMap[Extension];
}

void FEditorAssetManager::ImportAsset(const FString& Filepath)
{
	HAsset Handle = GenerateAssetHandle();
	FAssetMetadata Metadata;

	Metadata.Filepath = Filepath;
	Metadata.Type = ExtensionAssetTypeMap[FFileSystem::ExtractExtensionFromFilepath(Filepath)];

	verifya(Metadata.Type != EAssetType::None);
	TRef<FAsset> Asset = FAssetImporter::ImportAsset(Handle, Metadata);

	if (Asset)
	{
		Asset->AssetHandle = Handle;

		LoadedAssets[Handle] = Asset;
		AssetRegistry[Handle] = Metadata;

		SerializeAssetRegistry();
	}
}

EAssetUnloadResult FEditorAssetManager::UnloadAsset(HAsset Handle)
{
	if (!IsAssetLoaded(Handle))
	{
		return EAssetUnloadResult::NotLoaded;
	}

	// Will unload, of course only if nothing references it because assets are TRefs.
	bool bUnloaded = LoadedAssets[Handle].GetRefCount() <= 1;
	LoadedAssets[Handle].Reset();
	LoadedAssets.erase(Handle);

	return bUnloaded ? EAssetUnloadResult::Success : EAssetUnloadResult::AssetReferencedElsewhere;
}

TRef<FAsset> FEditorAssetManager::GetAsset(HAsset Handle)
{
	if (!IsAssetHandleValid(Handle))
	{
		// Asset not found in the registry, it is an invalid asset
		return nullptr;
	}

	TRef<FAsset> Asset;
	if (IsAssetLoaded(Handle))
	{
		// Asset is already loaded, return it
		return Asset;
	}
	else
	{
		// Asset isn't loaded, load it
		const FAssetMetadata& Metadata = GetMetadata(Handle);
		Asset = FAssetImporter::ImportAsset(Handle, Metadata);

		if (!Asset)
		{
			CL_CORE_ERROR("FEditorAssetManager::GetAsset - Import operation for asset with Handle: %e and %s failed.", Handle, *Metadata.Format());
		}

		LoadedAssets[Handle] = Asset;
	}

	return Asset;
}

bool FEditorAssetManager::IsAssetHandleValid(HAsset Handle) const
{
	return Handle != HASSET_INVALID_HANDLE && AssetRegistry.contains(Handle);
}

bool FEditorAssetManager::IsAssetLoaded(HAsset Handle) const
{
	return LoadedAssets.contains(Handle);
}

EAssetType FEditorAssetManager::GetAssetType(HAsset Handle) const
{
	const FAssetMetadata& Metadata = GetMetadata(Handle);

	if (Metadata == CNullAssetMetadata)
	{
		return EAssetType::None;
	}

	return Metadata.Type;
}

const FAssetMetadata& FEditorAssetManager::GetMetadata(HAsset Handle) const
{
	// Check if there's an entry for this asset handle within the asset registry.
	if (!IsAssetHandleValid(Handle))
	{
		return CNullAssetMetadata;
	}
	
	return AssetRegistry.at(Handle);
}

const FString& FEditorAssetManager::GetFilepath(HAsset Handle) const
{
	const FAssetMetadata& Metadata = GetMetadata(Handle);

	if (Metadata == CNullAssetMetadata)
	{
		static FString Empty = "";
		return Empty;
	}

	return Metadata.Filepath;
}

void FEditorAssetManager::SerializeAssetRegistry()
{
	FArchive Ar;

	for (auto&& [Handle, Metadata] : AssetRegistry)
	{
		Ar.SetField
		(
			FString::FromInt(Handle),
			ArTypes::Group(
				{
					{ "Type", AssetTypeToString(Metadata.Type) },
					{ "Filepath", Metadata.Filepath }
				}
			)
		);
	}

	FString AssetRegistryPath = FProject::GetActiveAssetRegistryPath();
	FFileSystem::WriteToFile(AssetRegistryPath, Ar.ConvertToCLARF());
}

void FEditorAssetManager::DeserializeAssetRegistry()
{
	AssetRegistry.clear();
	FString AssetRegistryPath = FProject::GetActiveAssetRegistryPath();

	FString Data;
	if (!FFileSystem::ReadFile(AssetRegistryPath, Data))
	{
		CL_CORE_WARN("Failed to deserialize the asset registry! The asset registry either doesn't exist or is inaccessible by the program!");
		return;
	}

	FArchive Ar = FArchive::FromCLARF(Data);
	for (auto&& [FieldName, FieldValue] : Ar)
	{
		const FArchive& MetadataField = FieldValue.AsGroup();
		bool bFlag;

		HAsset AssetHandle = FieldName.ToInteger<HAsset>();
		FAssetMetadata Metadata;
		FString Type;

		GetFieldChecked(MetadataField, "Type", String, Type, bFlag);
		GetFieldChecked(MetadataField, "Filepath", String, Metadata.Filepath, bFlag);

		Metadata.Type = AssetTypeFromString(*Type);
		AssetRegistry[AssetHandle] = Metadata;
	}
}
