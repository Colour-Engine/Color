#include "ColorPCH.h"
#include "Application.h"

#include "Renderer/Renderer.h"
#include "Utils/FileSystem.h"

FApplication::FApplication(const FCommandLine& InCommandLine)
	: CommandLine(InCommandLine)
{
	checkf(!Instance, "An application instance already exists!");
	Instance = this;

#ifdef CL_PLATFORM_WINDOWS
{
	HANDLE StdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode;

	GetConsoleMode(StdOutHandle, &dwMode);
	FLogger::bCanEverUseColors = SetConsoleMode(StdOutHandle, dwMode | (ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING));
}
#else
	// It's very likely that the terminal supports ANSI escape codes if it's a TTY.
	FLogger::bCanEverUseColors = isatty(STDOUT_FILENO);
#endif

	InitLog();
	CL_CORE_INFO("Initialized logging.");

	CL_CORE_INFO("Engine Build Info:");
	CL_CORE_INFO("  Compiler:   %s (%s)", CCompiler.Name, CCompiler.Abbreviation);
	CL_CORE_INFO("  Build Date: %s", __DATE__);
	CL_CORE_INFO("  Build Time: %s", __TIME__);

	const FApplicationSpecification& Specification = GetApplicationSpecification();
	{
		if (!Specification.WorkingDir.IsEmpty() && Specification.WorkingDir == ".")
		{
			CL_CORE_INFO("ApplicationSpecification requested a working directory change to the directory '%s'. The working directory will be changed.", *Specification.WorkingDir);
			FFileSystem::SetWorkingDir(Specification.WorkingDir);
		}
	}

	Window = FWindow::New(Specification.WindowProps);
	if (!Window->Init())
	{
		CL_CORE_FATAL("Application window creation failure!");
		return;
	}
	Window->OnClose.Bind(CALL_ON_OBJ(this, Quit));
	CL_CORE_TRACE("Created the application window.");

	FRenderer::Init();
	CL_CORE_TRACE("Initialized the Renderer.");
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
		OnPreAppTick();
		Window->Update();
		OnPostAppTick();
	}

	CleanUp();
}

void FApplication::CleanUp()
{
	Window->Destroy();
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
