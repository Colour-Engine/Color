#include "ColorPCH.h"
#include "SceneImporter.h"

#include "Core/Application.h"
#include "Project/Project.h"
#include "Utils/FileSystem.h"

TRef<FScene> FSceneImporter::ImportScene(HAsset Handle, const FAssetMetadata& Metadata)
{
	return LoadScene(FProject::GetActive()->GetAssetPath(Metadata.Filepath));
}

TRef<FScene> FSceneImporter::LoadScene(const FString& Filepath)
{
	TRef<FScene> Scene = MakeRef<FScene>();

	FString Data;
	if (!FFileSystem::ReadFile(Filepath, Data))
	{
		CL_CORE_ERROR("FSceneImporter::LoadScene - Failed to load scene from file '%s'! Specified file does not exist or is inaccessible by the application.");
		return nullptr;
	}

	GetGlobalSerializationManager()->DeserializeScene(Scene.Get(), FArchive::FromCLARF(Data));
	return Scene;
}
