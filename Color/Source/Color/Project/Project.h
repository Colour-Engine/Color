#pragma once

#include "Core/Base.h"

#define COLOR_PROJECT_FILE_EXTENSION ".clproj"

struct FProjectInfo
{
	FString Name = "Unnamed";

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
	FProject(const FProjectInfo& InProjectInfo, const FString& InProjectDirectory = "");

	static TRef<FProject> Load(const FString& Filepath, bool bSetAsActive = true);
	static TRef<FProject> Load(const FString& ProjectName, const FString& ProjectDirectory, bool bSetAsActive = true);

	static void Save(const TRef<FProject>& Project, const FString& SaveFilepath, bool bSetAsActive = true);
	static void Save(const TRef<FProject>& Project, bool bSetAsActive = true);

	void SaveActive(const FString& SaveDirectory);
	void SaveActive();

	const FProjectInfo& GetProjectInfo() const { return ProjectInfo; }
	const FString& GetProjectDirectory() const { return ProjectDirectory; }

	static TRef<FProject>& GetActive() { return Active; }
private:
	// Info/data/config about the project.
	FProjectInfo ProjectInfo;

	// The directory that contains the project (often .clproj) file, which is also the root project directory.
	FString ProjectDirectory;

	inline static TRef<FProject> Active;
};
