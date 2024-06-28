#include "ColorPCH.h"
#include "FileSystem.h"

#include "Platform/Windows/WindowsFileSystem.h"
#include "Platform/Linux/LinuxFileSystem.h"

#include <stdlib.h>

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

FString FFileSystem::ExtractExtensionFromFilepath(const FString& Filepath)
{
	uint_t LastDot = Filepath.Rfind('.');
	return LastDot == FString::Npos || LastDot == Filepath.Len() - 1 ? "" : Filepath.Sub(LastDot + 1);
}

FString FFileSystem::AppendPath(const FString& Path, const FString& PathToAppend)
{
	FString Result = Path;
	FString AppendPath = PathToAppend;

	while (Result.Last() == '/' || Result.Last() == '\\')
	{
		Result.Pop();
	}

	// Quick hack to see if the base path is empty so we see if we can just return the second part.
	// Also without this we'd have a leading seperator in our result if the logic below all ran (after this if statement)
	if (Result.IsEmpty())
	{
		return AppendPath;
	}

	while (AppendPath.Front() == '/' || AppendPath.Front() == '\\')
	{
		AppendPath.PopFront();
	}

	uint_t FirstSeperator = Result.FindFirstOf("/\\");
	char SeperatorType = FirstSeperator != FString::Npos ? Result[FirstSeperator] : '/';

	const char* Strings[3] =
	{
		Result.Get(),
		&SeperatorType,
		AppendPath.Get()
	};

	uint_t Sizes[3] =
	{
		Result.Len(),
		1,
		AppendPath.Len()
	};

	return FString::BulkConcate(Strings, Sizes, 3);
}

FString FFileSystem::RemoveFilenameFromFilepath(const FString& Filepath)
{
	uint_t LastSeperator = Filepath.Rfind("/\\");
	return LastSeperator == FString::Npos ? "" : Filepath.Sub(0, LastSeperator);
}

void FFileSystem::SetWorkingDir(const FString& WorkingDir)
{
	Instance->SetWorkingDir(WorkingDir);
}

FString FFileSystem::GetWorkingDir()
{
	return Instance->GetWorkingDir();
}

FString FFileSystem::Abs(const FString& Path)
{
	return Instance->Abs(Path);
}

FString FFileSystem::Rel(const FString& Path)
{
	return Instance->Rel(Path);
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
