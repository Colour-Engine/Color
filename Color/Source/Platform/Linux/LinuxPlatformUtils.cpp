#include "ColorPCH.h"
#include "LinuxPlatformUtils.h"

#include <X11/Xlib.h>

glm::ivec2 FLinuxPlatformUtils::GetMonitorSize_Impl() const
{
	Display* VDisplay = XOpenDisplay(0);

	if (!VDisplay)
	{
		CL_CORE_ERROR("LinuxPlatformUtil failure, failed to open XDisplay ('%s') to retrieve the monitor size!", XDisplayName(0));
		return {};
	}

	int SNum = DefaultScreen(VDisplay);
	int Width = DisplayWidth(VDisplay, SNum);
	int Height = DisplayWidth(VDisplay, SNum);

	return { Width, Height };
}
