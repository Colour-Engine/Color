#include "ColorPCH.h"
#include "Window.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Linux/LinuxWindow.h"

TScope<FWindow> FWindow::New(const FWindowProps& Props)
{
	return MakeScope<ConcatWithPlatformNameDef(Window)>(Props);
}
