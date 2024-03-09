#pragma once

#include "Utils/FileSystem.h"

class FWindowsNativeFileSystem : public FNativeFileSystem
{
public:
	virtual void SetWorkingDir(const FString& WorkingDir) override;
	virtual FString GetWorkingDir() const override;

	virtual bool CreateNewDirectory(const FString& Path) override;
	virtual bool DoesDirectoryExist(const FString& Path) const override;

	virtual bool WriteToFile(const FString& Filepath, const FString& Data) override;
	virtual bool AppendToFile(const FString& Filepath, const FString& Data) override;
private:
	bool WriteToFileEx(const FString& Filepath, const FString& Data, bool bAppend = false);
};
