#include "ColorPCH.h"
#include "UniformBuffer.h"

#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

TRef<FUniformBuffer> FUniformBuffer::New(uint32 Size, uint32 Binding)
{
	switch (FRendererAPI::GetAPI())
	{
	case ERendererAPI::None:   CL_CORE_ERROR("ERendererAPI::None is not a valid Renderer API!"); return nullptr;
	case ERendererAPI::OpenGL: return MakeRef<FOpenGLUniformBuffer>(Size, Binding);
	}

	unreachable();
	return nullptr;
}
