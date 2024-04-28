#include "ColorPCH.h"
#include "Logger.h"

#include "Utils/FileSystem.h"
#include "Misc/ExitCode.h"

#include <time.h>

FLogger::FLogger(const FString& Name, const FLoggerFileInfo& FileInfo, ELogLevel Level, bool bUseColors)
	: Name(Name), FileInfo(FileInfo), Level(Level), bUseColors(bUseColors)
{
	this->FileInfo.ResetFile();
}

void FLogger::LogTo(void* Stream, ELogLevel Level, const char* Format, va_list Arguments) const
{
	if (Level < this->Level)
	{
		return;
	}

	FString Message = FString::Format(Format, Arguments);
	FString NonColoredOutput;
	FString ColoredOutput;

	time_t RawTime;
	struct tm* LocalTime;
	char TimeBuffer[9];

	time(&RawTime);
	LocalTime = localtime(&RawTime);
	strftime(TimeBuffer, sizeof(TimeBuffer), "%H:%M:%S", LocalTime);

	if (FileInfo.bDoOutput)
	{
		FileInfo.EnsureFile();
		NonColoredOutput = FString::Format("[%s] (%s) %s: %s\n", TimeBuffer, LogLevelNames[(uint8) Level], *Name, *Message);

		FFileSystem::AppendToFile(FileInfo.OutputFile, NonColoredOutput);
	}

	if (bCanEverUseColors && bUseColors)
	{
		ColoredOutput = FString::Format("%s[%s] %s: %s\x1b[0m\n", LogLevelColors[(uint8) Level], TimeBuffer, *Name, *Message);
		fwrite(*ColoredOutput, sizeof(FString::CharType), ColoredOutput.Len(), (FILE*) Stream);
		fflush((FILE*) Stream);
	}
	else
	{
		if (NonColoredOutput.IsEmpty())
		{
			NonColoredOutput = FString::Format("[%s] (%s) %s: %s\n", TimeBuffer, LogLevelNames[(uint8) Level], *Name, *Message);
		}

		fwrite(*NonColoredOutput, sizeof(FString::CharType), ColoredOutput.Len(), (FILE*) Stream);
		fflush((FILE*) Stream);
	}

	if (Level == ELogLevel::Fatal)
	{
		char FatalNotify[] = "  \x1b[38;5;14m-> The application will be aborted due to that.\x1b[0m\n";
		fwrite(FatalNotify, 1, sizeof(FatalNotify), (FILE*) Stream);
		RtExit(ExitCode::FatalLog);
	}
}

void FLogger::LogTo(void* Stream, ELogLevel Level, const char* Format, ...) const
{
	va_list Arguments;
	va_start(Arguments, Format);

	LogTo(Stream, Level, Format, Arguments);
	va_end(Arguments);
}

void FLogger::Log(ELogLevel Level, const char* Format, va_list Arguments) const
{
	LogTo(stdout, Level, Format, Arguments);
}

void FLogger::Log(ELogLevel Level, const char* Format, ...) const
{
	va_list Arguments;
	va_start(Arguments, Format);

	Log(Level, Format, Arguments);
	va_end(Arguments);
}

void FLogger::Trace(const char* Format, ...) const
{
	va_list Arguments;
	va_start(Arguments, Format);

	Log(ELogLevel::Trace, Format, Arguments);
	va_end(Arguments);
}

void FLogger::Info(const char* Format, ...) const
{
	va_list Arguments;
	va_start(Arguments, Format);

	Log(ELogLevel::Info, Format, Arguments);
	va_end(Arguments);
}

void FLogger::Warn(const char* Format, ...) const
{
	va_list Arguments;
	va_start(Arguments, Format);

	Log(ELogLevel::Warn, Format, Arguments);
	va_end(Arguments);
}

void FLogger::Error(const char* Format, ...) const
{
	va_list Arguments;
	va_start(Arguments, Format);

	Log(ELogLevel::Error, Format, Arguments);
	va_end(Arguments);
}

void FLogger::Fatal(const char* Format, ...) const
{
	va_list Arguments;
	va_start(Arguments, Format);

	Log(ELogLevel::Fatal, Format, Arguments);
	va_end(Arguments);
}

void FLogger::SetName(const FString& NewName)
{
	Name = NewName;
}

void FLogger::SetFileInfo(const FLoggerFileInfo& NewFileInfo)
{
	FileInfo = NewFileInfo;
	FileInfo.ResetFile();
}

void FLogger::SetOutputFile(const FString& NewOutputFile)
{
	FileInfo.OutputFile = NewOutputFile;
	FileInfo.ResetFile();
}

void FLogger::SetDoFileOutput(bool bNewDoOutput)
{
	FileInfo.bDoOutput = bNewDoOutput;
}

void FLogger::SetUseColors(bool bNewUseColors)
{
	bUseColors = bNewUseColors;
}

void FLoggerFileInfo::EnsureFile() const
{
	if (OutputFile.ContainsAnyOf("/\\") && !FFileSystem::DoesDirectoryExist(FFileSystem::GetLeadingDirectories(OutputFile)))
	{
		ResetFile();
	}
}

void FLoggerFileInfo::ResetFile() const
{
	if (bDoOutput)
	{
		FFileSystem::SetupDirectoriesForFile(OutputFile);
		FFileSystem::WriteToFile(OutputFile, "");
	}
}
