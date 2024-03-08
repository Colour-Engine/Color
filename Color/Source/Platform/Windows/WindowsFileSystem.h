#pragma once

#include "Utils/FileSystem.h"

class FWindowsNativeFileSystem : public FNativeFileSystem
{
public:
	virtual void SetWorkingDir(const FString& WorkingDir) override;
	virtual FString GetWorkingDir() const override;
};
