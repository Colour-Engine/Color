#include "SandboxApp.h"

// >>> Entry Point (main function) included here. <<<
// Include this header in only one translation unit (.cpp file etc.) to avoid duplicate symbol errors.
#include "Core/EntryPoint.h"

#include "Sandbox2D.h"
#include "SandboxGlobalSerializationManager.h"

FSandboxApp::FSandboxApp(const FCommandLine& InCommandLine)
	: FApplication(InCommandLine)
{
	PushLayer(new FSandbox2D());
}

FSandboxApp::~FSandboxApp()
{
}

IMPLEMENT_SPECIFICATION
(
	"Sandbox", // Application name
	".",       // Working directory
	{          // Window properties
		"Sandbox", // Title
		 1600,     // Width
		 900,      // Height
		 false,    // Fullscreen?
		 false,    // Resizable?
		 true      // VSync?
	}
);

IMPLEMENT_APPLICATION(FSandboxApp);
DECLARE_GLOBAL_SERIALIZATION_MANAGER(FSandboxGlobalSerializationManager);
