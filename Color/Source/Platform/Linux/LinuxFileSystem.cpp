#include "ColorPCH.h"
#include "LinuxFileSystem.h"

void FLinuxNativeFileSystem::SetWorkingDir(const FString& WorkingDir)
{
	chdir(*WorkingDir);
}

FString FLinuxNativeFileSystem::GetWorkingDir() const
{
	char Buffer[PATH_MAX];
	getcwd(Buffer, PATH_MAX);

	return Buffer;
}
