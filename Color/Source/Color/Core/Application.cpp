#include "ColorPCH.h"
#include "Application.h"

#include "Utils/FileSystem.h"

FApplication::FApplication(const FCommandLine& InCommandLine)
	: CommandLine(InCommandLine)
{
	checkf(!Instance, "An application instance already exists!");
	Instance = this;

	const FApplicationSpecification& Specification = GetApplicationSpecification();
	{
		if (!Specification.WorkingDir.IsEmpty() && Specification.WorkingDir == ".")
		{
			CL_CORE_INFO("ApplicationSpecification requested a working directory change to the directory '%s'. The working directory will be changed.", *Specification.WorkingDir);
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
		CL_CORE_WARN("Cannot re-run an already running application instance!");
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
		CL_CORE_WARN("Cannot quit from a non-running application instance!");
		return;
	}

	CL_CORE_WARN("The application got a quit request, there won't be any updates after this frame.");
	bRunning = false;
}

void FApplication::Exit(ExitCode::Type ExitCode)
{
	CL_CORE_WARN("An immediate exit request was sent to the application with the exitcode '%d'. Exiting...", (int32) ExitCode);
	RtExit(ExitCode);
}
