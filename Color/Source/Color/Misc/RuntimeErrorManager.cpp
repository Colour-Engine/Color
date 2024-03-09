#include "ColorPCH.h"
#include "RuntimeErrorManager.h"

#include "Core/Logger.h"
#include "Misc/MessageDialog.h"

void FRuntimeErrorManager::RuntimeErrorLog(uint8 Level, const char* Format, va_list Arguments)
{
	if (Level < (uint8) ELogLevel::Error)
	{
		return;
	}

	FString Message = FString::Format(Format, Arguments);
	TScope<FMessageDialog> MessageDialog = FMessageDialog::New("", Message, EDialogControls::Ok, EDialogIcon::Error);

	switch ((ELogLevel) Level)
	{
	case ELogLevel::Error:
	{
		MessageDialog->SetCaption("Runtime error!");
		break;
	}
	case ELogLevel::Fatal:
	{
		MessageDialog->SetCaption("Fatal error!");
		MessageDialog->SetContent(MessageDialog->GetContent() + "\n\nThe application will be aborted due to that.");
		break;
	}
	}

	// This will block the current thread until the message box is closed.
	MessageDialog->CreateAndAwait();

	if (Level == (uint8) ELogLevel::Fatal)
	{
		RtExit(ExitCode::FatalLog);
	}
}

void FRuntimeErrorManager::RuntimeErrorLog(uint8 Level, const char* Format, ...)
{
	va_list Arguments;
	va_start(Arguments, Format);

	RuntimeErrorLog(Level, Format, Arguments);
	va_end(Arguments);
}

void FRuntimeErrorManager::RtError(const char* Format, ...)
{
	va_list Arguments;
	va_start(Arguments, Format);

	RuntimeErrorLog((uint8) ELogLevel::Error, Format, Arguments);
	va_end(Arguments);
}

void FRuntimeErrorManager::RtFatal(const char* Format, ...)
{
	va_list Arguments;
	va_start(Arguments, Format);

	RuntimeErrorLog((uint8) ELogLevel::Fatal, Format, Arguments);
	va_end(Arguments);
}
