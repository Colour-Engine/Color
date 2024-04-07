#include "ColorPCH.h"
#include "VertexArray.h"

#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

TRef<FVertexArray> FVertexArray::New()
{
	switch (FRendererAPI::GetAPI())
	{
	case ERendererAPI::None:   CL_CORE_ERROR("ERendererAPI::None is not a valid Renderer API!"); return nullptr;
	case ERendererAPI::OpenGL: return MakeRef<FOpenGLVertexArray>();
	}

	unreachable();
	return nullptr;
}
