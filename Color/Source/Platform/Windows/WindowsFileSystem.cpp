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

FString FWindowsNativeFileSystem::Abs(const FString& Path) const
{
	char Buffer[MAX_PATH];
	char** LppPart = { nullptr };

	GetFullPathNameA(*Path, MAX_PATH, Buffer, LppPart);
	return Buffer;
}

FString FWindowsNativeFileSystem::Rel(const FString& Path) const
{
	char Buffer[MAX_PATH];

	if (_fullpath(Buffer, *Path, MAX_PATH))
	{
		return Buffer;
	}

	return Path;
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
		CL_CORE_ERROR("Failed to open file '%s' for writing.", *Filepath);
		return false;
	}

	if (Data.Len() != 0)
	{
		DWORD NoBytesWritten = 0;
		BOOL bResult = WriteFile
		(
			hFile,
			Data.Get(),
			(DWORD) Data.Len(),
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
			CL_CORE_ERROR("Failed to write to file '%s'! Incorrect amount of bytes were written. Expected: %d, Written: %d.", *Filepath, Data.Len(), NoBytesWritten);
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

DWORD GNoBytesTransfered=0;
static void CALLBACK ReadFile_FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	GNoBytesTransfered = dwNumberOfBytesTransfered;
}

bool FWindowsNativeFileSystem::DoesFileExist(const FString& Filepath) const
{
	DWORD dwAttrib = GetFileAttributesA(*Filepath);
	return dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

uint_t FWindowsNativeFileSystem::GetFileSize(const FString& Filepath) const
{
	HANDLE hFile = CreateFileA
	(
		*Filepath,
		 GENERIC_READ,
		 FILE_SHARE_READ | FILE_SHARE_WRITE,
		 NULL,
		 OPEN_EXISTING,
		 FILE_ATTRIBUTE_NORMAL,
		 NULL
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		CL_CORE_ERROR("Failed to open file '%s' to retrieve it's size.", *Filepath);
		return 0;
	}

	LARGE_INTEGER Size;
	if (!GetFileSizeEx(hFile, &Size))
	{
		CL_CORE_ERROR("Failed to retrieve the size of the file '%s', GetFileSize returned FALSE.", *Filepath);
		CloseHandle(hFile);
		return 0;
	}

	CloseHandle(hFile);
	return Size.QuadPart;
}

bool FWindowsNativeFileSystem::ReadFile(const FString& Filepath, FString& OutData) const
{
	OutData.Clear();

	HANDLE hFile = CreateFileA
	(
		*Filepath,
		 GENERIC_READ,
		 FILE_SHARE_READ,
		 NULL,
		 OPEN_EXISTING,
		 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		 NULL
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		CL_CORE_ERROR("Failed to open file '%s' for reading.", *Filepath);
		return false;
	}

	LARGE_INTEGER LARGE_FileSize;
	if (!GetFileSizeEx(hFile, &LARGE_FileSize))
	{
		CL_CORE_ERROR("Failed to retrieve the size of the file '%s', GetFileSize returned FALSE.", *Filepath);
		CloseHandle(hFile);
		return 0;
	}

	DWORD FileSize = (DWORD) LARGE_FileSize.QuadPart;
	OutData.Resize(FileSize, '\0');

	OVERLAPPED Overlapped = { 0 };
	if (!ReadFileEx(hFile, OutData.Get(), FileSize, &Overlapped, ReadFile_FileIOCompletionRoutine))
	{
		CL_CORE_ERROR("Failed to read from file '%s'! ReadFile returned FALSE.", *Filepath);
		CloseHandle(hFile);
		return false;
	}

	// To clarify:
	// We give a stupid amount of time to the Sleep function to ensure it won't wake up (ironic, right)
	// But then, we'd just sleep forever. Well, that'd be the case with Sleep, but not with SleepEx.
	// TRUE is passed for the 2nd parameter (named bAlertable), if TRUE is passed, the program will wake up whenever an I/O routine finishes.
	// So, to give an overview, this code makes it so we sleep until an I/O routine finishes, which is when ReadFileEx does.
	SleepEx(0xffffffff, TRUE);

	if (GNoBytesTransfered != FileSize)
	{
		CL_CORE_ERROR("Failed to read from file '%s'! Incorrect amount of bytes were read. Expected: %d, Read: %d.", *Filepath, FileSize, GNoBytesTransfered);
		CloseHandle(hFile);
		return false;
	}

	return true;
}
