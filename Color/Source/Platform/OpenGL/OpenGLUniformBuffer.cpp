#include "ColorPCH.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>
#include "OpenGLUniformBuffer.h"

FOpenGLUniformBuffer::FOpenGLUniformBuffer(uint32 Size, uint32 Binding)
{
	glCreateBuffers(1, &RendererID);
	glNamedBufferData(RendererID, Size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, Binding, RendererID);
}

FOpenGLUniformBuffer::~FOpenGLUniformBuffer()
{
	glDeleteBuffers(1, &RendererID);
}

void FOpenGLUniformBuffer::SetData(const void* Data, uint32 Size, uint32 Offset)
{
	glNamedBufferSubData(RendererID, Offset, Size, Data);
}
