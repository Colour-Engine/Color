#include "ColorPCH.h"
#include "GraphicsContext.h"

#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLContext.h"

TScope<FGraphicsContext> FGraphicsContext::New(void* NativeWindowHandle)
{
	switch (FRendererAPI::GetAPI())
	{
	case ERendererAPI::None:   CL_CORE_ERROR("ERendererAPI::None is not a valid Renderer API!"); return nullptr;
	case ERendererAPI::OpenGL: return MakeScope<FOpenGLContext>((GLFWwindow*) NativeWindowHandle);
	}

	unreachable();
	return nullptr;
}
