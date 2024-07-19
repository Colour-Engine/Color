#pragma once

#include "Containers/String.h"
#include "Templates/Scope.h"

class FNativeFileSystem
{
public:
	virtual void SetWorkingDir(const FString& WorkingDir) = 0;
	virtual FString GetWorkingDir() const = 0;

	virtual FString Abs(const FString& Path) const = 0;
	virtual FString Rel(const FString& Path) const = 0;

	virtual bool CreateNewDirectory(const FString& Path) = 0;
	virtual bool DoesDirectoryExist(const FString& Path) const = 0;

	virtual bool WriteToFile(const FString& Filepath, const FString& Data) = 0;
	virtual bool AppendToFile(const FString& Filepath, const FString& Data) = 0;

	virtual bool DoesFileExist(const FString& Filepath) const = 0;
	virtual uint_t GetFileSize(const FString& Filepath) const = 0;
	virtual bool ReadFile(const FString& Filepath, FString& OutData) const = 0;
};

class FFileSystem
{
public:
	static void InitFS();

	static FString GetLeadingDirectories(const FString& Filepath);
	static FString ExtractFilenameFromFilepath(const FString& Filepath);
	static FString ExtractExtensionFromFilepath(const FString& Filepath);

	static FString AppendPath(FStringView Path, FStringView PathToAppend);
	static FString RemoveFilenameFromFilepath(const FString& Filepath);

	static void SetWorkingDir(const FString& WorkingDir);
	static FString GetWorkingDir();

	static FString Abs(const FString& Path);
	static FString Rel(const FString& Path);

	static bool CreateNewDirectory(const FString& Path);
	static bool CreateDirectories(const FString& Path);
	static bool DoesDirectoryExist(const FString& Path);

	static void SetupDirectoriesForFile(const FString& Filepath);
	static bool WriteToFile(const FString& Filepath, const FString& Data);
	static bool AppendToFile(const FString& Filepath, const FString& Data);

	static bool DoesFileExist(const FString& Filepath);
	static uint_t GetFileSize(const FString& Filepath);
	static bool ReadFile(const FString& Filepath, FString& OutData);
private:
	static TScope<FNativeFileSystem> Instance;
};

// A better way to express FFileSystem::AppendPath.
inline FString operator/(FStringView Path, FStringView PathToAppend)
{
	return FFileSystem::AppendPath(Path, PathToAppend);
}
