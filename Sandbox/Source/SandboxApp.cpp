#include "SandboxApp.h"

// >>> Entry Point (main function) included here. <<<
// Include this header in only one translation unit (.cpp file etc.) to avoid duplicate symbol errors.
#include "Core/EntryPoint.h"

#include "Containers/Array.h"

#include <iostream>

FSandboxApp::FSandboxApp()
{
	TArray<int> Array{1,2,3,4};
	Array.Resize(8);

	for (uint32 i = 0; i < Array.Num(); i++)
	{
		std::cout << Array[i] << '\n';
	}
}

FSandboxApp::~FSandboxApp()
{
}

IMPLEMENT_APPLICATION(FSandboxApp);
