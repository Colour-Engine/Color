#include "ColorPCH.h"
#include "FileSystem.h"

#include "Platform/Windows/WindowsFileSystem.h"
#include "Platform/Linux/LinuxFileSystem.h"

TScope<FNativeFileSystem> FFileSystem::Instance = MakeScope<ConcatWithPlatformNameDef(NativeFileSystem)>();

void FFileSystem::SetWorkingDir(const FString& WorkingDir)
{
	Instance->SetWorkingDir(WorkingDir);
}

FString FFileSystem::GetWorkingDir()
{
	return Instance->GetWorkingDir();
}
