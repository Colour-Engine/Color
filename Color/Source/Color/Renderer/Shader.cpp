#include "ColorPCH.h"
#include "Shader.h"

#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

TRef<FShader> FShader::New(const FString& Filepath)
{
	switch (FRendererAPI::GetAPI())
	{
	case ERendererAPI::None:   CL_CORE_ERROR("ERendererAPI::None is not a valid Renderer API!"); return nullptr;
	case ERendererAPI::OpenGL: return MakeRef<FOpenGLShader>(Filepath);
	}

	unreachable();
	return nullptr;
}

TRef<FShader> FShader::New(const FString& Name, const FString& VertexSrc, const FString& FragmentSrc)
{
	switch (FRendererAPI::GetAPI())
	{
	case ERendererAPI::None:   CL_CORE_ERROR("ERendererAPI::None is not a valid Renderer API!"); return nullptr;
	case ERendererAPI::OpenGL: return MakeRef<FOpenGLShader>(Name, VertexSrc, FragmentSrc);
	}

	unreachable();
	return nullptr;
}
