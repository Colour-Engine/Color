#include "ColorPCH.h"
#include "Application.h"

FApplication::FApplication()
{
	Instance = this;
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
		return;
	}

	bRunning = false;
}

void FApplication::Exit(ExitCode::Type ExitCode)
{
	RtExit(ExitCode);
}
