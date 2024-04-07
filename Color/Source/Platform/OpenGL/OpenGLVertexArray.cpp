#include "ColorPCH.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

static GLenum ColorShaderDataTypeToOpenGLType(EShaderDataType Type)
{
	switch (Type)
	{
	case EShaderDataType::Float:
	case EShaderDataType::Float2:
	case EShaderDataType::Float3:
	case EShaderDataType::Float4:
	case EShaderDataType::Mat3:
	case EShaderDataType::Mat4:
	{
		return GL_FLOAT;
	}
	case EShaderDataType::Int:
	case EShaderDataType::Int2:
	case EShaderDataType::Int3:
	case EShaderDataType::Int4:
	{
		return GL_INT;
	}
	case EShaderDataType::Bool:
	{
		return GL_BOOL;
	}
	}

	unreachable();
	return 0;
}

FOpenGLVertexArray::FOpenGLVertexArray()
{
	glCreateVertexArrays(1, &RendererID);
}

FOpenGLVertexArray::~FOpenGLVertexArray()
{
	glDeleteVertexArrays(1, &RendererID);
}

void FOpenGLVertexArray::Bind() const
{
	glBindVertexArray(RendererID);
}

void FOpenGLVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void FOpenGLVertexArray::AddVertexBuffer(const TRef<FVertexBuffer>& VertexBuffer)
{
	checkf(VertexBuffer->GetLayout().GetElements().Num(), "Vertex Buffer has no layout!");

	glBindVertexArray(RendererID);
	VertexBuffer->Bind();

	const FBufferLayout& Layout = VertexBuffer->GetLayout();
	for (const FBufferElement& Element : Layout)
	{
		switch (Element.Type)
		{
		case EShaderDataType::Float:
		case EShaderDataType::Float2:
		case EShaderDataType::Float3:
		case EShaderDataType::Float4:
		{
			glEnableVertexAttribArray(VertexBufferIndex);
			glVertexAttribPointer
			(
				VertexBufferIndex,
				Element.GetComponentCount(),
				ColorShaderDataTypeToOpenGLType(Element.Type),
				Element.bNormalized ? GL_TRUE : GL_FALSE,
				Layout.GetStride(),
				(const void*)(Element.Offset)
			);

			VertexBufferIndex++;
			break;
		}
		case EShaderDataType::Int:
		case EShaderDataType::Int2:
		case EShaderDataType::Int3:
		case EShaderDataType::Int4:
		case EShaderDataType::Bool:
		{
			glEnableVertexAttribArray(VertexBufferIndex);
			glVertexAttribIPointer
			(
				VertexBufferIndex,
				Element.GetComponentCount(),
				ColorShaderDataTypeToOpenGLType(Element.Type),
				Layout.GetStride(),
				(const void*)(Element.Offset)
			);

			VertexBufferIndex++;
			break;
		}
		case EShaderDataType::Mat3:
		case EShaderDataType::Mat4:
		{
			uint8 Count = Element.GetComponentCount();
			for (uint8 i = 0; i < Count; i++)
			{
				glEnableVertexAttribArray(VertexBufferIndex);
				glVertexAttribPointer
				(
					VertexBufferIndex,
					Count,
					ColorShaderDataTypeToOpenGLType(Element.Type),
					Element.bNormalized ? GL_TRUE : GL_FALSE,
					Layout.GetStride(),
					(const void*)(Element.Offset + sizeof(float) * Count * i)
				);
				glVertexAttribDivisor(VertexBufferIndex++, 1); // NOTE: VertexBufferIndex is increased here (post-increment).
			}

			break;
		}
		default:
		{
			unreachable();
		}
		}
	}

	VertexBuffers.Add(VertexBuffer);
}

void FOpenGLVertexArray::SetIndexBuffer(const TRef<FIndexBuffer>& IndexBuffer)
{
	glBindVertexArray(RendererID);
	IndexBuffer->Bind();

	this->IndexBuffer = IndexBuffer;
}
