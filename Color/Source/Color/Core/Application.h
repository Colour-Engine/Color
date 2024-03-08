#pragma once

#include "Core/Base.h"

#include "Misc/CommandLine.h"
#include "Misc/ExitCode.h"

// The application specification is used to specify certain attributes and properties of the application.
struct FApplicationSpecification
{
	// The internal name used for the application.
	FString Name;

	// The working/current directory the application will use within it's lifetime. Could be left as empty or "." to not change the inital one.
	FString WorkingDir;
};

class FApplication
{
public:
	FApplication(const FCommandLine& InCommandLine);
	virtual ~FApplication();

	// Starts the application loop.
	void Run();

	// Sends a quit request to the application, which will make it quit on the next frame with proper clean up.
	void Quit();

	// Immediately exits the application without any clean up whatsoever.
	void Exit(ExitCode::Type ExitCode);

	const FCommandLine& GetCommandLineArgs() const { return CommandLine; }
	FCommandLine& GetCommandLineArgs() { return CommandLine; }
	bool IsRunning() const { return bRunning; }

	static FApplication* Get() { return Instance; }
private:
	void CleanUp();
private:
	FCommandLine CommandLine;
	bool bRunning = false;
private:
	inline static FApplication* Instance = nullptr;
};

const FApplicationSpecification& GetApplicationSpecification();
FApplication* CreateApplication(const FCommandLine&);

#define IMPLEMENT_SPECIFICATION(...) \
const ::FApplicationSpecification& ::GetApplicationSpecification() \
{ \
	static ::FApplicationSpecification Specification = { __VA_ARGS__ }; \
	return Specification; \
}

#define IMPLEMENT_APPLICATION(ApplicationClass) \
::FApplication* ::CreateApplication(const ::FCommandLine& InCommandLine) \
{ \
	return new ApplicationClass(InCommandLine); \
}
