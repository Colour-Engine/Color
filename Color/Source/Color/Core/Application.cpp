#include "ColorPCH.h"
#include "Application.h"

#include "Renderer/Renderer.h"
#include "Misc/Random.h"

#include "Project/Project.h"
#include "Misc/MessageDialog.h"

#include "Utils/PlatformUtils.h"
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

	GRandom = FRandomGenerator64::SeededWithTime();
	CL_CORE_INFO("Initialized GRandom with the seed being the current time.");

	CL_CORE_INFO("Engine Build Info:");
	CL_CORE_INFO("  Compiler:   %s (%s)", CCompiler.Name, CCompiler.Abbreviation);
	CL_CORE_INFO("  Build Date: %s", __DATE__);
	CL_CORE_INFO("  Build Time: %s", __TIME__);

	const FApplicationSpecification& Specification = GetApplicationSpecification();
	{
		if (!Specification.WorkingDir.IsEmpty() && Specification.WorkingDir != ".")
		{
			CL_CORE_INFO("ApplicationSpecification requested a working directory change to the directory '%s'. The working directory will be changed.", *Specification.WorkingDir);
			FFileSystem::SetWorkingDir(Specification.WorkingDir);
		}
	}

	if (Specification.bAutoLoadProject)
	{
		FString ProjectFilepath = Specification.Name + COLOR_PROJECT_FILE_EXTENSION;
		CL_CORE_TRACE("Auto load project flag was set, trying to load project '%s'...", *ProjectFilepath);

		if (FProject::Load(ProjectFilepath))
		{
			CL_CORE_INFO("Successfully loaded the desired project file! Name of the loaded project (coming from the ProjectInfo) is '%s'", *FProject::GetActive()->GetProjectInfo().Name);
		}
		else
		{
			CL_CORE_ERROR("Failed to load project from file '%s'!", *ProjectFilepath);

			TScope<FMessageDialog> Dialog = FMessageDialog::New
			(
				"Failed to load project!",
				FString(
				"The application specification specified that the project file shall get automatically loaded. "
				"The engine tried to load the project file ") + ProjectFilepath + ", but the process failed. "
				"Would you like for the application to continue execution? Project-specific settings and directories/paths won't be set correctly, "
				"and will be left to the engine defaults. The application may even crash after this point in execution.",
				EDialogControls::YesNo,
				EDialogIcon::Error
			);
			
			EDialogResult Result = Dialog->CreateAndAwait();
			if (Result == EDialogResult::No)
			{
				Exit(ExitCode::ProjectLoadFailure);
			}

			CL_CORE_INFO("The user chose yes, resuming execution without a project...");
		}
	}
	else
	{
		CL_CORE_TRACE("Specification.bAutoLoadProject flag was set to false, calling HandleNoAutoLoadProject()...");
		HandleNoAutoLoadProject();

		if (FProject::GetActive())
		{
			CL_CORE_INFO("HandleNoAutoLoadProject seems to have successfully loaded/set an active project!");
		}
		else
		{
			if (!__Flg_nowarn_projectnull)
			{
				CL_CORE_WARN
				(
					"The application specification specified explicit project loading, but after the execution of HandleNoAutoLoadProject(), "
					"FProject::GetActive() still returned nullptr! Please refer to the comment section of FApplicationSpecification::bAutoLoadProject "
					"if you're unsure what this warning is about. You can ignore this if having no project loaded is your desired behavior. "
					"You can suppress this warning by setting the global \"__Flg_nowarn_projectnull\" boolean variable to true."
				);
			}
		}
	}

	Window = FWindow::New(Specification.WindowProps);
	if (!Window->Init())
	{
		CL_CORE_FATAL("Application window creation failure!");
		return;
	}
	Window->OnClose.Bind(CALL_ON_OBJ(this, Quit));

	// Center the window
	glm::ivec2 MonitorSize = FPlatformUtils::GetMonitorSize();
	glm::ivec2 WindowSize = Window->GetSize();
	Window->SetPos({ MonitorSize.x / 2 - WindowSize.x / 2, MonitorSize.y / 2 - WindowSize.y / 2 });

	CL_CORE_TRACE("Setted the application window up.");

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
		float Time = FPlatformUtils::GetTime();
		float DeltaTime = Time - LastFrameTime;
		LastFrameTime = Time;

		for (FLayer* Layer : LayerStack)
		{
			Layer->OnTick(DeltaTime);
		}

		Window->Update();
	}

	CleanUp();
}

void FApplication::CleanUp()
{
	FRenderer::Shutdown();
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

void FApplication::PushLayer(FLayer* Layer)
{
	LayerStack.PushLayer(Layer);
	Layer->OnAttach();
}

void FApplication::PushOverlay(FLayer* Overlay)
{
	LayerStack.PushOverlay(Overlay);
	Overlay->OnAttach();
}
