#include "ColorPCH.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

void FOpenGLBufferUtils::ConstructBuffer(uint32& RendererID, GLenum Target, const void* InData, uint32 InSize, GLenum Usage)
{
	glCreateBuffers(1, &RendererID);
	glBindBuffer(Target, RendererID);
	glBufferData(Target, InSize, InData, Usage);
}

FOpenGLVertexBuffer::FOpenGLVertexBuffer(uint32 InSize)
{
	FOpenGLBufferUtils::ConstructBuffer(RendererID, GL_ARRAY_BUFFER, nullptr, InSize, GL_DYNAMIC_DRAW);
}

FOpenGLVertexBuffer::FOpenGLVertexBuffer(const float* InVertices, uint32 InSize)
{
	FOpenGLBufferUtils::ConstructBuffer(RendererID, GL_ARRAY_BUFFER, InVertices, InSize, GL_STATIC_DRAW);
}

FOpenGLVertexBuffer::~FOpenGLVertexBuffer()
{
	glDeleteBuffers(1, &RendererID);
}

void FOpenGLVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, RendererID);
}

void FOpenGLVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FOpenGLVertexBuffer::SetData(const void* InData, uint32 InSize)
{
	glBindBuffer(GL_ARRAY_BUFFER, RendererID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, InSize, InData);
}

void FOpenGLVertexBuffer::SetLayout(const FBufferLayout& InLayout)
{
	Layout = InLayout;
}

FOpenGLIndexBuffer::FOpenGLIndexBuffer(const uint32* InIndices, uint32 InCount)
	: Count(InCount)
{
	FOpenGLBufferUtils::ConstructBuffer(RendererID, GL_ELEMENT_ARRAY_BUFFER, InIndices, InCount * sizeof(uint32), GL_STATIC_DRAW);
}

FOpenGLIndexBuffer::~FOpenGLIndexBuffer()
{
	glDeleteBuffers(1, &RendererID);
}

void FOpenGLIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID);
}

void FOpenGLIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
