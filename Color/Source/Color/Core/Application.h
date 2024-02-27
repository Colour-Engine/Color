#pragma once

#include "Core/Base.h"

#include "Misc/ExitCode.h"

class FApplication
{
public:
	FApplication();
	virtual ~FApplication();

	// Starts the application loop.
	void Run();

	// Sends a quit request to the application, which will make it quit on the next frame with proper clean up.
	void Quit();

	// Immediately exits the application without any clean up whatsoever.
	void Exit(ExitCode::Type ExitCode);

	bool IsRunning() const { return bRunning; }

	static FApplication* Get() { return Instance; }
private:
	void CleanUp();
private:
	bool bRunning = false;
private:
	inline static FApplication* Instance = nullptr;
};

FApplication* CreateApplication();

#define IMPLEMENT_APPLICATION(ApplicationClass) \
::FApplication* ::CreateApplication() \
{ \
	return new ApplicationClass(); \
}
