#pragma once

#include "Core/Base.h"
#include "Asset/AssetMetadata.h"

#include "Asset/RuntimeAssetManager.h"
#include "Asset/EditorAssetManager.h"

#define COLOR_PROJECT_FILE_EXTENSION ".clproj"

struct FProjectInfo
{
	FString Name = "Unnamed";

	HAsset RuntimeStartupScene = 0;
	HAsset EditorStartupScene  = 0;

	FString AssetDirectory = "Assets";
	FString AssetRegistryPath = ".classetreg"; // Relative to AssetDirectory!

	FString CacheDirectory = "Cache";
};

class FProject
{
public:
	FProject() = default;
	FProject(const FProject&) = default;
	FProject& operator=(const FProject&) = default;
	FProject(const FProjectInfo& InProjectInfo, const FString& InProjectDirectory = "", const TRef<FAssetManagerBase>& InAssetManager = nullptr);

	static TRef<FProject> Load(const FString& Filepath, bool bSetAsActive = true);
	static TRef<FProject> Load(const FString& ProjectName, const FString& ProjectDirectory, bool bSetAsActive = true);

	static void Save(const TRef<FProject>& Project, const FString& SaveFilepath, bool bSetAsActive = true);
	static void Save(const TRef<FProject>& Project, bool bSetAsActive = true);

	static void SaveActive(const FString& SaveDirectory);
	static void SaveActive();

	static FString GetActiveAssetDirectoryRelativeToProjectDirectory() { return GetActive()->GetAssetDirectoryRelativeToProjectDirectory(); }
	// Returns path relative to AssetDirectory, which is relative to ProjectDirectory in itself.
	static FString GetActiveAssetPath(const FString& RelativePath) { return GetActive()->GetAssetPath(RelativePath); }

	FString GetAssetDirectoryRelativeToProjectDirectory() const;
	// Returns path relative to AssetDirectory, which is relative to ProjectDirectory in itself.
	FString GetAssetPath(const FString& RelativePath) const;

	const FProjectInfo& GetProjectInfo() const { return ProjectInfo; }
	const FString& GetProjectDirectory() const { return ProjectDirectory; }
	const FString& GetProjectName() const { return ProjectInfo.Name; }
	const FString& GetAssetDirectory() const { return ProjectInfo.AssetDirectory; }
	const FString& GetAssetRegistryPath() const { return ProjectInfo.AssetRegistryPath; }
	const FString& GetCacheDirectory() const { return ProjectInfo.CacheDirectory; }

	FRuntimeAssetManager* GetRuntimeAssetManager() const { return Cast<FRuntimeAssetManager>(AssetManager.Get()); }
	FEditorAssetManager* GetEditorAssetManager() const { return Cast<FEditorAssetManager>(AssetManager.Get()); }
	FAssetManagerBase* GetAssetManager() const { return AssetManager.Get(); }

	static const FProjectInfo& GetActiveProjectInfo() { return GetActive()->GetProjectInfo(); }
	static const FString& GetActiveProjectDirectory() { return GetActive()->GetProjectDirectory(); }
	static const FString& GetActiveProjectName() { return GetActive()->GetProjectName(); }
	static const FString& GetActiveAssetDirectory() { return GetActive()->GetAssetDirectory(); }
	static const FString& GetActiveAssetRegistryPath() { return GetActive()->GetAssetRegistryPath(); }
	static const FString& GetActiveCacheDirectory() { return GetActive()->GetCacheDirectory(); }

	static FRuntimeAssetManager* GetActiveRuntimeAssetManager() { return GetActive()->GetRuntimeAssetManager(); }
	static FEditorAssetManager* GetActiveEditorAssetManager() { return GetActive()->GetEditorAssetManager(); }
	static FAssetManagerBase* GetActiveAssetManager() { return GetActive()->GetAssetManager(); }

	static void SetActive(const TRef<FProject>& NewActive) { Active = NewActive; }
	static TRef<FProject>& GetActive() { return Active; }
private:
	// Info/data/config about the project.
	FProjectInfo ProjectInfo;

	// The directory that contains the project (often .clproj) file, which is also the root project directory.
	FString ProjectDirectory;

	TRef<FAssetManagerBase> AssetManager;

	inline static TRef<FProject> Active;
};
