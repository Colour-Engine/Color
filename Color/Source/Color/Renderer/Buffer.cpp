#include "ColorPCH.h"
#include "Buffer.h"

#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

uint32 SizeOfShaderDataType(EShaderDataType Type)
{
	switch (Type)
	{
	case EShaderDataType::Float:  return 4;
	case EShaderDataType::Float2: return 4 * 2;
	case EShaderDataType::Float3: return 4 * 3;
	case EShaderDataType::Float4: return 4 * 4;
	case EShaderDataType::Mat3:   return 4 * 3 * 3;
	case EShaderDataType::Mat4:   return 4 * 4 * 4;
	case EShaderDataType::Int:    return 4;
	case EShaderDataType::Int2:   return 4 * 2;
	case EShaderDataType::Int3:   return 4 * 3;
	case EShaderDataType::Int4:   return 4 * 4;
	case EShaderDataType::Bool:   return 1;
	}

	unreachable();
	return 0;
}

TRef<FVertexBuffer> FVertexBuffer::New(uint32 InSize)
{
	switch (FRendererAPI::GetAPI())
	{
	case ERendererAPI::None:   CL_CORE_ERROR("ERendererAPI::None is not a valid Renderer API!"); return nullptr;
	case ERendererAPI::OpenGL: return MakeRef<FOpenGLVertexBuffer>(InSize);
	}

	unreachable();
	return nullptr;
}

TRef<FVertexBuffer> FVertexBuffer::New(const float* InVertices, uint32 InSize)
{
	switch (FRendererAPI::GetAPI())
	{
	case ERendererAPI::None:   CL_CORE_ERROR("ERendererAPI::None is not a valid Renderer API!"); return nullptr;
	case ERendererAPI::OpenGL: return MakeRef<FOpenGLVertexBuffer>(InVertices, InSize);
	}

	unreachable();
	return nullptr;
}

TRef<FIndexBuffer> FIndexBuffer::New(const uint32* InIndices, uint32 InCount)
{
	switch (FRendererAPI::GetAPI())
	{
	case ERendererAPI::None:   CL_CORE_ERROR("ERendererAPI::None is not a valid Renderer API!"); return nullptr;
	case ERendererAPI::OpenGL: return MakeRef<FOpenGLIndexBuffer>(InIndices, InCount);
	}

	unreachable();
	return nullptr;
}
