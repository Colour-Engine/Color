#pragma once

#include "Containers/String.h"

enum class ELogLevel : uint8
{
	Trace,
	Info,
	Warn,
	Error,
	Fatal
};

// Indexed by enum values themselves.
inline constexpr const char* LogLevelNames[] =
{
	"Trace",
	"Info",
	"Warn",
	"Error",
	"Fatal"
};

// Indexed by enum values themselves.
inline constexpr const char* LogLevelColors[] =
{
	"\x1b[38;5;8m",
	"\x1b[32m",
	"\x1b[33;1m",
	"\x1b[31m",
	"\x1b[41m"
};

struct FLoggerFileInfo
{
	FString OutputFile;
	bool bDoOutput = false;

	FLoggerFileInfo() = default;
	FLoggerFileInfo(const FString& InOutputFile, bool bDoOutput = true)
		: OutputFile(InOutputFile), bDoOutput(bDoOutput) { }

	void EnsureFile();
	void ResetFile();
};

class FLogger
{
public:
	FLogger
	(
		const FString&         Name       = "Unnamed Logger",
		const FLoggerFileInfo& FileInfo   = {},
		ELogLevel              Level      = ELogLevel::Trace,
		bool                   bUseColors = true
	);

	void LogTo(void* Stream, ELogLevel Level, const char* Format, va_list Arguments);
	void LogTo(void* Stream, ELogLevel Level, const char* Format, ...);

	// Logs to stdout.
	void Log(ELogLevel Level, const char* Format, va_list Arguments);
	void Log(ELogLevel Level, const char* Format, ...);

	// All of these log to stdout.
	void Trace(const char* Format, ...);
	void Info(const char* Format, ...);
	void Warn(const char* Format, ...);
	void Error(const char* Format, ...);
	void Fatal(const char* Format, ...);

	void SetName(const FString& NewName);
	void SetFileInfo(const FLoggerFileInfo& NewFileInfo);
	void SetOutputFile(const FString& NewOutputFile);
	void SetDoFileOutput(bool bNewDoOutput);
	void SetUseColors(bool bNewUseColors);

	const FString& GetName() const { return Name; }
	const FLoggerFileInfo& GetFileInfo() const { return FileInfo; }
	const FString& GetOutputFile() const { return FileInfo.OutputFile; }
	bool DoesFileOutput() const { return FileInfo.bDoOutput; }
	bool IsUsingColors() const { return bUseColors; }
private:
	FString Name;
	FLoggerFileInfo FileInfo;
	ELogLevel Level = ELogLevel::Trace;
	bool bUseColors = true;
};

#ifndef CL_SHIPPING

#define CL_LOGGER_TRACE(Logger, ...) Logger.Trace(__VA_ARGS__)
#define CL_LOGGER_INFO(Logger, ...)  Logger.Info(__VA_ARGS__)
#define CL_LOGGER_WARN(Logger, ...)  Logger.Warn(__VA_ARGS__)
#define CL_LOGGER_ERROR(Logger, ...) Logger.Error(__VA_ARGS__)
#define CL_LOGGER_FATAL(Logger, ...) Logger.Fatal(__VA_ARGS__)

#else

#define CL_LOGGER_TRACE(Logger, ...)
#define CL_LOGGER_INFO(Logger, ...)
#define CL_LOGGER_WARN(Logger, ...)
#define CL_LOGGER_ERROR(Logger, ...)
#define CL_LOGGER_FATAL(Logger, ...)

#endif
