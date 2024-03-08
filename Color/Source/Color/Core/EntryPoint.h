#pragma once

#include "Core/Application.h"

int32 EngineMain(int32 ArgumentCount, char** ArgumentList)
{
	FApplication* Application = CreateApplication({ ArgumentList, (uint32) ArgumentCount });
	Application->Run();

	delete Application;
	return ExitCode::Successful;
}

#if defined CL_SHIPPING && defined CL_PLATFORM_WINDOWS

#include <Windows.h>

int CALLBACK WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
)
{
	return EngineMain(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return EngineMain(argc, argv);
}

#endif
