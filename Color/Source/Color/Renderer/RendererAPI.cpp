#include "ColorPCH.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

TScope<FRendererAPI> FRendererAPI::New()
{
	switch (API)
	{
	case ERendererAPI::None:   CL_CORE_ERROR("ERendererAPI::None is not a valid Renderer API!"); return nullptr;
	case ERendererAPI::OpenGL: return MakeScope<FOpenGLRendererAPI>();
	}

	unreachable();
	return nullptr;
}
