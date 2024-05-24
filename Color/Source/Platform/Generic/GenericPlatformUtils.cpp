#include "ColorPCH.h"
#include "GenericPlatformUtils.h"

#include <GLFW/glfw3.h>

float FGenericPlatformUtils::GetTime_Impl() const
{
	return (float) glfwGetTime();
}
