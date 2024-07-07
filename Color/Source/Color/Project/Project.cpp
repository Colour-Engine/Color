#include "ColorPCH.h"
#include "Project.h"

#include "Core/Archive.h"
#include "Utils/ArchiveHelpers.h"
#include "Utils/FileSystem.h"

FProject::FProject(const FProjectInfo& InProjectInfo, const FString& InProjectDirectory, const TRef<FAssetManagerBase>& InAssetManager)
	: ProjectInfo(InProjectInfo), ProjectDirectory(InProjectDirectory), AssetManager(InAssetManager)
{
}

TRef<FProject> FProject::Load(const FString& Filepath, bool bSetAsActive)
{
	TRef<FProject> Project = MakeRef<FProject>();

	FString Data;
	if (!FFileSystem::ReadFile(Filepath, Data))
	{
		CL_CORE_ERROR("Failed to open project file '%s'!", *Filepath);
		return nullptr;
	}
	
	if (bSetAsActive)
	{
		Active = Project;
	}

	FArchive Ar = FArchive::FromCLARF(Data);
	bool bFlag;

	GetFieldChecked(Ar, "Name", String, Project->ProjectInfo.Name, bFlag);
	GetFieldChecked(Ar, "RuntimeStartupScene", UInteger, Project->ProjectInfo.RuntimeStartupScene, bFlag);
	GetFieldChecked(Ar, "EditorStartupScene", UInteger, Project->ProjectInfo.EditorStartupScene, bFlag);
	GetFieldChecked(Ar, "AssetDirectory", String, Project->ProjectInfo.AssetDirectory, bFlag);
	GetFieldChecked(Ar, "AssetRegistryPath", String, Project->ProjectInfo.AssetRegistryPath, bFlag);
	GetFieldChecked(Ar, "CacheDirectory", String, Project->ProjectInfo.CacheDirectory, bFlag);

	Project->AssetManager = MakeRef<FEditorAssetManager>();
	Project->GetEditorAssetManager()->DeserializeAssetRegistry();

	return Project;
}

TRef<FProject> FProject::Load(const FString& ProjectName, const FString& ProjectDirectory, bool bSetAsActive)
{
	return Load(FFileSystem::AppendPath(ProjectDirectory, ProjectName + COLOR_PROJECT_FILE_EXTENSION));
}

void FProject::Save(const TRef<FProject>& Project, const FString& SaveFilepath, bool bSetAsActive)
{
	FArchive Ar;

	Ar.SetField("Name", Project->ProjectInfo.Name);
	Ar.SetField("RuntimeStartupScene", Project->ProjectInfo.RuntimeStartupScene);
	Ar.SetField("EditorStartupScene", Project->ProjectInfo.EditorStartupScene);
	Ar.SetField("AssetDirectory", Project->ProjectInfo.AssetDirectory);
	Ar.SetField("AssetRegistryPath", Project->ProjectInfo.AssetRegistryPath);
	Ar.SetField("CacheDirectory", Project->ProjectInfo.CacheDirectory);

	FFileSystem::WriteToFile(SaveFilepath, Ar.ConvertToCLARF());
	Project->ProjectDirectory = FFileSystem::RemoveFilenameFromFilepath(SaveFilepath);

	if (bSetAsActive)
	{
		Active = Project;
	}
}

void FProject::Save(const TRef<FProject>& Project, bool bSetAsActive)
{
	Save(Project, FFileSystem::AppendPath(Project->ProjectDirectory, Project->ProjectInfo.Name + COLOR_PROJECT_FILE_EXTENSION), bSetAsActive);
}

void FProject::SaveActive(const FString& SaveDirectory)
{
	FProject::Save(Active, SaveDirectory);
}

void FProject::SaveActive()
{
	FProject::Save(Active);
}

FString FProject::GetAssetDirectoryRelativeToProjectDirectory() const
{
	return FFileSystem::AppendPath(ProjectDirectory, ProjectInfo.AssetDirectory);
}

FString FProject::GetAssetPath(const FString& RelativePath) const
{
	return FFileSystem::AppendPath(GetAssetDirectoryRelativeToProjectDirectory(), RelativePath);
}
