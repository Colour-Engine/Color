#include "ColorPCH.h"
#include "Application.h"

#include "Utils/FileSystem.h"

FApplication::FApplication(const FCommandLine& InCommandLine)
	: CommandLine(InCommandLine)
{
	// TODO: Make sure Instance=nullptr
	Instance = this;

	const FApplicationSpecification& Specification = GetApplicationSpecification();
	{
		if (!Specification.WorkingDir.IsEmpty() && Specification.WorkingDir == ".")
		{
			// TODO: Log this
			FFileSystem::SetWorkingDir(Specification.WorkingDir);
		}
	}
}

FApplication::~FApplication()
{
	if (Instance == this)
	{
		Instance = nullptr;
	}
}

void FApplication::Run()
{
	if (bRunning)
	{
		// TODO: Warn
		return;
	}

	bRunning = true;

	while (bRunning)
	{

	}

	CleanUp();
}

void FApplication::CleanUp()
{
}

void FApplication::Quit()
{
	if (!bRunning)
	{
		// TODO: Warn
		return;
	}

	bRunning = false;
}

void FApplication::Exit(ExitCode::Type ExitCode)
{
	RtExit(ExitCode);
}
