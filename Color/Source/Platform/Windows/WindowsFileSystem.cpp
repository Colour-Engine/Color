#include "ColorPCH.h"
#include "WindowsFileSystem.h"

#include "Core/Log.h"

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

bool FWindowsNativeFileSystem::CreateNewDirectory(const FString& Path)
{
	return CreateDirectoryA(*Path, NULL);
}

bool FWindowsNativeFileSystem::DoesDirectoryExist(const FString& Path) const
{
	DWORD dwAttrib = GetFileAttributesA(*Path);
	return dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool FWindowsNativeFileSystem::WriteToFileEx(const FString& Filepath, const FString& Data, bool bAppend)
{
	HANDLE hFile = CreateFileA
	(
		*Filepath,
		 bAppend ? FILE_APPEND_DATA : GENERIC_WRITE,
		 0,
		 NULL,
		 OPEN_ALWAYS,
		 FILE_ATTRIBUTE_NORMAL,
		 NULL
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	if (Data.Len() != 0)
	{
		DWORD NoBytesWritten = 0;
		BOOL bResult = WriteFile
		(
			hFile,
			Data.Get(),
			Data.Len(),
			&NoBytesWritten,
			NULL
		);

		CloseHandle(hFile);
		if (!bResult)
		{
			CL_CORE_ERROR("Failed to write to file '%s'! WriteFile returned FALSE.", *Filepath);
			return false;
		}
		else if (NoBytesWritten != Data.Len())
		{
			CL_CORE_ERROR("Failed to write to file '%s'! Incorrect amount of bytes were written. Expected: %d, Written: %d.", Data.Len(), NoBytesWritten);
			return false;
		}

		return true;
	}
	else
	{
		LARGE_INTEGER Size = { 0 };

		if (!GetFileSizeEx(hFile, &Size))
		{
			CloseHandle(hFile);
			return false;
		}

		LARGE_INTEGER Dist = { 0 };
		Dist.QuadPart = 0 - Size.QuadPart;

		if (!SetFilePointerEx(hFile, Dist, NULL, FILE_END))
		{
			CloseHandle(hFile);
			return false;
		}

		if (!SetEndOfFile(hFile))
		{
			CloseHandle(hFile);
			return false;
		}

		CloseHandle(hFile);
		return true;
	}

	return false;
}

bool FWindowsNativeFileSystem::WriteToFile(const FString& Filepath, const FString& Data)
{
	return WriteToFileEx(Filepath, Data, false);
}

bool FWindowsNativeFileSystem::AppendToFile(const FString& Filepath, const FString& Data)
{
	return WriteToFileEx(Filepath, Data, true);
}
