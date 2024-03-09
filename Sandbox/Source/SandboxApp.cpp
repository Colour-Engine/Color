#include "SandboxApp.h"

// >>> Entry Point (main function) included here. <<<
// Include this header in only one translation unit (.cpp file etc.) to avoid duplicate symbol errors.
#include "Core/EntryPoint.h"

FSandboxApp::FSandboxApp(const FCommandLine& InCommandLine)
	: FApplication(InCommandLine)
{
}

FSandboxApp::~FSandboxApp()
{
}

IMPLEMENT_SPECIFICATION("Sandbox");
IMPLEMENT_APPLICATION(FSandboxApp);
