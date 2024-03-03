#include "SandboxApp.h"

// >>> Entry Point (main function) included here. <<<
// Include this header in only one translation unit (.cpp file etc.) to avoid duplicate symbol errors.
#include "Core/EntryPoint.h"

#include "Containers/String.h"
#include "Containers/Array.h"

#include <iostream>
#include <string>

FSandboxApp::FSandboxApp()
{
	FString ENG = "This is a string.";
	FString GER = "Das ist ein String.";
	FString TUR = "Bu bir string";

	TUR.Push('.');
	TUR.Append(2, '.');
	TUR.Append(" Simdi oncelik oradan buradan bu kodu goren butun Turk kardeslerime selamlarimi iletmek isterim. Allah yardimcimiz olsun bu zamanlarda :)");
	TUR += { ' ', 'N', 'e', ' ', 'd', 'i', 'y', 'e', 'b', 'i', 'l', 'i', 'r', 'i', 'm', ' ', 'k', 'i', '?' };

	std::cout << *ENG << '\n';
	std::cout << *GER << '\n';
	std::cout << *TUR << '\n';

	std::cout << "---------------------------------------------------------\n";
	std::cout << ~ENG << '\n';
	std::cout << "---------------------------------------------------------\n";

	int32 NumConvert = -8421312;
	double DblConvert = 532.861;
	float FltConvert = 324.523f;
	std::cout << NumConvert << " stringified to " << FString::FromInt(NumConvert) << '\n';
	std::cout << "532.861 stringified to " << FString::FromDouble(DblConvert) << '\n';
	std::cout << "324.523(f) stringified to " << FString::FromFloat(FltConvert) << '\n';
	std::cout << "---------------------------------------------------------\n";

	FString Formatted = FString::Format("%s %d %c %b %b %f %f", "Heyo", 45, 'a', false, true, 12.57, 912.544f);
	std::cout << "Formatted String Output:\n";
	std::cout << "  " << Formatted << '\n';
}

FSandboxApp::~FSandboxApp()
{
}

IMPLEMENT_APPLICATION(FSandboxApp);
