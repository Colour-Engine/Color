#pragma once

#include "Containers/String.h"
#include "Templates/Scope.h"

class FNativeFileSystem
{
public:
	virtual void SetWorkingDir(const FString& WorkingDir) = 0;
	virtual FString GetWorkingDir() const = 0;
};

class FFileSystem
{
public:
	static void SetWorkingDir(const FString& WorkingDir);
	static FString GetWorkingDir();
private:
	static TScope<FNativeFileSystem> Instance;
};
