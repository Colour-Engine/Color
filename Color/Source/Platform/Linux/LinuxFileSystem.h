#pragma once

#include "Utils/FileSystem.h"

class FLinuxNativeFileSystem : public FNativeFileSystem
{
public:
	virtual void SetWorkingDir(const FString& WorkingDir) override;
	virtual FString GetWorkingDir() const override;
};
