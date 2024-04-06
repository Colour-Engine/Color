#include "ColorPCH.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

FOpenGLContext::FOpenGLContext(GLFWwindow* InNativeWindowHandle)
	: NativeWindowHandle(InNativeWindowHandle)
{
	checkf(NativeWindowHandle, "WindowHandle is null!");
}

bool FOpenGLContext::Init()
{
	glfwMakeContextCurrent(NativeWindowHandle);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		CL_CORE_ERROR("Failed to load OpenGL functionality (Glad initialization error)!");
		return false;
	}

	CL_CORE_INFO("OpenGL Info:");
	CL_CORE_INFO("  Vendor     -> %s", (const char*) glGetString(GL_VENDOR));
	CL_CORE_INFO("  Renderer   -> %s", (const char*) glGetString(GL_RENDERER));
	CL_CORE_INFO("  Version    -> %s", (const char*) glGetString(GL_VERSION));
	CL_CORE_INFO("  SL Version -> %s", (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (GLVersion.major < 4 || (GLVersion.major == 4 && GLVersion.minor < 5))
	{
		CL_CORE_ERROR(
			"Color Engine requires the GPU to provide at least OpenGL 4.5 functionality but the current renderer only supports up to %d.%d! "
			"Try updating your GPU drivers or changing your renderer if there are other ones available.",

			GLVersion.major,
			GLVersion.minor
		);

		return false;
	}

	return true;
}

void FOpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(NativeWindowHandle);
}
