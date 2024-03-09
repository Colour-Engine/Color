#pragma once

#include "Containers/String.h"
#include "Templates/Scope.h"

class FNativeFileSystem
{
public:
	virtual void SetWorkingDir(const FString& WorkingDir) = 0;
	virtual FString GetWorkingDir() const = 0;

	virtual bool CreateNewDirectory(const FString& Path) = 0;
	virtual bool DoesDirectoryExist(const FString& Path) const = 0;

	virtual bool WriteToFile(const FString& Filepath, const FString& Data) = 0;
	virtual bool AppendToFile(const FString& Filepath, const FString& Data) = 0;
};

class FFileSystem
{
public:
	static FString GetLeadingDirectories(const FString& Filepath);

	static void SetWorkingDir(const FString& WorkingDir);
	static FString GetWorkingDir();

	static bool CreateNewDirectory(const FString& Path);
	static bool CreateDirectories(const FString& Path);
	static bool DoesDirectoryExist(const FString& Path);

	static void SetupDirectoriesForFile(const FString& Filepath);
	static bool WriteToFile(const FString& Filepath, const FString& Data);
	static bool AppendToFile(const FString& Filepath, const FString& Data);
private:
	static TScope<FNativeFileSystem> Instance;
};
