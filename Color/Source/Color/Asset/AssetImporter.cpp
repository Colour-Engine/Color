#include "ColorPCH.h"
#include "AssetImporter.h"

#include "Asset/Importers/TextureImporter.h"
#include "Asset/Importers/SceneImporter.h"

#include <unordered_map>
#include <functional>

typedef std::function<TRef<FAsset>(HAsset, const FAssetMetadata&)> SigAssetImportFn;
static std::unordered_map<EAssetType, SigAssetImportFn> AssetImportFunctions =
{
	{ EAssetType::Scene, FSceneImporter::ImportScene },
	{ EAssetType::Texture2D, FTextureImporter::ImportTexture2D }
};

TRef<FAsset> FAssetImporter::ImportAsset(HAsset Handle, const FAssetMetadata& Metadata)
{
	if (!AssetImportFunctions.contains(Metadata.Type))
	{
		CL_CORE_ERROR(
			"FAssetImporter::ImportAsset - Asset with { Handle: %e, Type: '%s', Filepath: '%s' } couldn't be importer due to no importer being available for it's type.",
			Handle, AssetTypeToString(Metadata.Type), *Metadata.Filepath
		);

		return nullptr;
	}

	return AssetImportFunctions[Metadata.Type](Handle, Metadata);
}
