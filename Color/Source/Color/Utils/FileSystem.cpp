#include "ColorPCH.h"
#include "FileSystem.h"

#include "Platform/Windows/WindowsFileSystem.h"
#include "Platform/Linux/LinuxFileSystem.h"

TScope<FNativeFileSystem> FFileSystem::Instance = MakeScope<ConcatWithPlatformNameDef(NativeFileSystem)>();

void FFileSystem::InitFS()
{
	Instance = MakeScope<ConcatWithPlatformNameDef(NativeFileSystem)>();
}

FString FFileSystem::GetLeadingDirectories(const FString& Filepath)
{
	uint_t LastSlash = Filepath.FindLastOf("/\\");
	return LastSlash != FString::Npos ? Filepath.Sub(0, LastSlash) : "";
}

FString FFileSystem::ExtractFilenameFromFilepath(const FString& Filepath)
{
	uint_t LastSlash = Filepath.FindLastOf("/\\");
	return LastSlash != FString::Npos ? Filepath.Sub(LastSlash+1) : Filepath;
}

void FFileSystem::SetWorkingDir(const FString& WorkingDir)
{
	Instance->SetWorkingDir(WorkingDir);
}

FString FFileSystem::GetWorkingDir()
{
	return Instance->GetWorkingDir();
}

bool FFileSystem::CreateNewDirectory(const FString& Path)
{
	return Instance->CreateNewDirectory(Path);
}

bool FFileSystem::CreateDirectories(const FString& Path)
{
	uint_t Pos = 0;
	bool bLastSuccess = false;

	while (Pos != FString::Npos)
	{
		Pos = Path.FindFirstOf("/\\", Pos + 1);
		bLastSuccess = CreateNewDirectory(*Path.Sub(0, Pos));
	}

	return bLastSuccess;
}

bool FFileSystem::DoesDirectoryExist(const FString& Path)
{
	return Instance->DoesDirectoryExist(Path);
}

void FFileSystem::SetupDirectoriesForFile(const FString& Filepath)
{
	CreateDirectories(GetLeadingDirectories(Filepath));
}

bool FFileSystem::WriteToFile(const FString& Filepath, const FString& Data)
{
	return Instance->WriteToFile(Filepath, Data);
}

bool FFileSystem::AppendToFile(const FString& Filepath, const FString& Data)
{
	return Instance->AppendToFile(Filepath, Data);
}

bool FFileSystem::DoesFileExist(const FString& Filepath)
{
	return Instance->DoesFileExist(Filepath);
}

uint_t FFileSystem::GetFileSize(const FString& Filepath)
{
	return Instance->GetFileSize(Filepath);
}

bool FFileSystem::ReadFile(const FString& Filepath, FString& OutData)
{
	return Instance->ReadFile(Filepath, OutData);
}
