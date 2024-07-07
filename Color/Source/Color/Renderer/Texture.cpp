#include "ColorPCH.h"
#include "Texture.h"

#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

TRef<FTexture2D> FTexture2D::New(const FTextureSpecification& InSpecification, FBuffer Buffer)
{
	switch (FRendererAPI::GetAPI())
	{
	case ERendererAPI::None:   CL_CORE_ERROR("ERendererAPI::None is not a valid Renderer API!"); return nullptr;
	case ERendererAPI::OpenGL: return MakeRef<FOpenGLTexture2D>(InSpecification, Buffer);
	}

	unreachable();
	return nullptr;
}
