#pragma once

#include "Utils/FileSystem.h"

class FWindowsNativeFileSystem : public FNativeFileSystem
{
public:
	virtual void SetWorkingDir(const FString& WorkingDir) override;
	virtual FString GetWorkingDir() const override;

	virtual FString Abs(const FString& Path) const override;
	virtual FString Rel(const FString& Path) const override;

	virtual bool CreateNewDirectory(const FString& Path) override;
	virtual bool DoesDirectoryExist(const FString& Path) const override;

	virtual bool WriteToFile(const FString& Filepath, const FString& Data) override;
	virtual bool AppendToFile(const FString& Filepath, const FString& Data) override;

	virtual bool DoesFileExist(const FString& Filepath) const override;
	virtual uint_t GetFileSize(const FString& Filepath) const override;
	virtual bool ReadFile(const FString& Filepath, FString& OutData) const override;
private:
	bool WriteToFileEx(const FString& Filepath, const FString& Data, bool bAppend = false);
};
