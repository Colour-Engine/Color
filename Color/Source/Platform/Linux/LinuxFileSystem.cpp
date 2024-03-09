#include "ColorPCH.h"
#include "LinuxFileSystem.h"

#include <sys/stat.h>

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

bool FLinuxNativeFileSystem::CreateNewDirectory(const FString& Path)
{
	return mkdir(*Path);
}

bool FLinuxNativeFileSystem::DoesDirectoryExist(const FString& Path) const
{
	struct stat Stat;
	return stat(*Path, &Stat) == 0 && S_ISDIR(Stat.sb_mode);
}

bool WriteToFileEx(const FString& Filepath, const FString& Data, bool bAppend)
{
	FILE* File = fopen(*Filepath, bAppend ? "ab" : "wb");

	if (!File)
	{
		return false;
	}

	size_t NoWrittenChars = fwrite(*Data, sizeof(FString::CharType), Data.Len(), File);
	fclose(File);

	return NoWrittenChars == Data.Size;
}

bool FLinuxNativeFileSystem::WriteToFile(const FString& Filepath, const FString& Data)
{
	return WriteToFileEx(Filepath, Data, false);
}

bool FLinuxNativeFileSystem::AppendToFile(const FString& Filepath, const FString& Data)
{
	return WriteToFileEx(Filepath, Data, true);
}
