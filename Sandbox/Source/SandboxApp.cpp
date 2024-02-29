#include "SandboxApp.h"

// >>> Entry Point (main function) included here. <<<
// Include this header in only one translation unit (.cpp file etc.) to avoid duplicate symbol errors.
#include "Core/EntryPoint.h"

#include "Containers/Array.h"

#include <iostream>
#include <string>

FSandboxApp::FSandboxApp()
{
	TArray<std::string> Array = { "Hello ", "world ", "from ", "Color", " Engine", "!" };
	Array.SetRange("USSR ");

	for (uint32 i = 0; i < Array.Num(); i++)
	{
		std::cout << Array[i];
	}

	std::cout << '\n';
}

FSandboxApp::~FSandboxApp()
{
}

IMPLEMENT_APPLICATION(FSandboxApp);
