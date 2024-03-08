#include "ColorPCH.h"
#include "WindowsFileSystem.h"

void FWindowsNativeFileSystem::SetWorkingDir(const FString& WorkingDir)
{
	SetCurrentDirectoryA(*WorkingDir);
}

FString FWindowsNativeFileSystem::GetWorkingDir() const
{
	CHAR Buffer[MAX_PATH + 1];
	GetCurrentDirectoryA(MAX_PATH + 1, Buffer);

	return Buffer;
}
